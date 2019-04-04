/**
 * thermistor_interpolator.cpp
 *
 * A near-optimal thermistor curve interpolation table generator.
 * Generates a table of line segments interpolating the temperature
 * vs. ADC counts curve for an NTC thermistor with specified parameters
 * such that the table error is below some bound.
 * Effort is made to minimize the number of interpolating segments
 * required to acheive this.
 *
 * Written in 2019 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */

#include <cstdio>
#include <cinttypes>
#include <cmath>
#include <cassert>
#include <limits>
#include <vector>
#include <Eigen/Dense>
#include "multshiftround_run.hpp"

/**
 * Major variables used in calculations.
 * These are also taken as input arguments from
 * the command line.
 */
double min_table_temp_C;
double max_table_temp_C;
double Rntc_nom_Ohms;
double NTC_nom_temp_C;
double beta_K;
double Rpullup_nom_Ohms;
double Riso_nom_Ohms;
uint16_t ADC_counts;
double max_interp_error_C;

/**
 * Calculates nominal NTC resistance in Ohms
 * given an ADC reading on the range [0, ADC_counts - 1].
 */
double Rntc_from_ADCread(const uint16_t ADCread)
{
  assert(ADCread < ADC_counts);
  
  double ADCratio;
  if (ADCread == 0u) ADCratio = 0.5 / static_cast<double>(ADC_counts - 1u);
  else if (ADCread == ADC_counts - 1u) ADCratio = (static_cast<double>(ADC_counts) - 1.5) / static_cast<double>(ADC_counts - 1u);
  else ADCratio = static_cast<double>(ADCread) / static_cast<double>(ADC_counts - 1u);
  return (Rpullup_nom_Ohms * ADCratio - Riso_nom_Ohms * (1.0 - ADCratio)) / (1.0 - ADCratio);
}

/**
 * Calculates nominal NTC resistance in Ohms for a given
 * NTC temperature in degrees Celsius
 */
double Rntc_from_Tntc(double Tntc)
{
  assert(Tntc >= -273.15);
  assert(std::isfinite(Tntc));

  return Rntc_nom_Ohms * std::exp(beta_K * (1.0 / (Tntc + 273.15) - 1.0 / (NTC_nom_temp_C + 273.15)));
}

/**
 * Calculates nominal NTC temperature in degrees
 * Celsius given an ADC reading on the range
 * [0, ADC_counts - 1].
 * Returns NaN for infeasible ADC readings.
 */
double Tntc_from_ADCread(const uint16_t ADCread)
{
  assert(ADCread < ADC_counts);

  double Rntc = Rntc_from_ADCread(ADCread);
  if (Rntc <= 0.0) return std::numeric_limits<double>::quiet_NaN();
  return 1.0 / (std::log(Rntc/Rntc_nom_Ohms) / beta_K + 1.0 / (273.15 + NTC_nom_temp_C)) - 273.15;
}

/**
 * Calculates nominal ADC reading for a given
 * NTC temperature in degrees Celsius
 */
uint16_t ADCread_from_Tntc(double Tntc)
{
  assert(Tntc >= -273.15);
  assert(std::isfinite(Tntc));

  double Rntc = Rntc_from_Tntc(Tntc);
  assert(Rntc >= 0.0);

  double ADCratio = (Rntc + Riso_nom_Ohms) / (Rntc + Riso_nom_Ohms + Rpullup_nom_Ohms);
  return static_cast<uint16_t>(std::round(ADCratio * static_cast<double>(ADC_counts - 1u)));
}

/**
 * Convert a floating point degrees Celsius temperature
 * into (1/128) degrees Celsius fixed point.
 */
int16_t fixed_point_C(double temp_C)
{
  assert(temp_C >= -256.0);
  assert(temp_C <= 255.9921875);

  return static_cast<int16_t>(std::round(128.0 * temp_C));
}

/**
 * interp_segment_t defines a single linear interpolation
 *                  segment.
 *
 * start_count: the ADC count value corresponding to 
 *              start_temp
 *
 * start_temp: the temperature corresponding to start_count
 *             in 1/128ths of a degree Celsius.
 *             This is signed Q9.7 format fixed point.
 *
 * slope_multiplier: these two define the slope of the
 * slope_shift:      line segment as the rational number
 *                   (slope_multiplier / 2^slope_shift).
 *                   Units are 1/128ths of a degree Celsius
 *                   per ADC count.
 *
 * Each segment ends one count before the start of the
 * next segment. end_count in interp_table_t gives the last
 * valid ADC count for the final segment.
 */
