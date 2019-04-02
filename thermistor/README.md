# thermistor\_tolerance
See below for example output from thermistor\_tolerance.exe, as run on 64-bit Windows.
___
```
PowerShell> .\thermistor_tolerance.exe 27k 1 100 33k 3 3950 2 4096 3 2.8 1.3 1.7 -40 0 25 85

Monte Carlo tolerance analysis of an NTC thermistor and pullup resistor
measured by an ADC. Error due to self-heating is not modelled.

If special characters (°, Ω, ±, etc.) do not display,
set your console to unicode (PowerShell> chcp 65001).

[Input Arguments]
1.  Pullup resistor nominal resistance (Ω)
2.  Pullup resistor initial tolerance (%)
3.  Pullup resistor temperature coefficient (ppm/°C)
4.  NTC nominal resistance at 25°C (Ω)
5.  Initial tolerance of NTC nominal resistance at 25°C (%)
6.  NTC nominal β coefficient (K)
7.  NTC β coefficient initial tolerance (%)
8.  ADC number of counts (1024 for 10-bit, 4096 for 12-bit, etc.)
9.  ADC max gain error (LSB)
10. ADC max offset error (LSB)
11. ADC max DNL error (LSB)
12. ADC max INL error (LSB)
13. Temperature at which to evaluate tolerance (°C)
(14, 15, ...). Optional additional temperatures at which to evaluate tolerance (°C)

[Example Command]
.\thermistor_tolerance.exe 47k 1 100 33k 5 3950 2 4096 3 2.8 1.3 1.7 -15 0 25 70


[Inputs]
Pullup Resistor: 27000.0Ω ±1.0% initial, ±100ppm/°C
NTC: 33000.0Ω ±3.0% @ 25°C, β = 3950K ±2.0%
ADC: 4096 counts, ±3.0LSB max gain error, ±2.8LSB max offset error
     ±1.3LSB max DNL error, ±1.7LSB max INL error

[Tolerance analysis]
Eval @  -40.00°C: min  -42.75°C (  -2.749°C), max  -37.56°C (  +2.438°C)
Eval @    0.00°C: min   -1.39°C (  -1.387°C), max    1.40°C (  +1.397°C)
Eval @   25.00°C: min   23.99°C (  -1.015°C), max   26.01°C (  +1.006°C)
Eval @   85.00°C: min   81.79°C (  -3.208°C), max   88.30°C (  +3.305°C)
```
# thermistor\_interpolator
See below for example output from thermistor\_interpolator.exe, as run on 64-bit Windows.
___
```
PowerShell> .\thermistor_interpolator.exe -30 90 33k 25 3950 22.1k 1.3k 4096 0.1

Near-optimal thermistor curve interpolation table generator.
Generates a table of line segments interpolating the temperature
vs. ADC counts curve for an NTC thermistor with specified parameters
such that the table error is below some bound.
Effort is made to minimize the number of interpolating segments
required to acheive this.

If special characters (°, Ω, ±, etc.) do not display,
set your console to unicode (PowerShell> chcp 65001).

[Input Arguments]
1. Lowest table temperature (°C)
2. Highest table temperature (°C)
3. NTC nominal resistance (Ω)
4. Temperature for NTC nominal resistance (°C)
5. NTC nominal β coefficient (K)
6. Pullup resistor nominal resistance (Ω)
   - The pullup resistor connects between the NTC
     and the positive voltage supply.
7. Isolation resistor nominal resistance (Ω)
   - The isolation resistor connects between the NTC
     and GND.
8. ADC number of counts (1024 for 10-bit, 4096 for 12-bit, etc.)
9. Maximum error due to interpolation (°C)

[Example Command]
.\thermistor_interpolator.exe -30 90 33k 25 3950 22.1k 1.3k 4096 0.1


[Inputs]
NTC: 33000.0Ω @ 25.0°C, β = 3950K
Pullup Resistor: 22100.0Ω
Isolation Resistor:  1300.0Ω
ADC: 4096 counts
Table temperatures: lowest = -30.03460354°C, highest = 90.03772046°C
Max interpolation error: 0.100000°C

table start: ADC count   677
table end:   ADC count  3963
segment   0:  start ADC count =   677,  offset =   11513 =    89.945313 °C,  slope =   -157 / 2^(  4) =    -9.812500 (1/128)°C / ADC count.
segment   1:  start ADC count =   760,  offset =   10698 =    83.578125 °C,  slope =  -1049 / 2^(  7) =    -8.195313 (1/128)°C / ADC count.
segment   2:  start ADC count =   861,  offset =    9870 =    77.109375 °C,  slope =   -879 / 2^(  7) =    -6.867188 (1/128)°C / ADC count.
segment   3:  start ADC count =   982,  offset =    9038 =    70.609375 °C,  slope =   -369 / 2^(  6) =    -5.765625 (1/128)°C / ADC count.
segment   4:  start ADC count =  1134,  offset =    8161 =    63.757813 °C,  slope =   -623 / 2^(  7) =    -4.867188 (1/128)°C / ADC count.
segment   5:  start ADC count =  1321,  offset =    7250 =    56.640625 °C,  slope =  -1061 / 2^(  8) =    -4.144531 (1/128)°C / ADC count.
segment   6:  start ADC count =  1559,  offset =    6263 =    48.929688 °C,  slope =   -917 / 2^(  8) =    -3.582031 (1/128)°C / ADC count.
segment   7:  start ADC count =  1869,  offset =    5151 =    40.242188 °C,  slope =   -407 / 2^(  7) =    -3.179688 (1/128)°C / ADC count.
segment   8:  start ADC count =  2347,  offset =    3646 =    28.484375 °C,  slope =  -3187 / 2^( 10) =    -3.112305 (1/128)°C / ADC count.
segment   9:  start ADC count =  2924,  offset =    1848 =    14.437500 °C,  slope =   -445 / 2^(  7) =    -3.476563 (1/128)°C / ADC count.
segment  10:  start ADC count =  3224,  offset =     804 =     6.281250 °C,  slope =  -1033 / 2^(  8) =    -4.035156 (1/128)°C / ADC count.
segment  11:  start ADC count =  3436,  offset =     -53 =    -0.414063 °C,  slope =  -1231 / 2^(  8) =    -4.808594 (1/128)°C / ADC count.
segment  12:  start ADC count =  3599,  offset =    -839 =    -6.554688 °C,  slope =   -749 / 2^(  7) =    -5.851563 (1/128)°C / ADC count.
segment  13:  start ADC count =  3721,  offset =   -1555 =   -12.148438 °C,  slope =   -927 / 2^(  7) =    -7.242188 (1/128)°C / ADC count.
segment  14:  start ADC count =  3815,  offset =   -2238 =   -17.484375 °C,  slope =   -579 / 2^(  6) =    -9.046875 (1/128)°C / ADC count.
segment  15:  start ADC count =  3884,  offset =   -2865 =   -22.382813 °C,  slope =   -731 / 2^(  6) =   -11.421875 (1/128)°C / ADC count.
segment  16:  start ADC count =  3938,  offset =   -3492 =   -27.281250 °C,  slope =   -223 / 2^(  4) =   -13.937500 (1/128)°C / ADC count.

segment   0 stats:  # points =   83,  mean error =  0.037608 °C,  max error =  0.092408 °C
segment   1 stats:  # points =  101,  mean error =  0.038288 °C,  max error =  0.093776 °C
segment   2 stats:  # points =  121,  mean error =  0.036639 °C,  max error =  0.092536 °C
segment   3 stats:  # points =  152,  mean error =  0.038769 °C,  max error =  0.098431 °C
segment   4 stats:  # points =  187,  mean error =  0.038022 °C,  max error =  0.096965 °C
segment   5 stats:  # points =  238,  mean error =  0.038576 °C,  max error =  0.099419 °C
segment   6 stats:  # points =  310,  mean error =  0.037859 °C,  max error =  0.099259 °C
segment   7 stats:  # points =  478,  mean error =  0.035881 °C,  max error =  0.099936 °C
segment   8 stats:  # points =  577,  mean error =  0.029464 °C,  max error =  0.095171 °C
segment   9 stats:  # points =  300,  mean error =  0.035447 °C,  max error =  0.094679 °C
segment  10 stats:  # points =  212,  mean error =  0.034675 °C,  max error =  0.098227 °C
segment  11 stats:  # points =  163,  mean error =  0.037516 °C,  max error =  0.097058 °C
segment  12 stats:  # points =  122,  mean error =  0.036848 °C,  max error =  0.097529 °C
segment  13 stats:  # points =   94,  mean error =  0.038535 °C,  max error =  0.091880 °C
segment  14 stats:  # points =   69,  mean error =  0.034339 °C,  max error =  0.099054 °C
segment  15 stats:  # points =   54,  mean error =  0.037454 °C,  max error =  0.097913 °C
segment  16 stats:  # points =   26,  mean error =  0.013662 °C,  max error =  0.034604 °C

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
 * interp_table_t is the top level of a table of interpolation
 *                segments that define some curve.
 *
 * num_segments: number of linear interpolation segments
 *               in this table.
 *
 * end_count: largest ADC count value for which the last
 *            interpolation segment is valid.
 *
 * p_segments: pointer to an array of num_segments interpolation
 *             segments.
 */
typedef struct
{
  uint16_t num_segments;
  uint16_t end_count;
  interp_segment_t *p_segments;
} interp_table_t;

interp_segment_t thermistor_interp_segment_array[17] = {
  {  677,  11513,   -157,  4},
  {  760,  10698,  -1049,  7},
  {  861,   9870,   -879,  7},
  {  982,   9038,   -369,  6},
  { 1134,   8161,   -623,  7},
  { 1321,   7250,  -1061,  8},
  { 1559,   6263,   -917,  8},
  { 1869,   5151,   -407,  7},
  { 2347,   3646,  -3187, 10},
  { 2924,   1848,   -445,  7},
  { 3224,    804,  -1033,  8},
  { 3436,    -53,  -1231,  8},
  { 3599,   -839,   -749,  7},
  { 3721,  -1555,   -927,  7},
  { 3815,  -2238,   -579,  6},
  { 3884,  -2865,   -731,  6},
  { 3938,  -3492,   -223,  4}
};

interp_table_t thermistor_table = {17, 3963, thermistor_interp_segment_array};

```
thermistor\_interpolator.cpp uses some code from version 3.3.7 of the Eigen library, which I retrieved from http://eigen.tuxfamily.org/. The files needed for compiling the thermistor\_interpolator program are stored in the Eigen directory in this repository. Eigen distributed under the [MPL2 license](http://mozilla.org/MPL/2.0/).