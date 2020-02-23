/**
 * thermistor_tolerance.cpp
 * Performs Monte Carlo tolerance analysis of an NTC thermistor and pullup resistor
 * measured by an ADC. The goal is to obtain worst-case error among all considered
 * effects. Error due to self-heating is not modelled.
 *
 * Written in 2019 by Ben Tesch.
 * Originally distributed at https://github.com/slugrustle/numerical_routines
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */
#include <cstdio>
#include <cinttypes>
#include <cmath>
#include <chrono>
#include <random>
#include <vector>
#include <array>
#include <limits>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <thread>
#include "parsers.h"

// Returns a random sample pulled from a normal distribution.
// Samples outside of [-3σ, +3σ] are thrown out.
double rand_trimmed_normal(std::normal_distribution<double>& distrib, std::mt19937_64 &rand_generator)
{
  double temp_output;
  double min_output = -3.0*distrib.stddev();
  double max_output = -min_output;
  do
  {
    temp_output = distrib(rand_generator);
  } while (temp_output > max_output || temp_output < min_output);
  return temp_output;
}

// Creates a simulated ADC with gain, offset, and INL/DNL errors.
// Fills in the vADC_thresholds and vADC_codes vectors.
void create_ADC(const int64_t ADC_counts, const double gain_error, const double offset_error, 
                const double DNL_error, const double INL_max_error, std::mt19937_64 &rand_generator,
                std::vector<double> &vADC_thresholds, std::vector<uint32_t> &vADC_codes)
{
  vADC_codes.clear();
  vADC_codes.reserve(ADC_counts);
  for (int64_t j = 0ll; j < ADC_counts; j++)
  {
    vADC_codes.push_back(static_cast<uint32_t>(j));
  }

  // Setting standard deviation to 1/3 the max error for each distribution
  std::normal_distribution<double> gain_err_dist(0.0, gain_error/3.0);
  std::normal_distribution<double> offset_err_dist(0.0, offset_error/3.0);
  std::normal_distribution<double> DNL_err_dist(0.0, DNL_error/3.0);

  double max_INL;
  uint32_t attempts = 0u;
  do
  {
    attempts++;
    vADC_thresholds.clear();
    vADC_thresholds.reserve(ADC_counts-1ll);
    double gain_slope = 1.0 + rand_trimmed_normal(gain_err_dist, rand_generator) / static_cast<double>(ADC_counts);
    double offset = rand_trimmed_normal(offset_err_dist, rand_generator);

    // DNL is not applied to the first threshold
    vADC_thresholds.push_back(gain_slope-0.5 + offset);

    for (int64_t j = 2ll; j < ADC_counts; j++)
    {
      double this_threshold = static_cast<double>(j)*gain_slope - 0.5 + offset;
      this_threshold += rand_trimmed_normal(DNL_err_dist, rand_generator);
      vADC_thresholds.push_back(this_threshold);
    }

    // Check max INL
    double endpoint_corr_slope = (vADC_thresholds.back() - vADC_thresholds.front()) / static_cast<double>(vADC_codes.back() - vADC_codes.at(1u));
    max_INL = 0.0;
    for (int64_t j = 1ll; j < ADC_counts-1ll; j++)
    {
      double endpoint_corr_line = vADC_thresholds.front() + static_cast<double>(vADC_codes.at(j + 1u) - vADC_codes.at(1u)) * endpoint_corr_slope;
      double this_INL = std::fabs(vADC_thresholds.at(j) - endpoint_corr_line);
      if (this_INL > max_INL)
      {
        max_INL = this_INL;
        if (max_INL > INL_max_error) break;
      }
    }
  } while (max_INL > INL_max_error);

  // Remove missing thresholds and codes that come from DNL (if any)
  std::vector<double>::iterator itr_this_thresh = vADC_thresholds.begin();
  std::vector<uint32_t>::iterator itr_this_code = vADC_codes.begin();
  std::vector<double>::iterator itr_next_thresh = itr_this_thresh + 1;
  std::vector<uint32_t>::iterator itr_next_code = itr_this_code + 1;
  while (itr_next_thresh != vADC_thresholds.end())
  {
    if (*itr_next_thresh < *itr_this_thresh)
    {
      itr_this_thresh = vADC_thresholds.erase(itr_this_thresh);
      itr_next_thresh = itr_this_thresh + 1;
      itr_this_code = vADC_codes.erase(itr_this_code);
      itr_next_code = itr_this_code + 1;
    }
    else
    {
      itr_next_thresh++;
      itr_this_thresh++;
      itr_next_code++;
      itr_this_code++;
    }
  }
}

