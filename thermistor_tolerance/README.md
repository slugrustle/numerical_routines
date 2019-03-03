# thermistor\_tolerance
See below for example output from thermistor\_tolerance.exe, as run on 64-bit Windows.
___
```
powershell> .\thermistor_tolerance.exe 27k 1 100 33k 3 3950 2 4096 3 2.8 1.3 1.7 -40 0 25 85

Monte Carlo tolerance analysis of an NTC thermistor and pullup resistor
measured by an ADC. Error due to self-heating is not modelled.

If special characters (°, Ω, ±, etc.) do not display,
set your console to unicode (powershell> chcp 65001).

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