typedef struct
{
  uint16_t start_count;
  int16_t start_temp;
  int32_t slope_multiplier;
  uint8_t slope_shift;
} interp_segment_t;

/**
 * Holds fit statistics for a single interpolation segment.
 * Only used for informational purposes.
 */
typedef struct
{
  uint16_t num_points;
  double mean_error;
  double max_error;
} segment_stats_t;

/**
 * Parses a string to an int64_t. Sets success true/false based on
 * whether the entire input string was an integer or not.
 */
int64_t parse_int64(const std::string &in_str, bool &success)
{
  if (in_str.length() == 0u)
  {
    success = false;
    return 0ll;
  }

  size_t after_int = 0u;
  int64_t value = 0ll;
  try
  {
    value = std::stoll(in_str, &after_int, 10);
  }
  catch (const std::invalid_argument &e)
  {
    success = false;
    return 0ll;
  }
  catch (const std::out_of_range &e)
  {
    success = false;
    return 0ll;
  }

  if (after_int != in_str.length())
  {
    success = false;
    return 0ll;
  }

  success = true;
  return value;
}

/**
 * Parses a string to a double. Returns NaN if it can't.
 */
double parse_double(const std::string &in_str)
{
  if (in_str.length() == 0u) return std::numeric_limits<double>::quiet_NaN();

  size_t after_double = 0u;
  double value = std::numeric_limits<double>::quiet_NaN();
  try
  {
    value = std::stod(in_str, &after_double);
  }
  catch (const std::invalid_argument &e)
  {
    return std::numeric_limits<double>::quiet_NaN();
  }
  catch (const std::out_of_range &e)
  {
    return std::numeric_limits<double>::quiet_NaN();
  }

  if (after_double != in_str.length()) return std::numeric_limits<double>::quiet_NaN();
  return value;
}

/**
 * Parse resistances such as 33.2k, 10M, 100.2, 1, etc.
 * into a value in Ohms. Only the suffixes k and M are
 * recognized.
 * Return NaN if not parseable.
 * Negative and 0 values are returned as valid.
 */
double parse_resistance(std::string &res_string)
{
  if (res_string.length() == 0u) return std::numeric_limits<double>::quiet_NaN();

  // Look for a k or M suffix
  char suffix = ' ';
  size_t suffix_idx = 0u;
  if (res_string.length() > 1u)
  {
    suffix_idx = res_string.length() - 1u;
    suffix = res_string.at(suffix_idx);
  }

  std::string number_buffer;
  if ('k' == suffix || 'M' == suffix) number_buffer = res_string.substr(0u, suffix_idx);
  else number_buffer = res_string;

  double res_val = parse_double(number_buffer);
  if (std::isnan(res_val)) return res_val;
  if ('k' == suffix) return 1000.0 * res_val;
  if ('M' == suffix) return 1.0e6 * res_val;
  return res_val;
}