// Return the ADC code for a given input count.
// Input count is (ADC_counts-1) * ADC pin voltage / MCU supply voltage.
// create_ADC() must be called before this function.
uint32_t sample_ADC(const double input_count, const std::vector<double> &vADC_thresholds, const std::vector<uint32_t> &vADC_codes)
{
  if (input_count >= vADC_thresholds.back())
  {
    return vADC_codes.back();
  }

  if (input_count < vADC_thresholds.front())
  {
    return vADC_codes.front();
  }

  for (size_t j = 0ull; j < vADC_thresholds.size(); j++)
  {
    if (input_count >= vADC_thresholds.at(j) && input_count < vADC_thresholds.at(j+1ull))
    {
      return vADC_codes.at(j+1ull);
    }
  }

  std::printf(u8"\n\n\n===========================================================================================\n");
  std::printf(u8"=== ERROR: sample_ADC() failed to convert an input to a code. This should never happen. ===\n");
  std::printf(u8"===        Any results from this run are suspect and should be discarded.               ===\n");
  std::printf(u8"\n===========================================================================================\n\n\n");
  return vADC_codes.front();
}

// Create nominal lookup table that translates from ADC output code
// to a temperature in °C. Fills in vLookup_temps.
void create_Lookup(const double Rpull_nom, const double Rntc_25C_nom, const double Beta_ntc_nom,
                   const int64_t ADC_counts, std::vector<double> &vLookup_temps)
{
  vLookup_temps.clear();
  vLookup_temps.reserve(ADC_counts);
  
  // The 0 code could come from a ratio less than 0.5/(ADC_counts-1) in
  // an ideal ADC. Use this instead of actual 0 to avoid invalid
  // arguments to log().
  double sub_ratio = 0.5 / static_cast<double>(ADC_counts-1ll);
  double Rntc_code = Rpull_nom * sub_ratio / (1.0 - sub_ratio);
  double Temp_for_code = 1.0 / ((1.0/298.15) + (1.0/Beta_ntc_nom) * std::log(Rntc_code / Rntc_25C_nom)) - 273.15;
  vLookup_temps.push_back(Temp_for_code);

  for (int64_t j = 1ll; j < ADC_counts - 1ll; j++)
  {
    sub_ratio = static_cast<double>(j) / static_cast<double>(ADC_counts - 1ll);
    Rntc_code = Rpull_nom * sub_ratio / (1.0 - sub_ratio);
    Temp_for_code = 1.0 / ((1.0 / 298.15) + (1.0 / Beta_ntc_nom) * std::log(Rntc_code / Rntc_25C_nom)) - 273.15;
    vLookup_temps.push_back(Temp_for_code);
  }

  // The last code could come from a ratio greater than (ADC_counts-1.5)/(ADC_counts-1)
  // in an ideal ADC. Use this instead of actual 1 to avoid
  // division by 0.
  sub_ratio = (static_cast<double>(ADC_counts) - 1.5)/static_cast<double>(ADC_counts - 1ll);
  Rntc_code = Rpull_nom * sub_ratio / (1.0 - sub_ratio);
  Temp_for_code = 1.0 / ((1.0 / 298.15) + (1.0 / Beta_ntc_nom) * std::log(Rntc_code / Rntc_25C_nom)) - 273.15;
  vLookup_temps.push_back(Temp_for_code);
}

