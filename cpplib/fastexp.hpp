// Copyright 2021 Johan Rade (johan.rade@gmail.com)
// Distributed under the MIT license (https://opensource.org/licenses/MIT) 

#ifndef FAST_EXP_H
#define FAST_EXP_H

#include <cstdint>
#include <cstring>

inline float fast_exp(float x);
inline double fast_exp(double x);

/*
These functions return an approximation of exp(x) with a relative error <3%.
They are several times faster than std::exp(). The implementation uses a
method by Nicole Schraudolph.
 
The functions do bounds checking and return 0 for large negative x and infinity
for large positive x.
The code assumes that values of type float and double are stored in the IEEE754
single and double precision floating point formats.
Implementation note:
The code uses uses memcpy to transform an integer to a floating point number
with the same binary representation. This is C++ standard-compliant and with
a modern optimizing compiler it should be as fast as non-standard-compliant
hacks using reinterpret_cast or union.
So how does this work?
If you want to understand how the code works, start by figuring out why
 
    inline float fastPowerOfTwo(float x)
    {
        x = (1 << 23) * (x + 127);
        uint32_t n = static_cast<uint32_t>(x);
        memcpy(&x, &n, 4);
        return x;
    }
returns an approximation of pow(2.0f, x).
If x is an integer it returns pow(2.0f, x) exactly. For x between integers,
it interpolates linearly. Thus it gives a piecewise linear approximation of
pow(2.0f, x). The relative error varies between 0% and 6.15%. Note that 23 is
the number of significand bits and 127 is the exponent bias in the IEEE754
single precision floating point format.
Next we can calulate exp(x) using the formula  exp(x) = pow(2, x / log(2)).
This explains the divisor  0.6931... = log(2)  in the in fastExp() code.
The term  0.04367...  in the fastExp() code is an adjustment term. Without this
term the relative error would vary between 0% and 6.15%. With this term it
varies between -2.98% and 2.98%. Optimal adjustment terms according to
different criteria are derived in the paper by Schraudolph. Here we use the
adjustment term that gives the lowest maximum relative error.
References:
N. Schraudolph, “A Fast, Compact Approximation of the Exponential Function”,
Neural Computation 11, 853–862 (1999).
(available at https://nic.schraudolph.org/pubs/Schraudolph99.pdf)
*/


inline float fast_exp(float x)
{
    constexpr float a = (1 << 23) / 0.69314718f;
    constexpr float b = (1 << 23) * (127 - 0.043677448f);
    x = a * x + b;

    constexpr float c = (1 << 23);
    constexpr float d = (1 << 23) * 255;
    if (x < c || x > d)
        x = (x < c) ? 0.0f : d;

    uint32_t n = static_cast<uint32_t>(x);
    memcpy(&x, &n, 4);
    return x;
}


inline double fast_exp(double x)
{
    constexpr double a = (1ll << 52) / 0.6931471805599453;
    constexpr double b = (1ll << 52) * (1023 - 0.04367744890362246);
    x = a * x + b;

    constexpr double c = (1ll << 52);
    constexpr double d = (1ll << 52) * 2047;
    if (x < c || x > d)
        x = (x < c) ? 0.0 : d;

    uint64_t n = static_cast<uint64_t>(x);
    memcpy(&x, &n, 8);
    return x;
}

#endif