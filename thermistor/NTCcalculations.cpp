/**
 * NTCcalculations.cpp
 *
 * Definitions of several mathematical routines concerned with calculating
 * the resistance, temperature, or ADCreading associated with a Negative
 * Temperature Coefficient (NTC) thermistor in a circuit with a pullup
 * resistor and optional isolation resistor monitored by an Analog to Digital
 * Converter (ADC).
 * 
 * These functions use some of the constants defined in constants.h.
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

#include "NTCcalculations.h"
#include "constants.h"
#include <cassert>
#include <cmath>
#include <limits>

/**
 * Calculates nominal NTC resistance in Ohms
 * given an ADC reading on the range [0, ADC_counts - 1].
 * 
 * ADC_counts: Total number of counts in ADC (1024 for 10-bit, 4096 for 12-bit, etc.)
 * Rpullup_nom_Ohms: Nominal resistance (Ω) of pullup resistor in NTC measurement circuit.
 * Riso_nom_Ohms: Nominal resistance (Ω) of resistor between NTC and GND.
 */
double Rntc_from_ADCread(const uint16_t ADCread, const uint16_t ADC_counts, const double Rpullup_nom_Ohms, const double Riso_nom_Ohms)
{
  assert(ADC_counts >= MIN_ADC_COUNTS);
  assert(ADC_counts <= MAX_ADC_COUNTS);
  
  assert(ADCread < ADC_counts);
  
  assert(Rpullup_nom_Ohms >= MIN_RPULLUP_NOM_OHMS);
  assert(Rpullup_nom_Ohms <= MAX_RPULLUP_NOM_OHMS);

  assert(Riso_nom_Ohms >= MIN_RISO_NOM_OHMS);
  assert(Riso_nom_Ohms <= MAX_RISO_NOM_OHMS);
  
  double ADCratio = 0.0;
  
  if (ADCread == 0u) ADCratio = 0.5 / static_cast<double>(ADC_counts - (uint16_t)1u);
  else if (ADCread == ADC_counts - 1u) ADCratio = (static_cast<double>(ADC_counts) - 1.5) / static_cast<double>(ADC_counts - (uint16_t)1u);
  else ADCratio = static_cast<double>(ADCread) / static_cast<double>(ADC_counts - (uint16_t)1u);
  
  return (Rpullup_nom_Ohms * ADCratio - Riso_nom_Ohms * (1.0 - ADCratio)) / (1.0 - ADCratio);
}

/**
 * Calculates nominal NTC resistance in Ohms for a given
 * NTC temperature in degrees Celsius.
 * 
 * NTC_temp_C: NTC actual temperature (°C)
 * Rntc_nom_Ohms: NTC nominal resistance (Ω) at NTC_nom_temp_C (°C)
 * beta_K: NTC nominal ß coefficient (K)
 */
double Rntc_from_Tntc(const double NTC_temp_C, const double Rntc_nom_Ohms, const double beta_K, const double NTC_nom_temp_C)
{
  assert(std::isfinite(NTC_temp_C));
  assert(NTC_temp_C >= -KELVIN_OFFSET);

  assert(Rntc_nom_Ohms >= MIN_RNTC_NOM_OHMS);
  assert(Rntc_nom_Ohms <= MAX_RNTC_NOM_OHMS);

  assert(beta_K >= MIN_BETA_K);
  assert(beta_K <= MAX_BETA_K);

  assert(NTC_nom_temp_C >= MIN_NTC_NOM_TEMP_C);
  assert(NTC_nom_temp_C <= MAX_NTC_NOM_TEMP_C);

  return Rntc_nom_Ohms * std::exp(beta_K * (1.0 / (NTC_temp_C + KELVIN_OFFSET) - 1.0 / (NTC_nom_temp_C + KELVIN_OFFSET)));
}