int main (int argc, char **argv)
{
  /**
   * Help text. Print it every time.
   */
  std::printf(u8"\nNear-optimal thermistor curve interpolation table generator.\n");
  std::printf(u8"Generates a table of line segments interpolating the temperature\n");
  std::printf(u8"vs. ADC counts curve for an NTC thermistor with specified parameters\n");
  std::printf(u8"such that the table error is below some bound.\n");
  std::printf(u8"Effort is made to minimize the number of interpolating segments\n");
  std::printf(u8"required to acheive this.\n\n");

  std::printf(u8"If special characters (\u00B0, \u03A9, \u00B1, etc.) do not display,\n");
  std::printf(u8"set your console to unicode (PowerShell> chcp 65001).\n\n");

  std::printf(u8"[Input Arguments]\n");
  std::printf(u8"1. Lowest table temperature (\u00B0C)\n");
  std::printf(u8"2. Highest table temperature (\u00B0C)\n");
  std::printf(u8"3. NTC nominal resistance (\u03A9)\n");
  std::printf(u8"4. Temperature for NTC nominal resistance (\u00B0C)\n");
  std::printf(u8"5. NTC nominal \u03B2 coefficient (K)\n");
  std::printf(u8"6. Pullup resistor nominal resistance (\u03A9)\n");
  std::printf(u8"   - The pullup resistor connects between the NTC\n");
  std::printf(u8"     and the positive voltage supply.\n");
  std::printf(u8"7. Isolation resistor nominal resistance (\u03A9)\n");
  std::printf(u8"   - The isolation resistor connects between the NTC\n");
  std::printf(u8"     and GND.\n");
  std::printf(u8"8. ADC number of counts (1024 for 10-bit, 4096 for 12-bit, etc.)\n");
  std::printf(u8"9. Maximum error due to interpolation (\u00B0C)\n\n");

  std::printf(u8"[Example Command]\n");
  std::printf(u8".\\thermistor_interpolator.exe -30 90 33k 25 3950 22.1k 1.3k 4096 0.1\n\n\n");

  if (argc != 10)
  {
    std::printf(u8"Input Error: requires exactly 9 arguments.\n");
    std::printf(u8"             Please read the tedious help text above.\n\n");
    return 0;
  }

  /**
   * Parse the lowest table temperature input
   */
  std::string this_input = std::string(argv[1]);
  min_table_temp_C = parse_double(this_input);

  if (std::isnan(min_table_temp_C))
  {
    std::printf(u8"Input Error: could not parse the lowest table temperature\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (min_table_temp_C < -273.15)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return 0;
  }
  else if (min_table_temp_C < std::numeric_limits<int16_t>::lowest() / 128.0)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() / 128.0);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }
  else if (min_table_temp_C > std::numeric_limits<int16_t>::max() / 128.0)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() / 128.0);
    std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }

  /**
   * Parse the highest table temperature input
   */
  this_input = std::string(argv[2]);
  max_table_temp_C = parse_double(this_input);

  if (std::isnan(max_table_temp_C))
  {
    std::printf(u8"Input Error: could not parse the highest table temperature\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (max_table_temp_C < -273.15)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return 0;
  }
  else if (max_table_temp_C < std::numeric_limits<int16_t>::lowest() / 128.0)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() / 128.0);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }
  else if (max_table_temp_C > std::numeric_limits<int16_t>::max() / 128.0)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() / 128.0);
    std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }

  if (max_table_temp_C < min_table_temp_C)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             must be greater than the lowest table\n");
    std::printf(u8"             temperature value.\n\n");
    return 0;
  }

  /**
   * Parse NTC thermistor nominal resistance input
   */
  this_input = std::string(argv[3]);
  Rntc_nom_Ohms = parse_resistance(this_input);

  if (std::isnan(Rntc_nom_Ohms))
  {
    std::printf(u8"Input Error: could not parse the NTC nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rntc_nom_Ohms < 1.0)
  {
    std::printf(u8"Input Error: the NTC nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <1\u03A9.\n\n");
    return 0;
  }
  else if (Rntc_nom_Ohms > 100.0e6)
  {
    std::printf(u8"Input Error: the NTC nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100M\u03A9.\n\n");
    return 0;
  }

  /**
   * Parse the temperature of NTC nominal resistance input.
   */
  this_input = std::string(argv[4]);
  NTC_nom_temp_C = parse_double(this_input);

  if (std::isnan(NTC_nom_temp_C))
  {
    std::printf(u8"Input Error: could not parse the temperature for the NTC\n");
    std::printf(u8"             nominal resistance, \"%s\".\n\n", this_input.c_str());    
    return 0;
  }
  else if (NTC_nom_temp_C < -273.15)
  {
    std::printf(u8"Input Error: the temperature for the NTC nominal resistance\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return 0;
  }
  else if (NTC_nom_temp_C >= 2054.0)
  {
    std::printf(u8"Input Error: the temperature for the NTC nominal resistance\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be \u22652054\u00B0C. The common\n");
    std::printf(u8"             resistor substrate Alumina melts @ 2054\u00B0C.\n\n");
    return 0;
  }

  /**
   * Parse NTC nominal Beta coefficient input.
   */
  this_input = std::string(argv[5]);
  beta_K = parse_double(this_input);

  if (std::isnan(beta_K))
  {
    std::printf(u8"Input Error: could not parse the NTC nominal \u03B2 coefficient\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (beta_K < 100.0)
  {
    std::printf(u8"Input Error: the NTC nominal \u03B2 coefficient value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <100K.\n\n");
    return 0;
  }
  else if (beta_K > 100000.0)
  {
    std::printf(u8"Input Error: the NTC nominal \u03B2 coefficient value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100,000K.\n\n");
    return 0;
  }

  /**
   * Parse the pullup resistance input.
   */
  this_input = std::string(argv[6]);
  Rpullup_nom_Ohms = parse_resistance(this_input);

  if (std::isnan(Rpullup_nom_Ohms))
  {
    std::printf(u8"Input Error: could not parse the pullup resistor nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Rpullup_nom_Ohms < 1.0)
  {
    std::printf(u8"Input Error: the pullup resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <1\u03A9.\n\n");
    return 0;
  }
  else if (Rpullup_nom_Ohms > 100.0e6)
  {
    std::printf(u8"Input Error: the pullup resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100M\u03A9.\n\n");
    return 0;
  }

  /**
   * Parse the isolation resistance input.
   */
  this_input = std::string(argv[7]);
  Riso_nom_Ohms = parse_resistance(this_input);

  if (std::isnan(Riso_nom_Ohms))
  {
    std::printf(u8"Input Error: could not parse the isolation resistor nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (Riso_nom_Ohms < 1.0)
  {
    std::printf(u8"Input Error: the isolation resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <1\u03A9.\n\n");
    return 0;
  }
  else if (Riso_nom_Ohms > 100.0e6)
  {
    std::printf(u8"Input Error: the isolation resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >100M\u03A9.\n\n");
    return 0;
  }

  /**
   * Parse the number of ADC counts input.
   */
  this_input = std::string(argv[8]);
  bool parse_ok = false;
  int64_t tmp_ADC_counts = parse_int64(this_input, parse_ok);

  if (!parse_ok)
  {
    std::printf(u8"Input Error: could not parse the ADC number of counts\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (tmp_ADC_counts < 8ll)
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <8.\n\n");
    return 0;
  }
  else if (tmp_ADC_counts > std::numeric_limits<uint16_t>::max())
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%u.\n\n", std::numeric_limits<uint16_t>::max());
    return 0;
  }

  ADC_counts = static_cast<uint16_t>(tmp_ADC_counts);

  /**
   * Parse the maximum interpolation error input.
   */
  this_input = std::string(argv[9]);
  max_interp_error_C = parse_double(this_input);

  if (std::isnan(max_interp_error_C))
  {
    std::printf(u8"Input Error: could not parse the maximum interpolation error\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return 0;
  }
  else if (max_interp_error_C < 1.0/256.0)
  {
    std::printf(u8"Input Error: the maximum interpolation error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.10f, which is half\n", 1.0/256.0);
    std::printf(u8"             the value of one least significant bit in the\n");
    std::printf(u8"             underlying fixed point representation\n\n");
    return 0;
  }
  else if (max_interp_error_C >= std::max(std::fabs(min_table_temp_C), std::fabs(max_table_temp_C)))
  {
    std::printf(u8"Input Error: the maximum interpolation error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be larger than the absolute value\n");
    std::printf(u8"             of the minimum or maximum table temperature.\n\n");
    return 0;
  }

  /**
   * Do a little more validation of lowest and highest table temperatures.
   */
  double min_Rntc = Rntc_from_Tntc(max_table_temp_C);
  if (min_Rntc <= 0.0)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             and the NTC parameters result in an\n");
    std::printf(u8"             NTC resistance that is \u22640\u03A9.\n");
    return 0;
  }
  uint16_t table_start_count = ADCread_from_Tntc(max_table_temp_C);
  double real_max_table_temp = Tntc_from_ADCread(table_start_count);
  if (real_max_table_temp < max_table_temp_C)
  {
    table_start_count--;
    min_Rntc = Rntc_from_ADCread(table_start_count);
    if (min_Rntc <= 0.0)
    {
      std::printf(u8"Input Error: the highest table temperature value\n");
      std::printf(u8"             and the NTC parameters result in an\n");
      std::printf(u8"             NTC resistance that is \u22640\u03A9.\n");
      return 0;
    }
    real_max_table_temp = Tntc_from_ADCread(table_start_count);
    min_Rntc = Rntc_from_Tntc(real_max_table_temp);
    if (min_Rntc <= 0.0)
    {
      std::printf(u8"Input Error: the highest table temperature value\n");
      std::printf(u8"             and the NTC parameters result in an\n");
      std::printf(u8"             NTC resistance that is \u22640\u03A9.\n");
      return 0;
    }
  }

  if (real_max_table_temp > std::numeric_limits<int16_t>::max() / 128.0)
  {
    std::printf(u8"Input Error: the nearest ADC count that encompasses\n");
    std::printf(u8"             the highest table temperature value results\n");
    std::printf(u8"             in an actual highest table temperature that\n");
    std::printf(u8"             exceeds %.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() / 128.0);
    std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }

  uint16_t table_end_count = ADCread_from_Tntc(min_table_temp_C);
  double real_min_table_temp = Tntc_from_ADCread(table_end_count);
  if (table_end_count < ADC_counts - 1u && real_min_table_temp > min_table_temp_C)
  {
    table_end_count++;
    real_min_table_temp = Tntc_from_ADCread(table_end_count);
  }

  if (real_min_table_temp < std::numeric_limits<int16_t>::lowest() / 128.0)
  {
    std::printf(u8"Input Error: the nearest ADC count that encompasses\n");
    std::printf(u8"             the lowest table temperature value results\n");
    std::printf(u8"             in an actual lowest table temperature less\n");
    std::printf(u8"             than %.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() / 128.0);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }

  if (real_min_table_temp - min_table_temp_C > 1.0 / 128.0)
  {
    std::printf(u8"WARNING: The interpolation table actual minimum temperature\n");
    std::printf(u8"         is %.10f\u00B0C, which is less than\n", real_min_table_temp);
    std::printf(u8"         the requested %.10f\u00B0C because\n", min_table_temp_C);
    std::printf(u8"         the thermistor circuit output voltage has reached\n");
    std::printf(u8"         the limit of the ADC.\n\n");
  }

  /**
   * Redisplay input arguments back to user.
   */
  std::printf(u8"[Inputs]\n");
  std::printf(u8"NTC: %.1f\u03A9 @ %.1f\u00B0C, \u03B2 = %.0fK\n", Rntc_nom_Ohms, NTC_nom_temp_C, beta_K);
  std::printf(u8"Pullup Resistor: %.1f\u03A9\n", Rpullup_nom_Ohms);
  std::printf(u8"Isolation Resistor:  %.1f\u03A9\n", Riso_nom_Ohms);
  std::printf(u8"ADC: %u counts\n", ADC_counts);
  std::printf(u8"Table temperatures: lowest = %.8f\u00B0C, highest = %.8f\u00B0C\n", real_min_table_temp, real_max_table_temp);
  std::printf(u8"Max interpolation error: %.6f\u00B0C\n\n", max_interp_error_C);

  std::printf("table start: ADC count %5u\n", table_start_count);
  std::printf("table end:   ADC count %5u\n", table_end_count);

  /**
   * Solve for interpolation segments.
   * 
   * Basic strategy:
   * Least squares fit each interpolation segment to a wider and wider
   * section of data points until the maximum error of the interpolation
   * segment exceeds the maximum interpolation error.
   * Then save the segment with one less data point than the first segment
   * that exceeded the maximum interpolation error.
   */

  /**
   * Storage for segments & segment stats.
   */
  std::vector<interp_segment_t> vInterp_segments;
  std::vector<segment_stats_t> vSegment_stats;
  uint16_t next_start_count = table_start_count;

  while (true)
  {
    /**
     * Start with a single point interpolation "segment"
     * in case there is only one ADC count left at the end
     * of the table.
     */
    interp_segment_t kept_segment;
    kept_segment.start_count = next_start_count;
    kept_segment.start_temp = fixed_point_C(Tntc_from_ADCread(kept_segment.start_count));
    kept_segment.slope_multiplier = 0;
    kept_segment.slope_shift = 0u;
    double kept_mean_err = std::fabs(kept_segment.start_temp / 128.0 - Tntc_from_ADCread(kept_segment.start_count));
    double kept_max_err = kept_mean_err;
    uint16_t nPoints;

    for (nPoints = 2u; next_start_count + nPoints - 1u <= table_end_count; nPoints++)
    {
      /**
       * Use least squares to find the best fit line segment
       * to the data on [next_start_count, next_start_count + nPoints -1].
       * Store the result in test_segment.
       */
      interp_segment_t test_segment;
      test_segment = kept_segment;

      Eigen::Matrix<double, Eigen::Dynamic, 2> regressor(nPoints, 2);
      Eigen::Matrix<double, Eigen::Dynamic, 1> dataVector(nPoints);
      /**
       * Line segment parameters are in offset + slope form.
       * Offset is parameters(0).
       * Slope is parameters(1).
       */
      Eigen::Matrix<double, 2, 1> parameters;

      for (uint16_t jPoint = 0u; jPoint < nPoints; jPoint++)
      {
        /**
         * The independent variable for this line segment is
         * ADC counts, with segment.start_count as the 0 point.
         */
        regressor(jPoint,0) = 1.0;
        regressor(jPoint,1) = static_cast<double>(jPoint);

        /**
         * The dependent variable is thermistor temperature
         * in 1/128ths of a degree Celsius, so the slope will
         * be in 1/128ths of a degree Celsius per ADC count.
         */
        dataVector(jPoint) = 128.0 * Tntc_from_ADCread(next_start_count + jPoint);
      }

      /**
       * Jacobi SVD (non bidiagonalizing) with full pivot QR preconditioning
       * is the most numerically stable least squares solver in the Eigen
       * library.
       */
      Eigen::JacobiSVD<Eigen::Matrix<double, Eigen::Dynamic, 2>, Eigen::FullPivHouseholderQRPreconditioner> svd(regressor, Eigen::ComputeFullU | Eigen::ComputeFullV);
      parameters = svd.solve(dataVector);

      /**
       * Store the segment offset.
       */
      test_segment.start_temp = static_cast<int16_t>(std::round(parameters(0)));

      /**
       * Translate parameters(1), the slope, into a rational number
       * with base-2 denominator and use the value of that rational
       * for error calculation.
       * This way the error calculation includes errors in the fixed
       * point representation of the interpolation table.
       */
      bool found_rational = false;
      double frac_slope = parameters(1);
      test_segment.slope_shift = 0u;
      test_segment.slope_multiplier = 0;

      for (/* nothing */; test_segment.slope_shift < 30u; test_segment.slope_shift++)
      {
        double two_exp = double(1ull << test_segment.slope_shift);
        test_segment.slope_multiplier = static_cast<int32_t>(std::round(frac_slope * two_exp));
        double max_product = static_cast<double>(nPoints) * test_segment.slope_multiplier;
        double approx_max = max_product / two_exp;
        double actual_max = static_cast<double>(nPoints) * frac_slope;

        /**
         * Prevent overflow in the product test_segment.slope_multiplier * nPoints.
         */
        if (max_product > static_cast<double>(std::numeric_limits<int32_t>::max()) ||
            max_product < static_cast<double>(std::numeric_limits<int32_t>::min())) break;

        if (std::fabs(std::fabs(approx_max) - std::fabs(actual_max)) < 0.5)
        {
          found_rational = true;
          break;
        }
      }

      if (!found_rational)
      {
        std::printf("Error: could not find int32_t base-2 rational approximation to %f\n", frac_slope);
        return 0;
      }

      /**
       * Calculate the maximum interpolation error for this segment.
       */
      bool keepSegment = true;
      double mean_error = 0.0;
      double max_error = 0.0;

      for (uint16_t jPoint = 0u; jPoint < nPoints; jPoint++)
      {
        int16_t this_temp = test_segment.start_temp + multshiftround<int32_t>(jPoint, test_segment.slope_multiplier, test_segment.slope_shift);
        double this_abs_error = std::fabs(dataVector(jPoint) / 128.0 - static_cast<double>(this_temp) / 128.0);
        mean_error += this_abs_error;
        max_error = std::max(max_error, this_abs_error);
        if (this_abs_error > max_interp_error_C) keepSegment = false;
      }

      mean_error /= static_cast<double>(nPoints);

      /**
       * Store segment statistics.
       */
      if (keepSegment) 
      {
        kept_segment = test_segment;
        kept_mean_err = mean_error;
        kept_max_err = max_error;
      }
      else break;
    }

    /**
     * Adjust for the last increment in the for loop
     * or for the for loop assignment if a single point
     * "segment".
     */
    nPoints--;

    /**
     * Store the segment.
     */
    vInterp_segments.push_back(kept_segment);
    vSegment_stats.push_back({nPoints, kept_mean_err, kept_max_err});
    if (kept_segment.start_count + nPoints - 1u == table_end_count) break;
    next_start_count = kept_segment.start_count + nPoints;
  }

  /**
   * Print all the segment info, expanding each fixed point value
   * into recognizable units.
   */
  for (size_t jSegment = 0ull; jSegment < vInterp_segments.size(); jSegment++)
  {
    interp_segment_t this_segment = vInterp_segments.at(jSegment);
    std::printf(u8"segment %3" PRIu64 ":  start ADC count = %5u,  offset = % 7i = % 12.6f \u00B0C,  slope = % 6i / 2^(% 3i) = % 12.6f (1/128)\u00B0C / ADC count.\n",
                jSegment, this_segment.start_count, this_segment.start_temp,
                static_cast<double>(this_segment.start_temp) / 128.0,
                this_segment.slope_multiplier, this_segment.slope_shift,
                static_cast<double>(this_segment.slope_multiplier) / static_cast<double>(1ull << this_segment.slope_shift));
  }
  std::printf("\n");

  /**
   * Print segment statistics, largely to make me feel good.
   * (statistics make me feel good)
   */
  for (size_t jSegment = 0ull; jSegment < vSegment_stats.size(); jSegment++)
  {
    segment_stats_t these_stats = vSegment_stats.at(jSegment);
    std::printf(u8"segment %3" PRIu64 " stats:  # points = %4u,  mean error = % 9.6f \u00B0C,  max error = % 9.6f \u00B0C\n",
                jSegment, these_stats.num_points, these_stats.mean_error, these_stats.max_error);
  }
  std::printf("\n");

  /**
   * Print the struct definitions. I consider these
   * to be an important part of the table code.
   */
  std::printf("/" "**\n");
  std::printf(" * interp_segment_t defines a single linear interpolation\n");
  std::printf(" *                  segment.\n");
  std::printf(" *\n");
  std::printf(" * start_count: the ADC count value corresponding to\n");
  std::printf(" *              start_temp\n");
  std::printf(" *\n");
  std::printf(" * start_temp: the temperature corresponding to start_count\n");
  std::printf(" *             in 1/128ths of a degree Celsius.\n");
  std::printf(" *             This is signed Q9.7 format fixed point.\n");
  std::printf(" *\n");
  std::printf(" * slope_multiplier: these two define the slope of the\n");
  std::printf(" * slope_shift:      line segment as the rational number\n");
  std::printf(" *                   (slope_multiplier / 2^slope_shift).\n");
  std::printf(" *                   Units are 1/128ths of a degree Celsius\n");
  std::printf(" *                   per ADC count.\n");
  std::printf(" *\n");
  std::printf(" * Each segment ends one count before the start of the\n");
  std::printf(" * next segment. end_count in interp_table_t gives the last\n");
  std::printf(" * valid ADC count for the final segment.\n");
  std::printf(" *" "/\n");
  std::printf("typedef struct\n");
  std::printf("{\n");
  std::printf("  uint16_t start_count;\n");
  std::printf("  int16_t start_temp;\n");
  std::printf("  int32_t slope_multiplier;\n");
  std::printf("  uint8_t slope_shift;\n");
  std::printf("} interp_segment_t;\n\n");

  std::printf("/" "**\n");
  std::printf(" * Converts a raw ADC reading of the thermistor circuit\n");
  std::printf(" * into a temperature in 1/128ths of a degree Celsius.\n");
  std::printf(" *\n");
  std::printf(" * This code was autogenerated with the following parameters:\n");
  std::printf(" * NTC Thermistor: %.1f Ohms nominal @ %.1f deg. C.\n", Rntc_nom_Ohms, NTC_nom_temp_C);
  std::printf(" *                 Beta = %.0f K\n", beta_K);
  std::printf(" * Pullup resistor: %.1f Ohms nominal.\n", Rpullup_nom_Ohms);
  std::printf(" *   - The pullup resistor connects between the NTC and the\n");
  std::printf(" *     positive voltage supply.\n");
  std::printf(" * Isolation resistor: %.1f Ohms nominal\n", Riso_nom_Ohms);
  std::printf(" *   - The isolation resistor connects between the NTC and GND.\n");
  std::printf(" * Full ADC count range: 0-%u\n", ADC_counts - 1u);
  std::printf(" * Max interpolation error: %.8f deg. C\n", max_interp_error_C);
  std::printf(" * Table range: %.8f to %.8f deg. C\n", real_min_table_temp, real_max_table_temp);
  std::printf(" * ADCcount inputs >= %u result in the minimum table temperature.\n", table_end_count);
  std::printf(" * ADCcount inputs <= %u result in the maximum table temperature.\n", vInterp_segments.at(0).start_count);
  std::printf(" *" "/\n");
  std::printf("int16_t read_thermistor(const uint16_t ADCcount)\n");
  std::printf("{\n");
  std::printf("  static const uint16_t num_segments = %" PRIu64 "u;\n", vInterp_segments.size());
  std::printf("  static const interp_segment_t interp_segments[num_segments] = {\n");
  for (size_t jSegment = 0ull; jSegment + 1ull < vInterp_segments.size(); jSegment++)
  {
    interp_segment_t this_segment = vInterp_segments.at(jSegment);
    std::printf("    {%5u, % 6i, % 6i, %2u},\n", this_segment.start_count, this_segment.start_temp, this_segment.slope_multiplier, this_segment.slope_shift);
  }
  
  if (vInterp_segments.size() > 0ull)
  {
    interp_segment_t this_segment = vInterp_segments.back();
    std::printf("    {%5u, % 6i, % 6i, %2u}\n", this_segment.start_count, this_segment.start_temp, this_segment.slope_multiplier, this_segment.slope_shift);
  }

  std::printf("  };\n");
  std::printf("  static const uint16_t last_segment_end_count = %u;\n", table_end_count);
  std::printf("\n");
  std::printf("  /" "**\n");
  std::printf("   * Check input ADCcount against table min & max ADC counts.\n");
  std::printf("   *" "/\n");
  std::printf("  if (ADCcount <= interp_segments[0].start_count)\n");
  std::printf("  {\n");
  std::printf("    return interp_segments[0].start_temp;\n");
  std::printf("  }\n");
  std::printf("\n");
  std::printf("  uint16_t seg_index = 0u;\n");
  std::printf("\n");
  std::printf("  if (ADCcount >= last_segment_end_count)\n");
  std::printf("  {\n");
  std::printf("    seg_index = num_segments - 1u;\n");
  std::printf("    return interp_segments[seg_index].start_temp +\n");
  std::printf("           multshiftround<int32_t>(last_segment_end_count - interp_segments[seg_index].start_count,\n");
  std::printf("                                   interp_segments[seg_index].slope_multiplier,\n");
  std::printf("                                   interp_segments[seg_index].slope_shift);\n");
  std::printf("  }\n");
  std::printf("\n");
  std::printf("  /" "**\n");
  std::printf("   * Find the interpolation segment that contains ADCcount\n");
  std::printf("   * via binary search.\n");
  std::printf("   *" "/\n");
  std::printf("  uint16_t lower_bound = 0u;\n");
  std::printf("  uint16_t upper_bound = num_segments - 1u;\n");
  std::printf("  seg_index = (lower_bound + upper_bound) >> 1;\n");
  std::printf("\n");
  std::printf("  while (true)\n");
  std::printf("  {\n");
  std::printf("    if (ADCcount < interp_segments[seg_index].start_count)\n");
  std::printf("    {\n");
  std::printf("      upper_bound = seg_index - 1u;\n");
  std::printf("      seg_index = (lower_bound + upper_bound) >> 1;\n");
  std::printf("    }\n");
  std::printf("    else if (seg_index + 1u < num_segments &&\n");
  std::printf("             ADCcount >= interp_segments[seg_index + 1u].start_count)\n");
  std::printf("    {\n");
  std::printf("      lower_bound = seg_index + 1u;\n");
  std::printf("      seg_index = (lower_bound + upper_bound) >> 1;\n");
  std::printf("    }\n");
  std::printf("    else\n");
  std::printf("    {\n");
  std::printf("      return interp_segments[seg_index].start_temp +\n");
  std::printf("             multshiftround<int32_t>(ADCcount - interp_segments[seg_index].start_count,\n");
  std::printf("                                     interp_segments[seg_index].slope_multiplier,\n");
  std::printf("                                     interp_segments[seg_index].slope_shift);\n");
  std::printf("    }\n");
  std::printf("  }\n");
  std::printf("}\n\n");

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
