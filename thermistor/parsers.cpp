/**
 * parsers.cpp
 *
 * Definitions of several string to numerical value parsers used to 
 * interpret and validate user input in thermistor_interpolator.cpp.
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

#include "parsers.h"
#include "types.h"
#include "constants.h"
#include <exception>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <fstream>

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
    (void) e;
    success = false;
    return 0ll;
  }
  catch (const std::out_of_range &e)
  {
    (void) e;
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
    (void) e;
    return std::numeric_limits<double>::quiet_NaN();
  }
  catch (const std::out_of_range &e)
  {
    (void) e;
    return std::numeric_limits<double>::quiet_NaN();
  }

  if (after_double != in_str.length()) return std::numeric_limits<double>::quiet_NaN();
  return value;
}

/**
 * Parse resistances such as 33.2k, 10M, 100.2, 1, etc.
 * into a value in Ohms. Only the suffixes k and M are
 * recognized.
 * Return NaN if res_string is not parseable.
 * Negative and 0 values are returned as valid.
 */
double parse_resistance(std::string &res_string)
{
  if (res_string.length() == 0u) return std::numeric_limits<double>::quiet_NaN();

  /* Look for a k or M suffix */
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

/**
 * A basic string tokenizer.
 *
 * Contiguous runs of characters in the input string that are not in the set delimeters
 * will be returned as separate strings (tokens) in the output vector.
 */
std::vector<std::string> tokenize(const std::string &input, const std::set<std::string::value_type> &delimeters)
{
  std::vector<std::string> output;
  std::string this_token;
  bool last_char_nondelimeter = false;

  for (std::size_t jChar = 0u; jChar < input.size(); jChar++)
  {
    std::string::value_type this_char = input.at(jChar);
    bool this_char_nondelimeter = (delimeters.end() == delimeters.find(this_char));


    if (this_char_nondelimeter)
    {
      this_token.push_back(this_char);
    }
    else if (last_char_nondelimeter)
    {
      output.push_back(this_token);
      this_token.clear();
    }

    last_char_nondelimeter = this_char_nondelimeter;
  }

  if (!this_token.empty())
  {
    output.push_back(this_token);
  }

  return output;
}

/**
 * Parses a csv file with degrees Celsius temperatures in column 1
 * and corresponding NTC resistances in column 2 and stores this
 * data in the pointed-to array.
 * 
 * The number of valid rows commited to the storage array is saved
 * in stored_rows.
 */
bool parse_NTC_csv_file(const std::string &filename, NTC_temp_res_row_t *storage, uint32_t &stored_rows)
{
  static const std::set<std::string::value_type> delimeters = {',', ' ', '\n', '\r', '\t', '\f', '\v'};

  stored_rows = 0u;

  std::ifstream csv_file(filename);
  if (!csv_file.is_open())
  {
    std::printf("Input Error: could not open input .csv file\n");
    std::printf("             %s for reading.\n\n", filename.c_str());
    return false;
  }

  uint32_t jLine = 0u;
  std::string line;
  while (std::getline(csv_file, line) && stored_rows < max_csv_rows)
  {
    /* Erase a UTF-8 byte order mark if it is present. */
    if (jLine == 0 && line.size() >= 3 &&
        line.at(0) == '\xEF' && line.at(1) == '\xBB' && line.at(2) == '\xBF')
    {
        line = line.substr(3);
    }
  
    std::vector<std::string> tokens = tokenize(line, delimeters);

    /* Ignore lines with fewer than two "rows". */
    if (tokens.size() >= 2)
    {
      double row_temp_C = parse_double(tokens.at(0));
      if (std::isnan(row_temp_C))
      {
        std::printf(u8"Input Error: could not parse the temperature in column 1 on line %u\n", jLine);
        std::printf(u8"             of %s.\n\n", filename.c_str());
        return false;
      }
      else if (row_temp_C < -kelvin_offset)
      {
        std::printf(u8"Input Error: the temperature in column 1 on line %u\n", jLine);
        std::printf(u8"             of %s\n", filename.c_str());
        std::printf(u8"             should not be <-273.15\u00B0C (think about it).\n\n");
        return false;
      }
      else if (row_temp_C < std::numeric_limits<int16_t>::lowest() * inv_128)
      {
        std::printf(u8"Input Error: the temperature in column 1 on line %u\n", jLine);
        std::printf(u8"             of %s\n", filename.c_str());
        std::printf(u8"             should not be <%.8f\u00B0C.\n", std::numeric_limits<int16_t>::lowest() * inv_128);
        std::printf(u8"             This is the lowest 1/128th of a degree Celsius\n");
        std::printf(u8"             temperature representable in an int16_t.\n\n");
        return false;
      }
      else if (row_temp_C > std::numeric_limits<int16_t>::max() * inv_128)
      {
        std::printf(u8"Input Error: temperature in column 1 on line %u\n", jLine);
        std::printf(u8"             of %s\n", filename.c_str());
        std::printf(u8"             should not be >%.8f\u00B0C.\n", std::numeric_limits<int16_t>::max() * inv_128);
        std::printf(u8"             This is the highest 1/128th of a degree Celsius\n");
        std::printf(u8"             temperature representable in an int16_t.\n\n");
        return false;
      }

      double row_res_Ohms = parse_resistance(tokens.at(1));
      if (std::isnan(row_res_Ohms))
      {
        std::printf(u8"Input Error: could not parse the resistance in column 2 on line %u\n", jLine);
        std::printf(u8"             of %s.\n\n", filename.c_str());
        return false;
      }
      else if (row_res_Ohms < min_Rntc_Ohms)
      {
        std::printf(u8"Input Error: the resistance in column 2 on line %u\n", jLine);
        std::printf(u8"             of %s\n", filename.c_str());
        std::printf(u8"             should not be < %.3e \u03A9.\n", min_Rntc_Ohms);
        return false;
      }

      storage[stored_rows] = {row_temp_C, row_res_Ohms};
      stored_rows++;
    }

    jLine++;
  }

  csv_file.close();
  return true;
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