/**
 * Looks up NTC resistance in Ohms for a given NTC temperature
 * in degrees Celsius using the interpolated NTC thermistor
 * temperature / resistance data supplied by the user.
 * 
 * data: points to the parsed NTC thermistor temperature / resistance
 *       data that came from the user's .csv file
 * num_points: is the number of valid temperature / resistance data points
 * segments: stores the parameters for the piecewise cubic segments interpolating
 *           the points in data
 */
double Rntc_from_Tntc(const double NTC_temp_C, const NTC_temp_res_row_t *data, const uint32_t num_points, const cubic_interp_seg_t *segments)
{
  assert(std::isfinite(NTC_temp_C));
  assert(NTC_temp_C >= -KELVIN_OFFSET);

  assert(num_points >= MIN_CSV_ROWS);
  assert(num_points <= MAX_CSV_ROWS);

  /**
   * Check input NTC_temp_C against table min & max temperatures.
   */
  if (NTC_temp_C <= data[0].temp_C) return data[0].res_Ohms;
  if (NTC_temp_C >= data[num_points - 1u].temp_C) return data[num_points - 1u].res_Ohms;

  /**
   * Find the interpolation segment that contains NTC_temp_C
   * via binary search.
   */
  uint32_t lower_bound = 0u;
  uint32_t upper_bound = num_points - 2u;
  uint32_t seg_index = (lower_bound + upper_bound) >> 1;

  while (true)
  {
    if (NTC_temp_C < data[seg_index].temp_C)
    {
      upper_bound = seg_index - 1u;
      seg_index = (lower_bound + upper_bound) >> 1;
    }
    else if (seg_index + 1u < num_points - 1u &&
             NTC_temp_C >= data[seg_index + 1u].temp_C)
    {
      lower_bound = seg_index + 1u;
      seg_index = (lower_bound + upper_bound) >> 1;
    }
    else
    {
      NTC_temp_res_row_t seg_row = data[seg_index];
      double x = NTC_temp_C - seg_row.temp_C;
      double x_2 = x * x;
      cubic_interp_seg_t seg_coeffs = segments[seg_index];
      return seg_coeffs.a * x * x_2 + seg_coeffs.b * x_2 + seg_coeffs.c * x + seg_row.res_Ohms;
    }
  }
}

/**
 * Calculates nominal NTC temperature in degrees
 * Celsius given an ADC reading on the range
 * [0, ADC_counts - 1].
 * Returns NaN for infeasible ADC readings.
 * 
 * ADC_counts: Total number of counts in ADC (1024 for 10-bit, 4096 for 12-bit, etc.)
 * Rpullup_nom_Ohms: Nominal resistance (Ω) of pullup resistor in NTC measurement circuit.
 * Riso_nom_Ohms: Nominal resistance (Ω) of resistor between NTC and GND.
 * data: points to the parsed NTC thermistor temperature / resistance
 *       data that came from the user's .csv file
 * num_points: is the number of valid temperature / resistance data points
 * segments: stores the parameters for the piecewise cubic segments interpolating
 *           the points in data
 */
double Tntc_from_ADCread(const uint16_t ADCread, const uint16_t ADC_counts, const double Rpullup_nom_Ohms, 
                         const double Riso_nom_Ohms, const double Rntc_nom_Ohms, const double beta_K, const double NTC_nom_temp_C)
{
  assert(ADC_counts >= MIN_ADC_COUNTS);
  assert(ADC_counts <= MAX_ADC_COUNTS);
  
  assert(ADCread < ADC_counts);
  
  assert(Rpullup_nom_Ohms >= MIN_RPULLUP_NOM_OHMS);
  assert(Rpullup_nom_Ohms <= MAX_RPULLUP_NOM_OHMS);

  assert(Riso_nom_Ohms >= MIN_RISO_NOM_OHMS);
  assert(Riso_nom_Ohms <= MAX_RISO_NOM_OHMS);

  assert(Rntc_nom_Ohms >= MIN_RNTC_NOM_OHMS);
  assert(Rntc_nom_Ohms <= MAX_RNTC_NOM_OHMS);

  assert(beta_K >= MIN_BETA_K);
  assert(beta_K <= MAX_BETA_K);

  assert(NTC_nom_temp_C >= MIN_NTC_NOM_TEMP_C);
  assert(NTC_nom_temp_C <= MAX_NTC_NOM_TEMP_C);

  double Rntc = Rntc_from_ADCread(ADCread, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);
  assert(std::isfinite(Rntc));
  assert(Rntc >= MIN_RNTC_OHMS);

  return 1.0 / (std::log(Rntc / Rntc_nom_Ohms) / beta_K + 1.0 / (NTC_nom_temp_C + KELVIN_OFFSET)) - KELVIN_OFFSET;
}