// ADC trials are run multithreaded, where each thread runs this function.
// This is done because create_ADC() is expensive.
// This function simulates an imperfect ADC, then uses it to convert
// the worst-case thermistor/pullup resistor outputs at all evaluation temperatures
// to °C temperatures.
void ADC_thread(const uint64_t rand_seed, const uint32_t nTrials, const int64_t ADC_counts, const double gain_error,
  const double offset_error, const double DNL_error, const double INL_max_error,
  const std::vector<double> &vEval_temps, const std::vector<double> &vLookup_temps,
  const std::vector<double> &vMin_input_counts, const std::vector<double> &vMax_input_counts,
  std::vector<double> &vMin_temperatures, std::vector<double> &vMax_temperatures)
{
  std::mt19937_64 rand_generator(rand_seed);
  std::vector<double> vADC_thresholds;
  std::vector<uint32_t> vADC_codes;

  for (uint32_t jTrial = 0u; jTrial < nTrials; jTrial++)
  {
    create_ADC(ADC_counts, gain_error, offset_error, DNL_error, INL_max_error, rand_generator, vADC_thresholds, vADC_codes);
    for (uint32_t jTemperature = 0; jTemperature < vEval_temps.size(); jTemperature++)
    {
      uint32_t output_code = sample_ADC(vMin_input_counts.at(jTemperature), vADC_thresholds, vADC_codes);
      double converted_temp = vLookup_temps.at(output_code);
      vMin_temperatures.at(jTemperature) = std::min(converted_temp, vMin_temperatures.at(jTemperature));
      vMax_temperatures.at(jTemperature) = std::max(converted_temp, vMax_temperatures.at(jTemperature));

      output_code = sample_ADC(vMax_input_counts.at(jTemperature), vADC_thresholds, vADC_codes);
      converted_temp = vLookup_temps.at(output_code);
      vMin_temperatures.at(jTemperature) = std::min(converted_temp, vMin_temperatures.at(jTemperature));
      vMax_temperatures.at(jTemperature) = std::max(converted_temp, vMax_temperatures.at(jTemperature));
    }
  }
}

