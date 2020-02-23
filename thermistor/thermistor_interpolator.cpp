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
 * Originally distributed at https://github.com/slugrustle/numerical_routines
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
#include <limits>
#include <string>
#include <algorithm>
#include "multshiftround_run.hpp"
#include "globals.h"
#include "types.h"
#include "parsers.h"
#include "NTCcalculations.h"
#include "QRleast_squares.h"

/**
 * Defining variables declared in globals.h
 * These are also shared with NTCcalculations.cpp
 */
double Rntc_nom_Ohms;
double beta_K;
double Rpullup_nom_Ohms;
double Riso_nom_Ohms;
uint16_t ADC_counts;
double inv_Rntc_nom_Ohms;
double inv_NTC_nom_temp_K;
double inv_beta_K;
double inv_ADC_counts_minus_one;

/**
 * Input or calculation variables used in main.
 */
static double NTC_nom_temp_C;
static double min_table_temp_C;
static double max_table_temp_C;
static double max_interp_error_C;
static const double inv_128 = 1.0 / 128.0;

/**
 * The ADC_counts input is limited to 2^15 = 32768.
 * Max size the storage for the least squares problem
 * and use subsets of this array during computation.
 */
static least_squares_row_t least_squares_data[32768];

/**
 * Instead of calling Tntc_from_ADCread() on the same input
 * variables all the time, compute it for all ADC inputs in
 * the range of the table once and store those results in
 * Tntc_array.
 * 
 * Not all indices will be used.
 */
static double Tntc_array[32768];

/**
 * storage for the table's linear interpolation segments
 * and for statistics data about those segments.
 */
static interp_segment_t interp_segments[32768];
static uint16_t n_stored_segments;
static segment_stats_t segment_stats[32768];