/**
 * Calculates nominal NTC temperature in degrees
 * Celsius given an ADC reading on the range
 * [0, ADC_counts - 1].
 * Returns NaN for infeasible ADC readings.
 * 
 * ADC_counts: Total number of counts in ADC (1024 for 10-bit, 4096 for 12-bit, etc.)
 * Rpullup_nom_Ohms: Nominal resistance (Ω) of pullup resistor in NTC measurement circuit.
 * Riso_nom_Ohms: Nominal resistance (Ω) of resistor between NTC and GND.
 * Rntc_nom_Ohms: NTC nominal resistance (Ω) at NTC_nom_temp_C (°C)
 * beta_K: NTC nominal ß coefficient (K)
 */
double Tntc_from_ADCread(const uint16_t ADCread, const uint16_t ADC_counts, const double Rpullup_nom_Ohms, 
                         const double Riso_nom_Ohms, const NTC_temp_res_row_t *data, const uint32_t num_points, const cubic_interp_seg_t *segments)
{
  assert(ADC_counts >= MIN_ADC_COUNTS);
  assert(ADC_counts <= MAX_ADC_COUNTS);

  assert(ADCread < ADC_counts);
  
  assert(Rpullup_nom_Ohms >= MIN_RPULLUP_NOM_OHMS);
  assert(Rpullup_nom_Ohms <= MAX_RPULLUP_NOM_OHMS);

  assert(Riso_nom_Ohms >= MIN_RISO_NOM_OHMS);
  assert(Riso_nom_Ohms <= MAX_RISO_NOM_OHMS);

  assert(num_points >= MIN_CSV_ROWS);
  assert(num_points <= MAX_CSV_ROWS);

  double Rntc = Rntc_from_ADCread(ADCread, ADC_counts, Rpullup_nom_Ohms, Riso_nom_Ohms);
  assert(std::isfinite(Rntc));
  assert(Rntc >= MIN_RNTC_OHMS);
  
  /**
   * Check Rntc against table min & max resistances.
   */
  if (Rntc >= data[0].res_Ohms) return data[0].temp_C;
  if (Rntc <= data[num_points - 1u].res_Ohms) return data[num_points - 1u].temp_C;

  /**
   * Find the interpolation segment that contains Rntc
   * via binary search.
   */
  uint32_t lower_bound = 0u;
  uint32_t upper_bound = num_points - 2u;
  uint32_t seg_index = (lower_bound + upper_bound) >> 1;

  while (true)
  {
    if (Rntc > data[seg_index].res_Ohms)
    {
      upper_bound = seg_index - 1u;
      seg_index = (lower_bound + upper_bound) >> 1;
    }
    else if (seg_index + 1u < num_points - 1u &&
             Rntc <= data[seg_index + 1u].res_Ohms)
    {
      lower_bound = seg_index + 1u;
      seg_index = (lower_bound + upper_bound) >> 1;
    }
    else
    {
      /**
       * Use linear interpolation to get an initial estimate
       * for Newton's method.
       */
      NTC_temp_res_row_t seg_row = data[seg_index];
      NTC_temp_res_row_t next_row = data[seg_index+1u];
      double guess_temp_C = seg_row.temp_C + (next_row.temp_C - seg_row.temp_C) * (seg_row.res_Ohms - Rntc) / (seg_row.res_Ohms - next_row.res_Ohms);
      
      /**
       * Solve for the temperature that yields Rntc via Newton's method.
       */
      uint8_t jIteration = 0u;
      cubic_interp_seg_t seg_coeffs = segments[seg_index];
      double x = guess_temp_C - seg_row.temp_C;
      double x_2 = x * x;
      double this_err = seg_coeffs.a * x * x_2 + seg_coeffs.b * x_2 + seg_coeffs.c * x + seg_row.res_Ohms - Rntc;

      while (std::fabs(this_err) > NEWTON_STOP_ERR_THRESHOLD)
      {
        double next_temp_C = guess_temp_C - this_err / (3.0 * seg_coeffs.a * x_2 + 2.0 * seg_coeffs.b * x + seg_coeffs.c);
        x = next_temp_C - seg_row.temp_C;
        x_2 = x * x;
        double next_err = seg_coeffs.a * x * x_2 + seg_coeffs.b * x_2 + seg_coeffs.c * x + seg_row.res_Ohms - Rntc;

        if (std::fabs(next_err) > std::fabs(this_err))
        {
          std::printf(u8"Error: Newton's method misstepped while back-calculating\n");
          std::printf(u8"       NTC temperature from resistance. Please check your\n");
          std::printf(u8"       input parameters / data. If everything looks ok,\n");
          std::printf(u8"       please file a bug report.\n\n");
          return std::numeric_limits<double>::quiet_NaN();
        }

        guess_temp_C = next_temp_C;
        this_err = next_err;
        jIteration++;

        if (jIteration > NEWTON_MAX_ITERATIONS)
        {
          std::printf(u8"Error: Newton's method failed to converge while\n");
          std::printf(u8"       back-calculating NTC temperature from resistance.\n");
          std::printf(u8"       Please check your input parameters / data.\n");
          std::printf(u8"       If everything looks ok, please file a bug report.\n\n");
          return std::numeric_limits<double>::quiet_NaN();
        }
      }
      
      if (!std::isfinite(guess_temp_C))
      {
        std::printf(u8"Error: Back-calculation of NTC temperature from resistance\n");
        std::printf(u8"       failed for an unknown reason. Please check your input\n");
        std::printf(u8"       parameters / data. If everything looks ok, please\n");
        std::printf(u8"       file a bug report.\n\n");
      }

      return guess_temp_C;
    }
  }
}