int main(int argc, char *argv[])
{
  // Help text. Just print it every time.
  std::printf(u8"\nMonte Carlo tolerance analysis of an NTC thermistor and pullup resistor\n");
  std::printf(u8"measured by an ADC. Error due to self-heating is not modelled.\n\n");

  std::printf(u8"If special characters (\u00B0, \u03A9, \u00B1, etc.) do not display,\n");
  std::printf(u8"set your console to unicode (PowerShell> chcp 65001).\n\n");

  std::printf(u8"[Input Arguments]\n");
  std::printf(u8"1.  Pullup resistor nominal resistance (\u03A9)\n");
  std::printf(u8"2.  Pullup resistor initial tolerance (%%)\n");
  std::printf(u8"3.  Pullup resistor temperature coefficient (ppm/\u00B0C)\n");
  std::printf(u8"4.  NTC nominal resistance at 25\u00B0C (\u03A9)\n");
  std::printf(u8"5.  Initial tolerance of NTC nominal resistance at 25\u00B0C (%%)\n");
  std::printf(u8"6.  NTC nominal \u03B2 coefficient (K)\n");
  std::printf(u8"7.  NTC \u03B2 coefficient initial tolerance (%%)\n");
  std::printf(u8"8.  ADC number of counts (1024 for 10-bit, 4096 for 12-bit, etc.)\n");
  std::printf(u8"9.  ADC max gain error (LSB)\n");
  std::printf(u8"10. ADC max offset error (LSB)\n");
  std::printf(u8"11. ADC max DNL error (LSB)\n");
  std::printf(u8"12. ADC max INL error (LSB)\n");
  std::printf(u8"13. Temperature at which to evaluate tolerance (\u00B0C)\n");
  std::printf(u8"(14, 15, ...). Optional additional temperatures at which to evaluate tolerance (\u00B0C)\n\n");

  std::printf(u8"[Example Command]\n");
  std::printf(u8".\\thermistor_tolerance.exe 47k 1 100 33k 5 3950 2 4096 3 2.8 1.3 1.7 -15 0 25 70\n\n\n");

  if (argc < 14)
  {
    std::printf(u8"Input Error: requires at least 13 arguments.\n");
    std::printf(u8"             Please read the tedious help text above.\n\n");
    return 0;
  }

  // Parse pullup resistance value
  std::string this_input = std::string(argv[1]);
  double Rpull_nom = parse_resistance(this_input);
  
  if (std::isnan(Rpull_nom))
  {
    std::printf(u8"Input Error: could not parse the pullup resistor nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rpull_nom < 1.0)
  {
    std::printf(u8"Input Error: the pullup resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <1\u03A9.\n\n");
    return 0;
  }
  else if (Rpull_nom > 100.0e6)
  {
    std::printf(u8"Input Error: the pullup resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100M\u03A9.\n\n");
    return 0;
  }

  // Parse pullup resistor tolerance
  this_input = std::string(argv[2]);
  double Rpull_tol = parse_double(this_input);

  if (std::isnan(Rpull_tol))
  {
    std::printf(u8"Input Error: could not parse the pullup resistor tolerance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rpull_tol < 0.0)
  {
    std::printf(u8"Input Error: the pullup resistor tolerance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (Rpull_tol >= 100.0)
  {
    std::printf(u8"Input Error: the pullup resistor tolerance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265100%%.\n\n");
    return 0;
  }

  // Fractions are just easier to deal with
  Rpull_tol *= 0.01;

  // Parse pullup resistor temperature coefficient
  this_input = std::string(argv[3]);
  double Rpull_tcr = parse_double(this_input);

  if (std::isnan(Rpull_tcr))
  {
    std::printf(u8"Input Error: could not parse the pullup resistor temperature coefficient\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rpull_tcr < 0.0)
  {
    std::printf(u8"Input Error: the pullup resistor temperature coefficient value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (Rpull_tcr >= 1.0e6)
  {
    std::printf(u8"Input Error: the pullup resistor temperature coefficient value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u22651,000,000ppm/\u00B0C.\n\n");
    return 0;
  }

  // Fractions are just easier to deal with
  Rpull_tcr *= 1.0e-6;

  // Parse NTC thermistor nominal resistance at 25C
  this_input = std::string(argv[4]);
  double Rntc_25C_nom = parse_resistance(this_input);

  if (std::isnan(Rntc_25C_nom))
  {
    std::printf(u8"Input Error: could not parse the NTC 25\u00B0C nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rntc_25C_nom < 1.0)
  {
    std::printf(u8"Input Error: the NTC 25\u00B0C nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <1\u03A9.\n\n");
    return 0;
  }
  else if (Rntc_25C_nom > 100.0e6)
  {
    std::printf(u8"Input Error: the NTC 25\u00B0C nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100M\u03A9.\n\n");
    return 0;
  }

  // Parse tolerance of NTC thermistor resistance at 25C
  this_input = std::string(argv[5]);
  double Rntc_25C_tol = parse_double(this_input);

  if (std::isnan(Rntc_25C_tol))
  {
    std::printf(u8"Input Error: could not parse the NTC 25\u00B0C resistance tolerance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rntc_25C_tol < 0.0)
  {
    std::printf(u8"Input Error: the NTC 25\u00B0C resistance tolerance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (Rntc_25C_tol >= 100.0)
  {
    std::printf(u8"Input Error: the NTC 25\u00B0C resistance tolerance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265100%%.\n\n");
    return 0;
  }

  // Fractions are just easier to deal with
  Rntc_25C_tol *= 0.01;

  // Parse NTC nominal Beta coefficient
  this_input = std::string(argv[6]);
  double Beta_ntc_nom = parse_double(this_input);

  if (std::isnan(Beta_ntc_nom))
  {
    std::printf(u8"Input Error: could not parse the NTC nominal \u03B2 coefficient\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Beta_ntc_nom < 100.0)
  {
    std::printf(u8"Input Error: the NTC nominal \u03B2 coefficient value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <100K.\n\n");
    return 0;
  }
  else if (Beta_ntc_nom > 100000.0)
  {
    std::printf(u8"Input Error: the NTC nominal \u03B2 coefficient value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100,000K.\n\n");
    return 0;
  }

  // Parse tolerance of NTC thermistor resistance at 25C
  this_input = std::string(argv[7]);
  double Beta_ntc_tol = parse_double(this_input);

  if (std::isnan(Beta_ntc_tol))
  {
    std::printf(u8"Input Error: could not parse the NTC \u03B2 coefficient tolerance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Beta_ntc_tol < 0.0)
  {
    std::printf(u8"Input Error: the NTC \u03B2 coefficient tolerance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (Beta_ntc_tol >= 100.0)
  {
    std::printf(u8"Input Error: the NTC \u03B2 coefficient tolerance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265100%%.\n\n");
    return 0;
  }

  // Fractions are just easier to deal with
  Beta_ntc_tol *= 0.01;

  // Parse number of ADC counts
  this_input = std::string(argv[8]);
  bool parse_ok = false;
  int64_t ADC_counts = parse_int64(this_input, parse_ok);

  if (!parse_ok)
  {
    std::printf(u8"Input Error: could not parse the ADC number of counts\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (ADC_counts < 8ll)
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <8.\n\n");
    return 0;
  }
  else if (ADC_counts > 4294967296ll)
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >4294967296.\n\n");
    return 0;
  }

  // Parse gain error
  this_input = std::string(argv[9]);
  double gain_error = parse_double(this_input);

  if (std::isnan(gain_error))
  {
    std::printf(u8"Input Error: could not parse the ADC max gain error\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (gain_error < 0.0)
  {
    std::printf(u8"Input Error: the ADC max gain error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (gain_error >= static_cast<double>(ADC_counts))
  {
    std::printf(u8"Input Error: the ADC max gain error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265%" PRIi64 "LSB\n", ADC_counts);
    std::printf(u8"             (the ADC number of counts).\n\n");
    return 0;
  }

  // Parse offset error
  this_input = std::string(argv[10]);
  double offset_error = parse_double(this_input);

  if (std::isnan(offset_error))
  {
    std::printf(u8"Input Error: could not parse the ADC max offset error\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (offset_error < 0.0)
  {
    std::printf(u8"Input Error: the ADC max offset error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (offset_error >= static_cast<double>(ADC_counts))
  {
    std::printf(u8"Input Error: the ADC max offset error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265%" PRIi64 "LSB\n", ADC_counts);
    std::printf(u8"             (the ADC number of counts).\n\n");
    return 0;
  }

  // Parse DNL error
  this_input = std::string(argv[11]);
  double DNL_error = parse_double(this_input);

  if (std::isnan(DNL_error))
  {
    std::printf(u8"Input Error: could not parse the ADC max DNL error\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (DNL_error < 0.0)
  {
    std::printf(u8"Input Error: the ADC max DNL error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be negative.\n\n");
    return 0;
  }
  else if (DNL_error >= static_cast<double>(ADC_counts))
  {
    std::printf(u8"Input Error: the ADC max DNL error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265%" PRIi64 "LSB\n", ADC_counts);
    std::printf(u8"             (the ADC number of counts).\n\n");
    return 0;
  }

  // Parse max INL error
  this_input = std::string(argv[12]);
  double INL_max_error = parse_double(this_input);

  if (std::isnan(INL_max_error))
  {
    std::printf(u8"Input Error: could not parse the ADC max INL error\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (INL_max_error < DNL_error)
  {
    std::printf(u8"Input Error: the ADC max INL error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.1fLSB\n", DNL_error);
    std::printf(u8"             (the ADC max DNL error value).\n\n");
    return 0;
  }
  else if (INL_max_error >= static_cast<double>(ADC_counts))
  {
    std::printf(u8"Input Error: the ADC max INL error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u2265%" PRIi64 "LSB\n", ADC_counts);
    std::printf(u8"             (the ADC number of counts).\n\n");
    return 0;
  }

  // Parse temperature evaluation points
  std::vector<double> vEval_temps;

  for (int jArg = 13; jArg < argc; jArg++)
  {
    this_input = std::string(argv[jArg]);
    double this_temp = parse_double(this_input);

    if (std::isnan(this_temp))
    {
      std::printf(u8"Input Error: could not parse the evaluation temperature\n");
      std::printf(u8"             \"%s\".\n\n", this_input.c_str());
      return 0;
    }
    else if (this_temp < -273.15)
    {
      std::printf(u8"Input Error: the evaluation temperature value\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
      return 0;
    }
    else if (this_temp >= 2054.0)
    {
      std::printf(u8"Input Error: the evaluation temperature value\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be \u22652054\u00B0C. The common\n");
      std::printf(u8"             resistor substrate Alumina melts @ 2054\u00B0C.\n\n");
      return 0;
    }

    vEval_temps.push_back(this_temp);
  }

  // sort evaluation temperatures and remove duplicates
  std::sort(vEval_temps.begin(), vEval_temps.end());
  std::vector<double>::iterator unique_itr;
  unique_itr = std::unique(vEval_temps.begin(), vEval_temps.end());
  vEval_temps.resize(std::distance(vEval_temps.begin(), unique_itr));

  // Redisplay input arguments back to user
  std::printf(u8"[Inputs]\n");
  std::printf(u8"Pullup Resistor: %.1f\u03A9 \u00B1%.1f%% initial, \u00B1%.0fppm/\u00B0C\n", Rpull_nom, 100.0*Rpull_tol, 1.0e6*Rpull_tcr);
  std::printf(u8"NTC: %.1f\u03A9 \u00B1%.1f%% @ 25\u00B0C, \u03B2 = %.0fK \u00B1%.1f%%\n", Rntc_25C_nom, 100.0*Rntc_25C_tol, Beta_ntc_nom, 100.0*Beta_ntc_tol);
  std::printf(u8"ADC: %" PRIi64 " counts, \u00B1%.1fLSB max gain error, \u00B1%.1fLSB max offset error\n", ADC_counts, gain_error, offset_error);
  std::printf(u8"     \u00B1%.1fLSB max DNL error, \u00B1%.1fLSB max INL error\n\n", DNL_error, INL_max_error);
  
  // Get a random seed base for the RNGs
  uint64_t rand_seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::uniform_real_distribution<double> Rntc_25C_dist((1.0 - Rntc_25C_tol)*Rntc_25C_nom, (1.0 + Rntc_25C_tol)*Rntc_25C_nom);
  std::uniform_real_distribution<double> Beta_ntc_dist((1.0 - Beta_ntc_tol)*Beta_ntc_nom, (1.0 + Beta_ntc_tol)*Beta_ntc_nom);
  std::uniform_real_distribution<double> Rpull_dist((1.0 - Rpull_tol)*Rpull_nom, (1.0 + Rpull_tol)*Rpull_nom);
  std::uniform_int_distribution<uint32_t> Tcr_dist(0u, 1u);

  std::printf(u8"[Tolerance analysis]\n");

  // Get min and max ADC inputs for all temperatures first and store in these vectors
  std::vector<double> vMin_input_counts;
  std::vector<double> vMax_input_counts;
  std::mt19937_64 rand_generator(rand_seed);

  for (uint32_t jTemperature = 0; jTemperature < vEval_temps.size(); jTemperature++)
  {
    double this_temperature = vEval_temps.at(jTemperature);
    double inv_temp_kelvin = 1.0 / (this_temperature + 273.15);
    double max_input_count = std::numeric_limits<double>::lowest();
    double min_input_count = std::numeric_limits<double>::max();

    for (uint32_t jTrial = 0u; jTrial < 1000000u; jTrial++)
    {
      double Rpull = Rpull_dist(rand_generator);
      if (Tcr_dist(rand_generator))
      {
        Rpull *= (1.0 + Rpull_tcr * (this_temperature - 25.0));
      }
      else
      {
        Rpull *= (1.0 - Rpull_tcr * (this_temperature - 25.0));
      }
      double Rntc_25C = Rntc_25C_dist(rand_generator);
      double Beta_ntc = Beta_ntc_dist(rand_generator);
      double Rntc = Rntc_25C * std::exp(Beta_ntc * (inv_temp_kelvin - 1.0/298.15));
      double this_input_count = static_cast<double>(ADC_counts-1ll) * Rntc / (Rntc + Rpull);
      max_input_count = std::max(this_input_count, max_input_count);
      min_input_count = std::min(this_input_count, min_input_count);
    }

    vMin_input_counts.push_back(min_input_count);
    vMax_input_counts.push_back(max_input_count);
  }

  // Run ADC conversions only on the worst-case ADC inputs found earlier.
  // Do this multithreaded to make the runtime reasonable.

  // Fill in lookup table from ADC output codes to °C temperatures
  std::vector<double> vLookup_temps;
  create_Lookup(Rpull_nom, Rntc_25C_nom, Beta_ntc_nom, ADC_counts, vLookup_temps);

  uint32_t nThreads = std::max(1u, std::thread::hardware_concurrency());
  std::vector<std::thread> vADC_threads(nThreads);

  // Wort-case converted temperatures are stored per thread in these
  // vector vectors to remove the need for resource locking.
  std::vector<std::vector<double> > vADC_thread_vMin_temperatures(nThreads, std::vector<double>(vEval_temps.size(), std::numeric_limits<double>::max()));
  std::vector<std::vector<double> > vADC_thread_vMax_temperatures(nThreads, std::vector<double>(vEval_temps.size(), std::numeric_limits<double>::lowest()));

  const uint32_t nTrials = 100000u;
  uint32_t trials_per_thread = nTrials / nThreads;
  uint32_t trials_remainder = nTrials % nThreads;
  
  for (uint32_t jThread = 0u; jThread < nThreads; jThread++)
  {
    uint32_t trials_this_thread = trials_per_thread;
    if (jThread < trials_remainder) trials_this_thread++;
    // A different RNG seed per thread.
    uint64_t this_seed = rand_seed + 1234567ull * (static_cast<uint64_t>(jThread) + 1ull);
    vADC_threads.at(jThread) = std::move(std::thread(ADC_thread, this_seed, trials_this_thread, ADC_counts,
                                                     gain_error, offset_error, DNL_error, INL_max_error,
                                                     std::ref(vEval_temps), std::ref(vLookup_temps),
                                                     std::ref(vMin_input_counts), std::ref(vMax_input_counts),
                                                     std::ref(vADC_thread_vMin_temperatures.at(jThread)),
                                                     std::ref(vADC_thread_vMax_temperatures.at(jThread))));
  }

  for (uint32_t jThread = 0u; jThread < nThreads; jThread++)
  {
    vADC_threads.at(jThread).join();
  }

  // Display results
  for (uint32_t jTemperature = 0; jTemperature < vEval_temps.size(); jTemperature++)
  {
    double this_temperature = vEval_temps.at(jTemperature);

    // Get the worst-case converted temperatures across all threads
    double min_temperature = std::numeric_limits<double>::max();
    double max_temperature = std::numeric_limits<double>::lowest();
    for (uint32_t jThread = 0u; jThread < nThreads; jThread++)
    {
      min_temperature = std::min(min_temperature, vADC_thread_vMin_temperatures.at(jThread).at(jTemperature));
      max_temperature = std::max(max_temperature, vADC_thread_vMax_temperatures.at(jThread).at(jTemperature));
    }

    std::printf(u8"Eval @ %7.2f\u00B0C: min %7.2f\u00B0C (%+8.3f\u00B0C), max %7.2f\u00B0C (%+8.3f\u00B0C)\n", 
                this_temperature, min_temperature, min_temperature-this_temperature, max_temperature, max_temperature-this_temperature);
  }
  std::printf(u8"\n");
  return 0;
}

/*
Creative Commons Legal Code

CC0 1.0 Universal

    CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
    LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
    ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
    INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
    REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
    PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
    THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
    HEREUNDER.

Statement of Purpose

The laws of most jurisdictions throughout the world automatically confer
exclusive Copyright and Related Rights (defined below) upon the creator
and subsequent owner(s) (each and all, an "owner") of an original work of
authorship and/or a database (each, a "Work").

Certain owners wish to permanently relinquish those rights to a Work for
the purpose of contributing to a commons of creative, cultural and
scientific works ("Commons") that the public can reliably and without fear
of later claims of infringement build upon, modify, incorporate in other
works, reuse and redistribute as freely as possible in any form whatsoever
and for any purposes, including without limitation commercial purposes.
These owners may contribute to the Commons to promote the ideal of a free
culture and the further production of creative, cultural and scientific
works, or to gain reputation or greater distribution for their Work in
part through the use and efforts of others.

For these and/or other purposes and motivations, and without any
expectation of additional consideration or compensation, the person
associating CC0 with a Work (the "Affirmer"), to the extent that he or she
is an owner of Copyright and Related Rights in the Work, voluntarily
elects to apply CC0 to the Work and publicly distribute the Work under its
terms, with knowledge of his or her Copyright and Related Rights in the
Work and the meaning and intended legal effect of CC0 on those rights.

1. Copyright and Related Rights. A Work made available under CC0 may be
protected by copyright and related or neighboring rights ("Copyright and
Related Rights"). Copyright and Related Rights include, but are not
limited to, the following:

  i. the right to reproduce, adapt, distribute, perform, display,
     communicate, and translate a Work;
 ii. moral rights retained by the original author(s) and/or performer(s);
iii. publicity and privacy rights pertaining to a person's image or
     likeness depicted in a Work;
 iv. rights protecting against unfair competition in regards to a Work,
     subject to the limitations in paragraph 4(a), below;
  v. rights protecting the extraction, dissemination, use and reuse of data
     in a Work;
 vi. database rights (such as those arising under Directive 96/9/EC of the
     European Parliament and of the Council of 11 March 1996 on the legal
     protection of databases, and under any national implementation
     thereof, including any amended or successor version of such
     directive); and
vii. other similar, equivalent or corresponding rights throughout the
     world based on applicable law or treaty, and any national
     implementations thereof.

2. Waiver. To the greatest extent permitted by, but not in contravention
of, applicable law, Affirmer hereby overtly, fully, permanently,
irrevocably and unconditionally waives, abandons, and surrenders all of
Affirmer's Copyright and Related Rights and associated claims and causes
of action, whether now known or unknown (including existing as well as
future claims and causes of action), in the Work (i) in all territories
worldwide, (ii) for the maximum duration provided by applicable law or
treaty (including future time extensions), (iii) in any current or future
medium and for any number of copies, and (iv) for any purpose whatsoever,
including without limitation commercial, advertising or promotional
purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
member of the public at large and to the detriment of Affirmer's heirs and
successors, fully intending that such Waiver shall not be subject to
revocation, rescission, cancellation, termination, or any other legal or
equitable action to disrupt the quiet enjoyment of the Work by the public
as contemplated by Affirmer's express Statement of Purpose.

3. Public License Fallback. Should any part of the Waiver for any reason
be judged legally invalid or ineffective under applicable law, then the
Waiver shall be preserved to the maximum extent permitted taking into
account Affirmer's express Statement of Purpose. In addition, to the
extent the Waiver is so judged Affirmer hereby grants to each affected
person a royalty-free, non transferable, non sublicensable, non exclusive,
irrevocable and unconditional license to exercise Affirmer's Copyright and
Related Rights in the Work (i) in all territories worldwide, (ii) for the
maximum duration provided by applicable law or treaty (including future
time extensions), (iii) in any current or future medium and for any number
of copies, and (iv) for any purpose whatsoever, including without
limitation commercial, advertising or promotional purposes (the
"License"). The License shall be deemed effective as of the date CC0 was
applied by Affirmer to the Work. Should any part of the License for any
reason be judged legally invalid or ineffective under applicable law, such
partial invalidity or ineffectiveness shall not invalidate the remainder
of the License, and in such case Affirmer hereby affirms that he or she
will not (i) exercise any of his or her remaining Copyright and Related
Rights in the Work or (ii) assert any associated claims and causes of
action with respect to the Work, in either case contrary to Affirmer's
express Statement of Purpose.

4. Limitations and Disclaimers.

 a. No trademark or patent rights held by Affirmer are waived, abandoned,
    surrendered, licensed or otherwise affected by this document.
 b. Affirmer offers the Work as-is and makes no representations or
    warranties of any kind concerning the Work, express, implied,
    statutory or otherwise, including without limitation warranties of
    title, merchantability, fitness for a particular purpose, non
    infringement, or the absence of latent or other defects, accuracy, or
    the present or absence of errors, whether or not discoverable, all to
    the greatest extent permissible under applicable law.
 c. Affirmer disclaims responsibility for clearing rights of other persons
    that may apply to the Work or any use thereof, including without
    limitation any person's Copyright and Related Rights in the Work.
    Further, Affirmer disclaims responsibility for obtaining any necessary
    consents, permissions or other rights required for any use of the
    Work.
 d. Affirmer understands and acknowledges that Creative Commons is not a
    party to this document and has no duty or obligation with respect to
    this CC0 or use of the Work.
*/