/**
 * Main routine of thermistor_interpolator.
 * 1. Displays usage message
 * 2. Parses & validates user input
 * 3. Computes near-optimal thermistor table
 * 4. Prints table, related data structures, and code
 *    that performs lookups on the table
 */
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
  else if (min_table_temp_C < -kelvin_offset)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return 0;
  }
  else if (min_table_temp_C < std::numeric_limits<int16_t>::lowest() * inv_128)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() * inv_128);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }
  else if (min_table_temp_C > std::numeric_limits<int16_t>::max() * inv_128)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() * inv_128);
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
  else if (max_table_temp_C < -kelvin_offset)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return 0;
  }
  else if (max_table_temp_C < std::numeric_limits<int16_t>::lowest() * inv_128)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() * inv_128);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }
  else if (max_table_temp_C > std::numeric_limits<int16_t>::max() * inv_128)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() * inv_128);
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

  inv_Rntc_nom_Ohms = 1.0 / Rntc_nom_Ohms;

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
  else if (NTC_nom_temp_C < -kelvin_offset)
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

  inv_NTC_nom_temp_K = 1.0 / (NTC_nom_temp_C + kelvin_offset);

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

  inv_beta_K = 1.0 / beta_K;

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
  else if (Riso_nom_Ohms < 0.0)
  {
    std::printf(u8"Input Error: the isolation resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <0\u03A9.\n\n");
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
  else if (tmp_ADC_counts > (1ll << 15))
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%u.\n\n", 1u << 15);
    return 0;
  }

  ADC_counts = static_cast<uint16_t>(tmp_ADC_counts);
  inv_ADC_counts_minus_one = 1.0 / static_cast<double>(ADC_counts - 1u);

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

  if (real_max_table_temp > std::numeric_limits<int16_t>::max() * inv_128)
  {
    std::printf(u8"Input Error: the nearest ADC count that encompasses\n");
    std::printf(u8"             the highest table temperature value results\n");
    std::printf(u8"             in an actual highest table temperature that\n");
    std::printf(u8"             exceeds %.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() * inv_128);
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

  if (real_min_table_temp < std::numeric_limits<int16_t>::lowest() * inv_128)
  {
    std::printf(u8"Input Error: the nearest ADC count that encompasses\n");
    std::printf(u8"             the lowest table temperature value results\n");
    std::printf(u8"             in an actual lowest table temperature less\n");
    std::printf(u8"             than %.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() * inv_128);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return 0;
  }

  if (real_min_table_temp - min_table_temp_C > inv_128)
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
   * Fill in table of NTC temperature readings indexed by ADC count
   * for the whole table range.
   */
  for (uint16_t ADC_read = table_start_count; ADC_read <= table_end_count; ADC_read++)
  {
    Tntc_array[ADC_read] = Tntc_from_ADCread(ADC_read);
  }

  /**
   * Solve for interpolation segments.
   * 
   * Basic strategy:
   * Least squares fit each interpolation segment to a wider and wider
   * section of data points until the maximum error of the interpolation
   * segment exceeds the maximum interpolation error.
   * Then backtrack to find the segment with the most points that also
   * does not exceed the maximum interpolation error.
   */
  uint16_t next_start_count = table_start_count;
  n_stored_segments = 0u;

  while (true)
  {
    /**
     * Start with a single point interpolation "segment"
     * in case there is only one ADC count left at the end
     * of the table.
     */
    interp_segment_t kept_segment;
    kept_segment.start_count = next_start_count;
    kept_segment.start_temp = fixed_point_C(Tntc_array[kept_segment.start_count]);
    kept_segment.slope_multiplier = 0;
    kept_segment.slope_shift = 0u;
    double kept_mean_err = std::fabs(kept_segment.start_temp * inv_128 - Tntc_array[kept_segment.start_count]);
    double kept_max_err = kept_mean_err;
    uint16_t nPoints = 1u;
    uint16_t previous_nPoints = nPoints;
    uint16_t max_nPoints = table_end_count + static_cast<uint16_t>(1) - next_start_count;

    /**
     * A backtracking line search algorithm is used to find the largest
     * value of nPoints for which this segment does not exceed the maximum
     * interpolation error.
     * It starts by trying fixed multiples of nPoints for the next nPoints
     * if the error is low, then reverts to increasing nPoints by fixed
     * increments.
     * The multiplier or increment is decreased each time a segment exceeds
     * maximum interpolation error, until this occurs with an increment of 1.
     */
    bool trying_multiples = true;
    bool trying_increments = true;
    uint16_t last_multiple = 1u;
    uint16_t last_increment = 1u;
    

    if (2u <= max_nPoints)
    {
      nPoints = 2u;
    }

    while (nPoints >= 2u && nPoints <= max_nPoints)
    {
      /**
       * Use least squares to find the best fit line segment
       * to the data on [next_start_count, next_start_count + nPoints -1].
       * Store the result in test_segment.
       */
      interp_segment_t test_segment;
      test_segment = kept_segment;

      /**
       * Line segment parameters are in offset + slope form.
       * Offset is parameters(0).
       * Slope is parameters(1).
       */
      double parameters[2] = {0.0, 0.0};

      for (uint16_t jPoint = 0u; jPoint < nPoints; jPoint++)
      {
        /**
         * The independent variable for this line segment is offset
         * ADC counts, with segment.start_count as the 0 point.
         */
        least_squares_data[jPoint].columns[0] = 1.0;
        least_squares_data[jPoint].columns[1] = static_cast<double>(jPoint);

        /**
         * The dependent variable is thermistor temperature
         * in 1/128ths of a degree Celsius, so the slope will
         * be in 1/128ths of a degree Celsius per ADC count.
         */
        least_squares_data[jPoint].rhs = 128.0 * Tntc_array[next_start_count + jPoint];
      }

      QRleast_squares(least_squares_data, nPoints, parameters);

      /**
       * Store the segment offset.
       */
      test_segment.start_temp = static_cast<int16_t>(std::round(parameters[0]));

      /**
       * Translate parameters[1], the slope, into a rational number
       * with base-2 denominator and use the value of that rational
       * for error calculation.
       * This way the error calculation includes errors in the fixed
       * point representation of the interpolation table.
       */
      bool found_rational = false;
      double frac_slope = parameters[1];
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
        int16_t this_temp = static_cast<int16_t>(static_cast<int32_t>(test_segment.start_temp) + multshiftround<int32_t>(jPoint, test_segment.slope_multiplier, test_segment.slope_shift));
        double this_abs_error = std::fabs(Tntc_array[test_segment.start_count + jPoint] - static_cast<double>(this_temp) * inv_128);
        mean_error += this_abs_error;
        max_error = std::max(max_error, this_abs_error);
        if (this_abs_error > max_interp_error_C) keepSegment = false;
      }

      mean_error /= static_cast<double>(nPoints);

      /**
       * Figure out the next value for nPoints using a
       * backtracking line search.
       */
      if (keepSegment) 
      {
        /**
         * Store segment statistics.
         */
        kept_segment = test_segment;
        kept_mean_err = mean_error;
        kept_max_err = max_error;
        previous_nPoints = nPoints;

        if (nPoints == max_nPoints)
        {
          /**
           * No more points to interpolate.
           */
          break;
        }

        if (nPoints <= 2u)
        {
          /**
           * nPoints == 2 has a perfect fit by definition.
           * Wait until we have an actual error measure to
           * perform the backtracking line search.
           */
          nPoints++;
          last_multiple = 1u;
          last_increment = 1u;
        }
        else
        {
          /**
           * Multiply nPoints to get next nPoints only if the segment error
           * is significantly below the max interpolation error.
           */
          bool multiple_found = false;

          if (trying_multiples)
          {
            if (kept_max_err < 0.1 * max_interp_error_C)
            {
              last_multiple = 5u;
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints * last_multiple));
              last_increment = nPoints - previous_nPoints;
              multiple_found = true;
            }
            else if (kept_max_err < 0.5 * max_interp_error_C)
            {
              last_multiple = 2u;
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints * last_multiple));
              last_increment = nPoints - previous_nPoints;
              multiple_found = true;
            }
          }

          /**
           * Fall back to incrementing nPoints if we are not far enough
           * below max interpolation error to attempt multiplying nPoints.
           */
          if (trying_increments & !multiple_found)
          {
            trying_multiples = false;
            last_multiple = 1u;

            if (last_increment >= 500u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 500u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment >= 200u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 200u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment >= 100u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 100u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment >= 50u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 50u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment >= 20u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 20u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment >= 10u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 10u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment >= 5u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 5u));
              last_increment = nPoints - previous_nPoints;
            }
            else
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(nPoints + 2u));
              last_increment = nPoints - previous_nPoints;
            }
          }
          else if (!multiple_found)
          {
            trying_multiples = false;
            last_multiple = 1u;

            nPoints++;
            last_increment = 1u;
          }
        }
      }
      else if (trying_multiples)
      {
        /**
         * Segment exceeded max interpolation error when multiplying nPoints
         * to get next nPoints.
         * Try a smaller multiplier, or fall back to incrementing nPoints
         * by an appropriate amount.
         */
        if (last_multiple > 2u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints * 2u));
          last_multiple = 2u;
        }
        else
        {
          trying_multiples = false;
          last_multiple = 1u;

          if (trying_increments)
          {
            if (last_increment > 500u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 500u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 200u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 200u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 100u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 100u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 50u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 50u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 20u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 20u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 10u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 10u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 5u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 5u));
              last_increment = nPoints - previous_nPoints;
            }
            else if (last_increment > 2u)
            {
              nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 2u));
              last_increment = nPoints - previous_nPoints;
            }
            else
            {
              trying_increments = false;
              last_increment = 1u;
              nPoints = previous_nPoints + last_increment;
            }
          }
          else
          {
            last_increment = 1u;
            nPoints = previous_nPoints + last_increment;
          }
        }
      }
      else if (trying_increments)
      {
        /**
         * Segment exceeded max interpolation error when incrementing
         * nPoints to get next nPoints.
         * Try a smaller increment value, down to 1 if necessary.
         */
        last_multiple = 1u;

        if (last_increment >= 500u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 200u));
          last_increment = nPoints - previous_nPoints;
        }
        else if (last_increment >= 200u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 100u));
          last_increment = nPoints - previous_nPoints;
        }
        else if (last_increment >= 100u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 50u));
          last_increment = nPoints - previous_nPoints;
        }
        else if (last_increment >= 50u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 20u));
          last_increment = nPoints - previous_nPoints;
        }
        else if (last_increment >= 20u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 10u));
          last_increment = nPoints - previous_nPoints;
        }
        else if (last_increment >= 10u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 5u));
          last_increment = nPoints - previous_nPoints;
        }
        else if (last_increment >= 5u)
        {
          nPoints = std::min(max_nPoints, static_cast<uint16_t>(previous_nPoints + 2u));
          last_increment = nPoints - previous_nPoints;
        }
        else
        {
          trying_increments = false;
          last_increment = 1u;
          nPoints = previous_nPoints + last_increment;
        }
      }
      else
      {
        /**
         * Segment exceeded max interpolation error when incrementing nPoints by 1.
         * We're done.
         */
        break;
      }
    }

    /**
     * Store the last kept segment.
     */
    interp_segments[n_stored_segments] = kept_segment;
    segment_stats[n_stored_segments] = {previous_nPoints, kept_mean_err, kept_max_err};
    n_stored_segments++;
    if (kept_segment.start_count + previous_nPoints - 1u == table_end_count) break;
    next_start_count = kept_segment.start_count + previous_nPoints;
  }

  /**
   * Print all the segment info, expanding each fixed point value
   * into recognizable units.
   */
  for (uint16_t jSegment = 0u; jSegment < n_stored_segments; jSegment++)
  {
    interp_segment_t this_segment = interp_segments[jSegment];
    std::printf(u8"segment %3u:  start ADC count = %5u,  offset = % 7i = % 12.6f \u00B0C,  slope = % 6i / 2^(% 3i) = % 12.6f (1/128)\u00B0C / ADC count.\n",
                jSegment, this_segment.start_count, this_segment.start_temp,
                static_cast<double>(this_segment.start_temp) * inv_128,
                this_segment.slope_multiplier, this_segment.slope_shift,
                static_cast<double>(this_segment.slope_multiplier) / static_cast<double>(1ull << this_segment.slope_shift));
  }
  std::printf("\n");

  /**
   * Print segment statistics, largely to make me feel good.
   * (statistics make me feel good)
   */
  for (uint16_t jSegment = 0u; jSegment < n_stored_segments; jSegment++)
  {
    segment_stats_t these_stats = segment_stats[jSegment];
    std::printf(u8"segment %3u stats:  # points = %4u,  mean error = % 9.6f \u00B0C,  max error = % 9.6f \u00B0C\n",
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
  std::printf(" * ADCcount inputs <= %u result in the maximum table temperature.\n", interp_segments[0].start_count);
  std::printf(" *" "/\n");
  std::printf("int16_t read_thermistor(const uint16_t ADCcount)\n");
  std::printf("{\n");
  std::printf("  static const uint16_t num_segments = %uu;\n", n_stored_segments);
  std::printf("  static const interp_segment_t interp_segments[num_segments] = {\n");
  for (uint16_t jSegment = 0u; jSegment + 1u < n_stored_segments; jSegment++)
  {
    interp_segment_t this_segment = interp_segments[jSegment];
    std::printf("    {%5u, % 6i, % 6i, %2u},\n", this_segment.start_count, this_segment.start_temp, this_segment.slope_multiplier, this_segment.slope_shift);
  }
  
  if (n_stored_segments > 0u)
  {
    interp_segment_t this_segment = interp_segments[n_stored_segments-1u];
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