/**
 * Calculates nominal ADC reading for a given
 * NTC temperature in degrees Celsius
 * 
 * Rntc_nom_Ohms: NTC nominal resistance (Ω) at NTC_nom_temp_C (°C)
 * beta_K: NTC nominal ß coefficient (K)
 * ADC_counts: Total number of counts in ADC (1024 for 10-bit, 4096 for 12-bit, etc.)
 * Rpullup_nom_Ohms: Nominal resistance (Ω) of pullup resistor in NTC measurement circuit.
 * Riso_nom_Ohms: Nominal resistance (Ω) of resistor between NTC and GND.
 */
uint16_t ADCread_from_Tntc(const double NTC_temp_C, const double Rntc_nom_Ohms, const double beta_K, const double NTC_nom_temp_C,
                           const uint16_t ADC_counts, const double Rpullup_nom_Ohms, const double Riso_nom_Ohms)
{
  assert(std::isfinite(NTC_temp_C));
  assert(NTC_temp_C >= -KELVIN_OFFSET);

  assert(Rntc_nom_Ohms >= MIN_RNTC_NOM_OHMS);
  assert(Rntc_nom_Ohms <= MAX_RNTC_NOM_OHMS);

  assert(beta_K >= MIN_BETA_K);
  assert(beta_K <= MAX_BETA_K);

  assert(NTC_nom_temp_C >= MIN_NTC_NOM_TEMP_C);
  assert(NTC_nom_temp_C <= MAX_NTC_NOM_TEMP_C);

  assert(ADC_counts >= MIN_ADC_COUNTS);
  assert(ADC_counts <= MAX_ADC_COUNTS);

  assert(Rpullup_nom_Ohms >= MIN_RPULLUP_NOM_OHMS);
  assert(Rpullup_nom_Ohms <= MAX_RPULLUP_NOM_OHMS);

  assert(Riso_nom_Ohms >= MIN_RISO_NOM_OHMS);
  assert(Riso_nom_Ohms <= MAX_RISO_NOM_OHMS);

  double Rntc = Rntc_from_Tntc(NTC_temp_C, Rntc_nom_Ohms, beta_K, NTC_nom_temp_C);
  assert(std::isfinite(Rntc));
  assert(Rntc >= MIN_RNTC_OHMS);

  double ADCratio = (Rntc + Riso_nom_Ohms) / (Rntc + Riso_nom_Ohms + Rpullup_nom_Ohms);
  return static_cast<uint16_t>(std::round(ADCratio * static_cast<double>(ADC_counts - (uint16_t)1u)));
}

