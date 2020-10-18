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
#include "constants.h"
#include "types.h"
#include "parsers.h"
#include "NTCcalculations.h"
#include "QRleast_squares.h"
#include "steffen_interpolate.h"

/**
 * In-memory storage for a user-supplied table that gives
 * NTC thermistor resistance in Ohms at various temperatures
 * in degrees Celsius.
 */
static NTC_temp_res_row_t NTC_temp_res_data[MAX_CSV_ROWS];

/**
 * Function to enable sorting NTC_temp_res_data by temperature
 * ascending.
 */
static bool NTC_temp_res_data_sort(const NTC_temp_res_row_t &a, const NTC_temp_res_row_t &b)
{
  return a.temp_C < b.temp_C;
}

/**
 * Table of cubic interpolants of NTC_temp_res_data.
 */
static cubic_interp_seg_t cubic_interp_segs[MAX_CSV_ROWS - 1u];

/**
 * The ADC_counts input is limited to 2^15 = 32768.
 * Max size the storage for the least squares problem
 * and use subsets of this array during computation.
 */
static least_squares_row_t least_squares_data[MAX_ADC_COUNTS];

/**
 * Instead of calling Tntc_from_ADCread() on the same input
 * variables all the time, compute it for all ADC inputs in
 * the range of the table once and store those results in
 * Tntc_array.
 * 
 * Not all indices will be used.
 */
static double Tntc_array[MAX_ADC_COUNTS];

/**
 * storage for the table's linear interpolation segments
 * and for statistics data about those segments.
 */
static interp_segment_t interp_segments[MAX_ADC_COUNTS];
static uint16_t n_stored_segments;
static segment_stats_t segment_stats[MAX_ADC_COUNTS];

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
  std::printf(u8"vs. ADC counts curve for an NTC thermistor such that the maximum\n");
  std::printf(u8"interpolation error is below some bound.\n");
  std::printf(u8"Effort is made to minimize the number of interpolating segments\n");
  std::printf(u8"required to acheive this.\n\n");

  std::printf(u8"If special characters (\u00B0, \u03A9, \u00B1, etc.) do not display,\n");
  std::printf(u8"set your console to unicode (PowerShell> chcp 65001).\n\n");

  std::printf("There are two operating modes:\n");
  std::printf("  Mode 1: NTC Thermistor is specified by nominal resistance and\n");
  std::printf("          \u03B2 coefficient.\n");
  std::printf("  Mode 2: NTC Thermistor is specified by a .csv file containing\n");
  std::printf("          a table of \u00B0C temperatures (column 1) and NTC\n");
  std::printf("          resistances (in \u03A9, column 2). The .csv file must not\n");
  std::printf("          contain other data or header rows.\n\n\n");

  std::printf(u8"[Mode 1 Input Arguments]\n");
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

  std::printf(u8"[Mode 1 Example Command]\n");
  std::printf(u8".\\thermistor_interpolator.exe -30 90 33k 25 3950 22.1k 1.3k 4096 0.1\n\n\n");

  std::printf(u8"[Mode 2 Input Arguments]\n");
  std::printf(u8"1. Lowest table temperature (\u00B0C), within .csv data bounds.\n");
  std::printf(u8"2. Highest table temperature (\u00B0C), within .csv data bounds.\n");
  std::printf(u8"3. Filename of .csv file with NTC temperatures (\u00B0C) in column 1\n");
  std::printf(u8"   and NTC resistances (\u03A9) in column 2 and no header rows.\n");
  std::printf(u8"4. Pullup resistor nominal resistance (\u03A9)\n");
  std::printf(u8"   - The pullup resistor connects between the NTC\n");
  std::printf(u8"     and the positive voltage supply.\n");
  std::printf(u8"5. Isolation resistor nominal resistance (\u03A9)\n");
  std::printf(u8"   - The isolation resistor connects between the NTC\n");
  std::printf(u8"     and GND.\n");
  std::printf(u8"6. ADC number of counts (1024 for 10-bit, 4096 for 12-bit, etc.)\n");
  std::printf(u8"7. Maximum error due to interpolation (\u00B0C)\n\n");

  std::printf(u8"[Mode 2 Example Command]\n");
  std::printf(u8".\\thermistor_interpolator.exe -30 90 NTC_data.csv 22.1k 1.3k 4096 0.1\n\n\n");

  const uint8_t OPERATING_MODE = (argc == (NUM_ARGUMENTS_PARAMETER_MODE + 1) || argc == (NUM_ARGUMENTS_TABLE_MODE + 1)) ?
    (argc == (NUM_ARGUMENTS_PARAMETER_MODE + 1) ? NTC_PARAMETER_MODE : NTC_TABLE_MODE) : UNDEFINED_MODE;

  if (OPERATING_MODE != NTC_PARAMETER_MODE && OPERATING_MODE != NTC_TABLE_MODE)
  {
    std::printf(u8"Input Error: This program requires either %i or %i arguments.\n", NUM_ARGUMENTS_PARAMETER_MODE, NUM_ARGUMENTS_TABLE_MODE);
    std::printf(u8"             Please carefully read the tedious help text above.\n\n");
    return EXIT_FAILURE;
  }

  /* Used by both operating modes */
  const uint8_t MIN_TABLE_TEMP_ARGV_INDX = 1u;
  const uint8_t MAX_TABLE_TEMP_ARGV_INDX = 2u;
  
  /* Only used by NTC_PARAMETER_MODE */
  const uint8_t RNTC_NOM_ARGV_INDX = 3u;
  const uint8_t NTC_NOM_TEMP_ARGV_INDX = 4u;
  const uint8_t BETA_ARGV_INDX = 5u;

  /* Only used by NTC_TABLE_MODE */
  const uint8_t CSV_FILENAME_ARGV_INDX = 3u;

  /* Used by both operating modes */
  const uint8_t RPULLUP_NOM_ARGV_INDX = OPERATING_MODE == NTC_PARAMETER_MODE ? 6u : 4u;
  const uint8_t RISO_NOM_ARGV_INDX = OPERATING_MODE == NTC_PARAMETER_MODE ? 7u : 5u;
  const uint8_t ADC_COUNTS_ARGV_INDX = OPERATING_MODE == NTC_PARAMETER_MODE ? 8u : 6u;
  const uint8_t MAX_INTERP_ERROR_ARGV_INDX = OPERATING_MODE == NTC_PARAMETER_MODE ? 9u : 7u;

  /**
   * Parse the lowest table temperature input
   */
  std::string this_input = std::string(argv[MIN_TABLE_TEMP_ARGV_INDX]);
  double min_table_temp_C = parse_double(this_input);

  if (std::isnan(min_table_temp_C))
  {
    std::printf(u8"Input Error: could not parse the lowest table temperature\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return EXIT_FAILURE;
  }
  else if (min_table_temp_C < -KELVIN_OFFSET)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return EXIT_FAILURE;
  }
  else if (min_table_temp_C < MIN_FIXEDPOINTABLE_TEMP_C)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.8f\u00B0C.\n", MIN_FIXEDPOINTABLE_TEMP_C);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return EXIT_FAILURE;
  }
  else if (min_table_temp_C > MAX_FIXEDPOINTABLE_TEMP_C)
  {
    std::printf(u8"Input Error: the lowest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.8f\u00B0C.\n", MAX_FIXEDPOINTABLE_TEMP_C);
    std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return EXIT_FAILURE;
  }

  /**
   * Parse the highest table temperature input
   */
  this_input = std::string(argv[MAX_TABLE_TEMP_ARGV_INDX]);
  double max_table_temp_C = parse_double(this_input);

  if (std::isnan(max_table_temp_C))
  {
    std::printf(u8"Input Error: could not parse the highest table temperature\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return EXIT_FAILURE;
  }
  else if (max_table_temp_C < -KELVIN_OFFSET)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
    return EXIT_FAILURE;
  }
  else if (max_table_temp_C < MIN_FIXEDPOINTABLE_TEMP_C)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.8f\u00B0C.\n", MIN_FIXEDPOINTABLE_TEMP_C);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return EXIT_FAILURE;
  }
  else if (max_table_temp_C > MAX_FIXEDPOINTABLE_TEMP_C)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.8f\u00B0C.\n", MAX_FIXEDPOINTABLE_TEMP_C);
    std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return EXIT_FAILURE;
  }

  if (max_table_temp_C < min_table_temp_C)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             must be greater than the lowest table\n");
    std::printf(u8"             temperature value.\n\n");
    return EXIT_FAILURE;
  }

  double Rntc_nom_Ohms = std::numeric_limits<double>::quiet_NaN();
  std::string csv_filename;
  uint32_t csv_stored_rows = 0u;

  if (OPERATING_MODE == NTC_PARAMETER_MODE)
  {
    /**
     * Parse NTC thermistor nominal resistance input
     */
    this_input = std::string(argv[RNTC_NOM_ARGV_INDX]);
    Rntc_nom_Ohms = parse_resistance(this_input);

    if (std::isnan(Rntc_nom_Ohms))
    {
      std::printf(u8"Input Error: could not parse the NTC nominal resistance\n");
      std::printf(u8"             \"%s\".\n\n", this_input.c_str());
      return EXIT_FAILURE;
    }
    else if (Rntc_nom_Ohms < MIN_RNTC_NOM_OHMS)
    {
      std::printf(u8"Input Error: the NTC nominal resistance value\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be <%.0f\u03A9.\n\n", MIN_RNTC_NOM_OHMS);
      return EXIT_FAILURE;
    }
    else if (Rntc_nom_Ohms > MAX_RNTC_NOM_OHMS)
    {
      std::printf(u8"Input Error: the NTC nominal resistance value\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be >%.0fM\u03A9.\n\n", 1.0e-6 * MAX_RNTC_NOM_OHMS);
      return EXIT_FAILURE;
    }
  }
  else if (OPERATING_MODE == NTC_TABLE_MODE)
  {
    /**
     * Parse NTC temperature / resistance .csv filename input
     */
    csv_filename = std::string(argv[CSV_FILENAME_ARGV_INDX]);
    if (!parse_NTC_csv_file(csv_filename, NTC_temp_res_data, csv_stored_rows))
    {
      return EXIT_FAILURE;
    }

    if (csv_stored_rows < MIN_CSV_ROWS)
    {
      std::printf(u8"Input Error: found <%u valid rows in the input .csv file\n", MIN_CSV_ROWS);
      std::printf(u8"             %s.\n", csv_filename.c_str());
      std::printf(u8"             At least %u valid data rows are required for this\n", MIN_CSV_ROWS);
      std::printf(u8"             program to run.\n\n");
      return EXIT_FAILURE;
    }

    /* Sort the NTC temperature/resistance data by temperature ascending. */
    std::sort(NTC_temp_res_data, NTC_temp_res_data + csv_stored_rows, NTC_temp_res_data_sort);

    /**
     * Check the NTC temperature/resistance data.
     * Temperature should be strictly increasing and
     * resistance should be strictly decreasing.
     */
    NTC_temp_res_row_t prev_row = NTC_temp_res_data[0];
    for (uint32_t jRow = 1u; jRow < csv_stored_rows; jRow++)
    {
      NTC_temp_res_row_t this_row = NTC_temp_res_data[jRow];
      if (this_row.temp_C <= prev_row.temp_C)
      {
        std::printf(u8"Input Error: Temperature is not strictly increasing in the\n");
        std::printf(u8"             input .csv file %s\n", csv_filename.c_str());
        std::printf(u8"             even after sorting by temperature ascending.\n\n");
        return EXIT_FAILURE;
      }

      if (this_row.res_Ohms >= prev_row.res_Ohms)
      {
        std::printf(u8"Input Error: Resistance is not strictly decreasing in the\n");
        std::printf(u8"             input .csv file %s\n", csv_filename.c_str());
        std::printf(u8"             even after sorting by temperature ascending.\n\n");
        return EXIT_FAILURE;
      }

      prev_row = this_row;
    }

    if (min_table_temp_C < NTC_temp_res_data[0].temp_C)
    {
      std::printf(u8"Input Error: the lowest table temperature value\n");
      std::printf(u8"             must be greater than or equal to the minimum\n");
      std::printf(u8"             temperature in the .csv file.\n\n");
      return EXIT_FAILURE;
    }

    if (max_table_temp_C > NTC_temp_res_data[csv_stored_rows-1u].temp_C)
    {
      std::printf(u8"Input Error: the highest table temperature value\n");
      std::printf(u8"             must be less than or equal to the maximum\n");
      std::printf(u8"             temperature in the .csv file.\n\n");
      return EXIT_FAILURE;
    }

    /**
     * Interpolate the NTC temperature/resistance data via Steffen's
     * method and save the interpolant for use in the main algorithm.
     */
    if (!steffen_interpolate(NTC_temp_res_data, csv_stored_rows, cubic_interp_segs))
    {
      std::printf(u8"Error: Something went very wrong while attempting to\n");
      std::printf(u8"       interpolate the NTC thermistor temperature / resistance\n");
      std::printf(u8"       data found in %s.\n", csv_filename.c_str());
      std::printf(u8"       Please check your input data. Please file a bug report\n");
      std::printf(u8"       if the data looks ok.\n\n");
      return EXIT_FAILURE;
    }
  }
  else
  {
    std::printf(u8"Error: Unrecognized operational mode during\n");
    std::printf(u8"       NTC nominal resistance or csv filename argument\n");
    std::printf(u8"       parsing. Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  double NTC_nom_temp_C = std::numeric_limits<double>::quiet_NaN();

  if (OPERATING_MODE == NTC_PARAMETER_MODE)
  {
    /**
     * Parse the temperature of NTC nominal resistance input.
     */
    this_input = std::string(argv[NTC_NOM_TEMP_ARGV_INDX]);
    NTC_nom_temp_C = parse_double(this_input);

    if (std::isnan(NTC_nom_temp_C))
    {
      std::printf(u8"Input Error: could not parse the temperature for the NTC\n");
      std::printf(u8"             nominal resistance, \"%s\".\n\n", this_input.c_str());    
      return EXIT_FAILURE;
    }
    else if (NTC_nom_temp_C < MIN_NTC_NOM_TEMP_C)
    {
      std::printf(u8"Input Error: the temperature for the NTC nominal resistance\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be <%.3f\u00B0C.\n\n", MIN_NTC_NOM_TEMP_C);
      return EXIT_FAILURE;
    }
    else if (NTC_nom_temp_C >= MAX_NTC_NOM_TEMP_C)
    {
      std::printf(u8"Input Error: the temperature for the NTC nominal resistance\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be \u22652054\u00B0C. The common\n");
      std::printf(u8"             resistor substrate Alumina melts @ %.0f\u00B0C.\n\n", MAX_NTC_NOM_TEMP_C);
      return EXIT_FAILURE;
    }
  }
  else if (OPERATING_MODE == NTC_TABLE_MODE)
  {
    /* Do nothing. */
  }
  else
  {
    std::printf(u8"Error: Unrecognized operational mode during\n");
    std::printf(u8"       NTC nominal temperature argument\n");
    std::printf(u8"       parsing. Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  double beta_K = std::numeric_limits<double>::quiet_NaN();

  if (OPERATING_MODE == NTC_PARAMETER_MODE)
  {
    /**
     * Parse NTC nominal Beta coefficient input.
     */
    this_input = std::string(argv[BETA_ARGV_INDX]);
    beta_K = parse_double(this_input);

    if (std::isnan(beta_K))
    {
      std::printf(u8"Input Error: could not parse the NTC nominal \u03B2 coefficient\n");
      std::printf(u8"             \"%s\".\n\n", this_input.c_str());
      return EXIT_FAILURE;
    }
    else if (beta_K < MIN_BETA_K)
    {
      std::printf(u8"Input Error: the NTC nominal \u03B2 coefficient value\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be <%.0fK.\n\n", MIN_BETA_K);
      return EXIT_FAILURE;
    }
    else if (beta_K > MAX_BETA_K)
    {
      std::printf(u8"Input Error: the NTC nominal \u03B2 coefficient value\n");
      std::printf(u8"             \"%s\"\n", this_input.c_str());
      std::printf(u8"             should not be >%.0fK.\n\n", MAX_BETA_K);
      return EXIT_FAILURE;
    }
  }
  else if (OPERATING_MODE == NTC_TABLE_MODE)
  {
    /* Do nothing. */
  }
  else
  {
    std::printf(u8"Error: Unrecognized operational mode during\n");
    std::printf(u8"       NTC nominal beta coefficient argument\n");
    std::printf(u8"       parsing. Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  /**
   * Parse the pullup resistance input.
   */
  this_input = std::string(argv[RPULLUP_NOM_ARGV_INDX]);
  double Rpullup_nom_Ohms = parse_resistance(this_input);

  if (std::isnan(Rpullup_nom_Ohms))
  {
    std::printf(u8"Input Error: could not parse the pullup resistor nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return EXIT_FAILURE;
  }
  else if (Rpullup_nom_Ohms < MIN_RPULLUP_NOM_OHMS)
  {
    std::printf(u8"Input Error: the pullup resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.0f\u03A9.\n\n", MIN_RPULLUP_NOM_OHMS);
    return EXIT_FAILURE;
  }
  else if (Rpullup_nom_Ohms > MAX_RPULLUP_NOM_OHMS)
  {
    std::printf(u8"Input Error: the pullup resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.0fM\u03A9.\n\n", 1.0e-6 * MAX_RPULLUP_NOM_OHMS);
    return EXIT_FAILURE;
  }

  /**
   * Parse the isolation resistance input.
   */
  this_input = std::string(argv[RISO_NOM_ARGV_INDX]);
  double Riso_nom_Ohms = parse_resistance(this_input);

  if (std::isnan(Riso_nom_Ohms))
  {
    std::printf(u8"Input Error: could not parse the isolation resistor nominal resistance\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return EXIT_FAILURE;
  }
  else if (Riso_nom_Ohms < MIN_RISO_NOM_OHMS)
  {
    std::printf(u8"Input Error: the isolation resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.0f\u03A9.\n\n", MIN_RISO_NOM_OHMS);
    return EXIT_FAILURE;
  }
  else if (Riso_nom_Ohms > MAX_RISO_NOM_OHMS)
  {
    std::printf(u8"Input Error: the isolation resistor nominal resistance value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%.0fM\u03A9.\n\n", 1.0e-6 * MAX_RISO_NOM_OHMS);
    return EXIT_FAILURE;
  }

  /**
   * Parse the number of ADC counts input.
   */
  this_input = std::string(argv[ADC_COUNTS_ARGV_INDX]);
  bool parse_ok = false;
  int64_t tmp_ADC_counts = parse_int64(this_input, parse_ok);

  if (!parse_ok)
  {
    std::printf(u8"Input Error: could not parse the ADC number of counts\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return EXIT_FAILURE;
  }
  else if (tmp_ADC_counts < MIN_ADC_COUNTS)
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%" PRIi64 ".\n\n", MIN_ADC_COUNTS);
    return EXIT_FAILURE;
  }
  else if (tmp_ADC_counts > MAX_ADC_COUNTS)
  {
    std::printf(u8"Input Error: the ADC number of counts\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be >%" PRIi64 ".\n\n", MAX_ADC_COUNTS);
    return EXIT_FAILURE;
  }

  uint16_t ADC_counts = static_cast<uint16_t>(tmp_ADC_counts);

  /**
   * Parse the maximum interpolation error input.
   */
  this_input = std::string(argv[MAX_INTERP_ERROR_ARGV_INDX]);
  double max_interp_error_C = parse_double(this_input);

  if (std::isnan(max_interp_error_C))
  {
    std::printf(u8"Input Error: could not parse the maximum interpolation error\n");
    std::printf(u8"             \"%s\".\n\n", this_input.c_str());
    return EXIT_FAILURE;
  }
  else if (max_interp_error_C < MIN_MAX_INTERP_ERROR_C)
  {
    std::printf(u8"Input Error: the maximum interpolation error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be <%.10f, which is half\n", MIN_MAX_INTERP_ERROR_C);
    std::printf(u8"             the value of one least significant bit in the\n");
    std::printf(u8"             underlying fixed point representation\n\n");
    return EXIT_FAILURE;
  }
  else if (max_interp_error_C >= std::max(std::fabs(min_table_temp_C), std::fabs(max_table_temp_C)))
  {
    std::printf(u8"Input Error: the maximum interpolation error value\n");
    std::printf(u8"             \"%s\"\n", this_input.c_str());
    std::printf(u8"             should not be larger than the absolute value\n");
    std::printf(u8"             of the minimum or maximum table temperature.\n\n");
    return EXIT_FAILURE;
  }

  /**
   * Do a little more validation of lowest and highest table temperatures.
   */
  bool any_warnings = false;
  double lowest_Rntc = std::numeric_limits<double>::quiet_NaN();
  if (OPERATING_MODE == NTC_PARAMETER_MODE) lowest_Rntc = Rntc_from_Tntc(max_table_temp_C, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
  else if (OPERATING_MODE == NTC_TABLE_MODE) lowest_Rntc = Rntc_from_Tntc(max_table_temp_C, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while\n");
    std::printf(u8"       checking minimum table NTC resistance.\n");
    std::printf(u8"       Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (!std::isfinite(lowest_Rntc))
  {
    std::printf(u8"Error: Encountered invalid calculation result while\n");
    std::printf(u8"       checking minimum table NTC resistance.\n");
    std::printf(u8"       Please check your input parameters / data.\n");
    std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (lowest_Rntc < MIN_RNTC_OHMS)
  {
    std::printf(u8"Input Error: the highest table temperature value\n");
    std::printf(u8"             and the NTC parameters result in an\n");
    std::printf(u8"             NTC resistance that is < %.3e \u03A9.\n", MIN_RNTC_OHMS);
    return EXIT_FAILURE;
  }

  uint16_t table_start_count = 0u;
  if (OPERATING_MODE == NTC_PARAMETER_MODE) table_start_count = ADCread_from_Tntc(max_table_temp_C, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);
  else if (OPERATING_MODE == NTC_TABLE_MODE) table_start_count = ADCread_from_Tntc(max_table_temp_C, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while\n");
    std::printf(u8"       checking table starting ADC count.\n");
    std::printf(u8"       Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  double real_max_table_temp_C = std::numeric_limits<double>::quiet_NaN();
  if (OPERATING_MODE == NTC_PARAMETER_MODE) real_max_table_temp_C = Tntc_from_ADCread(table_start_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
  else if (OPERATING_MODE == NTC_TABLE_MODE) real_max_table_temp_C = Tntc_from_ADCread(table_start_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while\n");
    std::printf(u8"       checking table actual maximum temperature.\n");
    std::printf(u8"       Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (!std::isfinite(real_max_table_temp_C))
  {
    std::printf(u8"Error: Encountered invalid calculation result while\n");
    std::printf(u8"       checking table actual maximum temperature.\n");
    std::printf(u8"       Please check your input parameters / data.\n");
    std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (table_start_count > 0u && real_max_table_temp_C < max_table_temp_C)
  {
    table_start_count--;
    lowest_Rntc = Rntc_from_ADCread(table_start_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);

    if (!std::isfinite(lowest_Rntc))
    {
      std::printf(u8"Error: Encountered invalid calculation result while\n");
      std::printf(u8"       rechecking minimum table NTC resistance.\n");
      std::printf(u8"       Please check your input parameters / data.\n");
      std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
      return EXIT_FAILURE;
    }

    if (lowest_Rntc < MIN_RNTC_OHMS)
    {
      std::printf(u8"Input Error: the highest table temperature value\n");
      std::printf(u8"             and the NTC parameters result in an\n");
      std::printf(u8"             NTC resistance that is < %.3e \u03A9.\n", MIN_RNTC_OHMS);
      return EXIT_FAILURE;
    }

    if (OPERATING_MODE == NTC_PARAMETER_MODE) real_max_table_temp_C = Tntc_from_ADCread(table_start_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
    else if (OPERATING_MODE == NTC_TABLE_MODE) real_max_table_temp_C = Tntc_from_ADCread(table_start_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
    else
    {
      std::printf(u8"Error: Unrecognized operational mode while\n");
      std::printf(u8"       adjusting table starting ADC count.\n");
      std::printf(u8"       Please file a bug report.\n\n");
      return EXIT_FAILURE;
    }

    if (!std::isfinite(real_max_table_temp_C))
    {
      std::printf(u8"Error: Encountered invalid calculation result while\n");
      std::printf(u8"       adjusting table actual maximum temperature.\n");
      std::printf(u8"       Please check your input parameters / data.\n");
      std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
      return EXIT_FAILURE;
    }

    if (OPERATING_MODE == NTC_PARAMETER_MODE) lowest_Rntc = Rntc_from_Tntc(real_max_table_temp_C, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
    else if (OPERATING_MODE == NTC_TABLE_MODE) lowest_Rntc = Rntc_from_Tntc(real_max_table_temp_C, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
    else
    {
      std::printf(u8"Error: Unrecognized operational mode while\n");
      std::printf(u8"       rechecking minimum table NTC resistance.\n");
      std::printf(u8"       Please file a bug report.\n\n");
      return EXIT_FAILURE;
    }

    if (!std::isfinite(lowest_Rntc))
    {
      std::printf(u8"Error: Encountered invalid calculation result while\n");
      std::printf(u8"       double checking minimum table NTC resistance.\n");
      std::printf(u8"       Please check your input parameters / data.\n");
      std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
      return EXIT_FAILURE;
    }

    if (lowest_Rntc < MIN_RNTC_OHMS)
    {
      std::printf(u8"Input Error: the highest table temperature value\n");
      std::printf(u8"             and the NTC parameters result in an\n");
      std::printf(u8"             NTC resistance that is < %.3e \u03A9.\n", MIN_RNTC_OHMS);
      return EXIT_FAILURE;
    }
  }

  if (table_start_count == 0u)
  {
    any_warnings = true;
    std::printf(u8"WARNING: The interpolation table lowest ADC count, corresponding\n");
    std::printf(u8"         to a max temperature of %.8f\u00B0C,\n", real_max_table_temp_C);
    std::printf(u8"         is %u, which is the min output of the ADC.\n", table_start_count);
    std::printf(u8"         This thermistor circuit appears to be saturating.\n");
    std::printf(u8"         Please check your input parameters / data. If everything\n");
    std::printf(u8"         looks ok, please file a bug report.\n\n");
  }

  if (real_max_table_temp_C > MAX_FIXEDPOINTABLE_TEMP_C)
  {
    std::printf(u8"Input Error: the nearest ADC count that encompasses\n");
    std::printf(u8"             the highest table temperature value results\n");
    std::printf(u8"             in an actual highest table temperature that\n");
    std::printf(u8"             exceeds %.8f\u00B0C.\n", MAX_FIXEDPOINTABLE_TEMP_C);
    std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n\n");
    return EXIT_FAILURE;
  }

  if (max_table_temp_C - real_max_table_temp_C > INV_128)
  {
    any_warnings = true;
    std::printf(u8"WARNING: The interpolation table actual maximum temperature\n");
    std::printf(u8"         is %.10f\u00B0C, which is less than\n", real_max_table_temp_C);
    std::printf(u8"         the requested %.10f\u00B0C. This can occur if\n", max_table_temp_C);
    std::printf(u8"         the thermistor circuit output voltage reaches\n");
    std::printf(u8"         the limit of the ADC.\n");
    std::printf(u8"         Please check your input parameters / data.\n");
    std::printf(u8"         If everything looks ok, please file a bug report.\n\n");
  }

  uint16_t table_end_count = 0u;
  if (OPERATING_MODE == NTC_PARAMETER_MODE) table_end_count = ADCread_from_Tntc(min_table_temp_C, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);
  else if (OPERATING_MODE == NTC_TABLE_MODE) table_end_count = ADCread_from_Tntc(min_table_temp_C, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while\n");
    std::printf(u8"       checking table ending ADC count.\n");
    std::printf(u8"       Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  double real_min_table_temp_C = std::numeric_limits<double>::quiet_NaN();
  if (OPERATING_MODE == NTC_PARAMETER_MODE) real_min_table_temp_C = Tntc_from_ADCread(table_end_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
  else if (OPERATING_MODE == NTC_TABLE_MODE) real_min_table_temp_C = Tntc_from_ADCread(table_end_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while\n");
    std::printf(u8"       checking table actual minimum temperature.\n");
    std::printf(u8"       Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (!std::isfinite(real_min_table_temp_C))
  {
    std::printf(u8"Error: Encountered invalid calculation result while\n");
    std::printf(u8"       checking table actual minimum temperature.\n");
    std::printf(u8"       Please check your input parameters / data.\n");
    std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (table_end_count < ADC_counts - 1u && real_min_table_temp_C > min_table_temp_C)
  {
    table_end_count++;
    if (OPERATING_MODE == NTC_PARAMETER_MODE) real_min_table_temp_C = Tntc_from_ADCread(table_end_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
    else if (OPERATING_MODE == NTC_TABLE_MODE) real_min_table_temp_C = Tntc_from_ADCread(table_end_count, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
    else
    {
      std::printf(u8"Error: Unrecognized operational mode while\n");
      std::printf(u8"       adjusting table ending ADC count.\n");
      std::printf(u8"       Please file a bug report.\n\n");
      return EXIT_FAILURE;
    }

    if (!std::isfinite(real_min_table_temp_C))
    {
      std::printf(u8"Error: Encountered invalid calculation result while\n");
      std::printf(u8"       rechecking table actual minimum temperature.\n");
      std::printf(u8"       Please check your input parameters / data.\n");
      std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
      return EXIT_FAILURE;
    }
  }

  if (table_end_count >= ADC_counts)
  {
    std::printf(u8"Error: The interpolation table highest ADC count, corresponding\n");
    std::printf(u8"       to a min temperature of %.8f\u00B0C,\n", real_min_table_temp_C);
    std::printf(u8"       is %u, which is outside the range of the ADC.\n", table_end_count);
    std::printf(u8"       Please check your input parameters / data. If everything\n");
    std::printf(u8"       looks ok, please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (table_end_count == ADC_counts - 1u)
  {
    any_warnings = true;
    std::printf(u8"WARNING: The interpolation table highest ADC count, corresponding\n");
    std::printf(u8"         to a min temperature of %.8f\u00B0C,\n", real_min_table_temp_C);
    std::printf(u8"         is %u, which is the max output of the ADC.\n", table_end_count);
    std::printf(u8"         This thermistor circuit appears to be saturating.\n");
    std::printf(u8"         Please check your input parameters / data. If everything\n");
    std::printf(u8"         looks ok, please file a bug report.\n\n");
  }

  if (real_min_table_temp_C < MIN_FIXEDPOINTABLE_TEMP_C)
  {
    std::printf(u8"Input Error: the nearest ADC count that encompasses\n");
    std::printf(u8"             the lowest table temperature value results\n");
    std::printf(u8"             in an actual lowest table temperature less\n");
    std::printf(u8"             than %.8f\u00B0C.\n", MIN_FIXEDPOINTABLE_TEMP_C);
    std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
    std::printf(u8"             temperature representable in an int16_t.\n");
    std::printf(u8"             Please check your input parameters / data.\n");
    std::printf(u8"             If everything looks ok, please file a bug report.\n\n");
    return EXIT_FAILURE;
  }

  if (real_min_table_temp_C - min_table_temp_C > INV_128)
  {
    any_warnings = true;
    std::printf(u8"WARNING: The interpolation table actual minimum temperature\n");
    std::printf(u8"         is %.10f\u00B0C, which is greater than\n", real_min_table_temp_C);
    std::printf(u8"         the requested %.10f\u00B0C. This can occur if\n", min_table_temp_C);
    std::printf(u8"         the thermistor circuit output voltage reaches\n");
    std::printf(u8"         the limit of the ADC.\n");
    std::printf(u8"         Please check your input parameters / data.\n");
    std::printf(u8"         If everything looks ok, please file a bug report.\n\n");
  }

  /**
   * Redisplay input arguments back to user.
   */
  std::printf(u8"[Inputs]\n");
  if (OPERATING_MODE == NTC_PARAMETER_MODE)
  {
    std::printf(u8"NTC: %.1f\u03A9 @ %.1f\u00B0C, \u03B2 = %.0fK\n", Rntc_nom_Ohms, NTC_nom_temp_C, beta_K);
  }
  else if (OPERATING_MODE == NTC_TABLE_MODE)
  {
    std::printf(u8"NTC: Specified by %s.\n", csv_filename.c_str());
  }
  else
  {
    std::printf(u8"Error: Unrecognized operational mode during\n");
    std::printf(u8"       input argument display. Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }
  std::printf(u8"Pullup Resistor: %.1f\u03A9\n", Rpullup_nom_Ohms);
  std::printf(u8"Isolation Resistor:  %.1f\u03A9\n", Riso_nom_Ohms);
  std::printf(u8"ADC: %u counts\n", ADC_counts);
  std::printf(u8"Table temperatures: lowest = %.8f\u00B0C, highest = %.8f\u00B0C\n", real_min_table_temp_C, real_max_table_temp_C);
  std::printf(u8"Max interpolation error: %.6f\u00B0C\n\n", max_interp_error_C);

  std::printf(u8"table start: ADC count %5u\n", table_start_count);
  std::printf(u8"table end:   ADC count %5u\n", table_end_count);

  /**
   * Fill in table of NTC temperature readings indexed by ADC count
   * for the whole table range.
   */
  if (OPERATING_MODE == NTC_PARAMETER_MODE)
  {
    for (uint16_t ADC_read = table_start_count; ADC_read <= table_end_count; ADC_read++)
    {
      Tntc_array[ADC_read] = Tntc_from_ADCread(ADC_read, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
      if (!std::isfinite(Tntc_array[ADC_read]))
      {
        std::printf(u8"Error: Invalid calculation result while precomputing temperatures\n");
        std::printf(u8"       from ADC counts. Please check your input parameters / data.\n");
        std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
        return EXIT_FAILURE;
      }

      if (Tntc_array[ADC_read] < MIN_FIXEDPOINTABLE_TEMP_C ||
          Tntc_array[ADC_read] > MAX_FIXEDPOINTABLE_TEMP_C)
      {
        std::printf(u8"Error: Encountered out-of-range value while precomputing temperatures\n");
        std::printf(u8"       from ADC counts. Please check your input parameters / data.\n");
        std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
        return EXIT_FAILURE;
      }
    }
  }
  else if (OPERATING_MODE == NTC_TABLE_MODE)
  {
    for (uint16_t ADC_read = table_start_count; ADC_read <= table_end_count; ADC_read++)
    {
      Tntc_array[ADC_read] = Tntc_from_ADCread(ADC_read, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms, NTC_temp_res_data, csv_stored_rows, cubic_interp_segs);
      if (!std::isfinite(Tntc_array[ADC_read]))
      {
        std::printf(u8"Error: Invalid calculation result while precomputing temperatures\n");
        std::printf(u8"       from ADC counts. Please check your input parameters / data.\n");
        std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
        return EXIT_FAILURE;
      }

      if (Tntc_array[ADC_read] < MIN_FIXEDPOINTABLE_TEMP_C ||
          Tntc_array[ADC_read] > MAX_FIXEDPOINTABLE_TEMP_C)
      {
        std::printf(u8"Error: Encountered out-of-range value while precomputing temperatures\n");
        std::printf(u8"       from ADC counts. Please check your input parameters / data.\n");
        std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
        return EXIT_FAILURE;
      }
    }
  }
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while precomputing\n");
    std::printf(u8"       temperatures from ADC counts. Please file a bug report.\n\n");
    return EXIT_FAILURE;
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
    double kept_mean_err = std::fabs(kept_segment.start_temp * INV_128 - Tntc_array[kept_segment.start_count]);
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

      if (!std::isfinite(parameters[0]) || !std::isfinite(parameters[1]))
      {
        std::printf(u8"Error: Encountered Inf and/or NaN during least-squares\n");
        std::printf(u8"       fit of trial segment. Please check your input\n");
        std::printf(u8"       parameters / data. If everything looks ok, please\n");
        std::printf(u8"       file a bug report.\n\n");
        return EXIT_FAILURE;
      }

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
        std::printf(u8"Error: could not find an int32_t rational approximation to\n");
        std::printf(u8"       %f which has a power of 2 denominator.\n", frac_slope);
        std::printf(u8"       Please check your input parameters / data.\n");
        std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
        return EXIT_FAILURE;
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
        double this_abs_error = std::fabs(Tntc_array[test_segment.start_count + jPoint] - static_cast<double>(this_temp) * INV_128);
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
                static_cast<double>(this_segment.start_temp) * INV_128,
                this_segment.slope_multiplier, this_segment.slope_shift,
                static_cast<double>(this_segment.slope_multiplier) / static_cast<double>(1ull << this_segment.slope_shift));
  }
  std::printf(u8"\n");

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
  std::printf(u8"\n");

  /**
   * Print the struct definitions. I consider these
   * to be an important part of the table code.
   */
  std::printf(u8"/" "**\n");
  std::printf(u8" * interp_segment_t defines a single linear interpolation\n");
  std::printf(u8" *                  segment.\n");
  std::printf(u8" *\n");
  std::printf(u8" * start_count: the ADC count value corresponding to\n");
  std::printf(u8" *              start_temp\n");
  std::printf(u8" *\n");
  std::printf(u8" * start_temp: the temperature corresponding to start_count\n");
  std::printf(u8" *             in 1/128ths of a degree Celsius.\n");
  std::printf(u8" *             This is signed Q9.7 format fixed point.\n");
  std::printf(u8" *\n");
  std::printf(u8" * slope_multiplier: these two define the slope of the\n");
  std::printf(u8" * slope_shift:      line segment as the rational number\n");
  std::printf(u8" *                   (slope_multiplier / 2^slope_shift).\n");
  std::printf(u8" *                   Units are 1/128ths of a degree Celsius\n");
  std::printf(u8" *                   per ADC count.\n");
  std::printf(u8" *\n");
  std::printf(u8" * Each segment ends one count before the start of the\n");
  std::printf(u8" * next segment. end_count in interp_table_t gives the last\n");
  std::printf(u8" * valid ADC count for the final segment.\n");
  std::printf(u8" *" "/\n");
  std::printf(u8"typedef struct\n");
  std::printf(u8"{\n");
  std::printf(u8"  uint16_t start_count;\n");
  std::printf(u8"  int16_t start_temp;\n");
  std::printf(u8"  int32_t slope_multiplier;\n");
  std::printf(u8"  uint8_t slope_shift;\n");
  std::printf(u8"} interp_segment_t;\n\n");

  std::printf(u8"/" "**\n");
  std::printf(u8" * Converts a raw ADC reading of the thermistor circuit\n");
  std::printf(u8" * into a temperature in 1/128ths of a degree Celsius.\n");
  std::printf(u8" *\n");
  std::printf(u8" * This code was autogenerated with the following parameters:\n");
  if (OPERATING_MODE == NTC_PARAMETER_MODE)
  {
    std::printf(u8" * NTC Thermistor: %.1f Ohms nominal @ %.1f deg. C.\n", Rntc_nom_Ohms, NTC_nom_temp_C);
    std::printf(u8" *                 Beta = %.0f K\n", beta_K);
  }
  else if (OPERATING_MODE == NTC_TABLE_MODE)
  {
    std::printf(u8" * NTC Thermistor: Defined by Temperature / Resistance data\n");
    std::printf(u8" *                 provided in %s.\n", csv_filename.c_str());
  }
  else
  {
    std::printf(u8"Error: Unrecognized operational mode while displaying\n");
    std::printf(u8"       generated code. Please file a bug report.\n\n");
    return EXIT_FAILURE;
  }
  std::printf(u8" * Pullup resistor: %.1f Ohms nominal.\n", Rpullup_nom_Ohms);
  std::printf(u8" *   - The pullup resistor connects between the NTC and the\n");
  std::printf(u8" *     positive voltage supply.\n");
  std::printf(u8" * Isolation resistor: %.1f Ohms nominal\n", Riso_nom_Ohms);
  std::printf(u8" *   - The isolation resistor connects between the NTC and GND.\n");
  std::printf(u8" * Full ADC count range: 0-%u\n", ADC_counts - 1u);
  std::printf(u8" * Max interpolation error: %.8f deg. C\n", max_interp_error_C);
  double true_min_table_temp_C = static_cast<double>(interp_segments[n_stored_segments - 1u].start_temp + 
    multshiftround<int32_t>(table_end_count - interp_segments[n_stored_segments - 1u].start_count, 
                            interp_segments[n_stored_segments - 1u].slope_multiplier, 
                            interp_segments[n_stored_segments - 1u].slope_shift)) * INV_128;
  std::printf(u8" * Table range: %.8f to %.8f deg. C\n", true_min_table_temp_C, interp_segments[0].start_temp * INV_128);
  std::printf(u8" * ADCcount inputs >= %u result in the minimum table temperature.\n", table_end_count);
  std::printf(u8" * ADCcount inputs <= %u result in the maximum table temperature.\n", interp_segments[0].start_count);
  std::printf(u8" *" "/\n");
  std::printf(u8"int16_t read_thermistor(const uint16_t ADCcount)\n");
  std::printf(u8"{\n");
  std::printf(u8"  static const uint16_t num_segments = %uu;\n", n_stored_segments);
  std::printf(u8"  static const interp_segment_t interp_segments[num_segments] = {\n");
  for (uint16_t jSegment = 0u; jSegment + 1u < n_stored_segments; jSegment++)
  {
    interp_segment_t this_segment = interp_segments[jSegment];
    std::printf(u8"    {%5u, % 6i, % 6i, %2u},\n", this_segment.start_count, this_segment.start_temp, this_segment.slope_multiplier, this_segment.slope_shift);
  }
  
  if (n_stored_segments > 0u)
  {
    interp_segment_t this_segment = interp_segments[n_stored_segments-1u];
    std::printf(u8"    {%5u, % 6i, % 6i, %2u}\n", this_segment.start_count, this_segment.start_temp, this_segment.slope_multiplier, this_segment.slope_shift);
  }

  std::printf(u8"  };\n");
  std::printf(u8"  static const uint16_t last_segment_end_count = %u;\n", table_end_count);
  std::printf(u8"\n");
  std::printf(u8"  /" "**\n");
  std::printf(u8"   * Check input ADCcount against table min & max ADC counts.\n");
  std::printf(u8"   *" "/\n");
  std::printf(u8"  if (ADCcount <= interp_segments[0].start_count)\n");
  std::printf(u8"  {\n");
  std::printf(u8"    return interp_segments[0].start_temp;\n");
  std::printf(u8"  }\n");
  std::printf(u8"\n");
  std::printf(u8"  uint16_t seg_index = 0u;\n");
  std::printf(u8"\n");
  std::printf(u8"  if (ADCcount >= last_segment_end_count)\n");
  std::printf(u8"  {\n");
  std::printf(u8"    seg_index = num_segments - 1u;\n");
  std::printf(u8"    return interp_segments[seg_index].start_temp +\n");
  std::printf(u8"           multshiftround<int32_t>(last_segment_end_count - interp_segments[seg_index].start_count,\n");
  std::printf(u8"                                   interp_segments[seg_index].slope_multiplier,\n");
  std::printf(u8"                                   interp_segments[seg_index].slope_shift);\n");
  std::printf(u8"  }\n");
  std::printf(u8"\n");
  std::printf(u8"  /" "**\n");
  std::printf(u8"   * Find the interpolation segment that contains ADCcount\n");
  std::printf(u8"   * via binary search.\n");
  std::printf(u8"   *" "/\n");
  std::printf(u8"  uint16_t lower_bound = 0u;\n");
  std::printf(u8"  uint16_t upper_bound = num_segments - 1u;\n");
  std::printf(u8"  seg_index = (lower_bound + upper_bound) >> 1;\n");
  std::printf(u8"\n");
  std::printf(u8"  while (true)\n");
  std::printf(u8"  {\n");
  std::printf(u8"    if (ADCcount < interp_segments[seg_index].start_count)\n");
  std::printf(u8"    {\n");
  std::printf(u8"      upper_bound = seg_index - 1u;\n");
  std::printf(u8"      seg_index = (lower_bound + upper_bound) >> 1;\n");
  std::printf(u8"    }\n");
  std::printf(u8"    else if (seg_index + 1u < num_segments &&\n");
  std::printf(u8"             ADCcount >= interp_segments[seg_index + 1u].start_count)\n");
  std::printf(u8"    {\n");
  std::printf(u8"      lower_bound = seg_index + 1u;\n");
  std::printf(u8"      seg_index = (lower_bound + upper_bound) >> 1;\n");
  std::printf(u8"    }\n");
  std::printf(u8"    else\n");
  std::printf(u8"    {\n");
  std::printf(u8"      return interp_segments[seg_index].start_temp +\n");
  std::printf(u8"             multshiftround<int32_t>(ADCcount - interp_segments[seg_index].start_count,\n");
  std::printf(u8"                                     interp_segments[seg_index].slope_multiplier,\n");
  std::printf(u8"                                     interp_segments[seg_index].slope_shift);\n");
  std::printf(u8"    }\n");
  std::printf(u8"  }\n");
  std::printf(u8"}\n\n\n");
  
  if (any_warnings)
  {
    std::printf(u8"thermistor_interpolator finished with warnings.\n");
    std::printf(u8"Please read the warning messages above the code\n");
    std::printf(u8"and other numberical output above.\n\n");
  }
  else
  {
    std::printf(u8"thermistor_interpolator completed successfully.\n\n");
  }

  return EXIT_SUCCESS;
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