/**
 * Calculates nominal ADC reading for a given
 * NTC temperature in degrees Celsius
 * 
 * data: Points to the parsed NTC thermistor temperature / resistance
 *       data that came from the user's .csv file
 * num_points: The number of valid temperature / resistance data points
 * segments: Stores the parameters for the piecewise cubic segments interpolating
 *           the points in data
 * ADC_counts: Total number of counts in ADC (1024 for 10-bit, 4096 for 12-bit, etc.)
 * Rpullup_nom_Ohms: Nominal resistance (Ω) of pullup resistor in NTC measurement circuit.
 * Riso_nom_Ohms: Nominal resistance (Ω) of resistor between NTC and GND.
 */
uint16_t ADCread_from_Tntc(const double NTC_temp_C, const NTC_temp_res_row_t *data, const uint32_t num_points, 
                           const cubic_interp_seg_t *segments, const uint16_t ADC_counts, const double Rpullup_nom_Ohms,
                           const double Riso_nom_Ohms)
{
  assert(std::isfinite(NTC_temp_C));
  assert(NTC_temp_C >= -KELVIN_OFFSET);

  assert(num_points >= MIN_CSV_ROWS);
  assert(num_points <= MAX_CSV_ROWS);

  assert(ADC_counts >= MIN_ADC_COUNTS);
  assert(ADC_counts <= MAX_ADC_COUNTS);

  assert(Rpullup_nom_Ohms >= MIN_RPULLUP_NOM_OHMS);
  assert(Rpullup_nom_Ohms <= MAX_RPULLUP_NOM_OHMS);

  assert(Riso_nom_Ohms >= MIN_RISO_NOM_OHMS);
  assert(Riso_nom_Ohms <= MAX_RISO_NOM_OHMS);

  double Rntc = Rntc_from_Tntc(NTC_temp_C, data, num_points, segments);
  assert(std::isfinite(Rntc));
  assert(Rntc >= MIN_RNTC_OHMS);

  double ADCratio = (Rntc + Riso_nom_Ohms) / (Rntc + Riso_nom_Ohms + Rpullup_nom_Ohms);
  return static_cast<uint16_t>(std::round(ADCratio * static_cast<double>(ADC_counts - (uint16_t)1u)));
}

/**
 * Convert a floating point degrees Celsius temperature
 * into (1/128) degrees Celsius fixed point.
 */
int16_t fixed_point_C(const double temp_C)
{
  assert(temp_C >= MIN_FIXEDPOINTABLE_TEMP_C);
  assert(temp_C <= MAX_FIXEDPOINTABLE_TEMP_C);

  return static_cast<int16_t>(std::round(128.0 * temp_C));
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
