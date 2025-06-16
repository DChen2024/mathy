#include "mathy.h"

#include <math.h>
#include <float.h>

// IEEE-754 floating-point recommended

/*
The integer base-2 logarithm is also the index of the most significant
set bit. This can be trivially found using compiler intrinsics.

For unknown compilers, convert to floating-point representation and use
type punning to extract the exponent. For the range of uint64_t, not all
values are exactly representable, and a correction step must be applied.
*/
int8_t mathy_ilog2(uint64_t n) {
#if __GNUC__ /* GCC or Clang */
    return n ? (64-1)-__builtin_clzll(n) : -1;
#elif _MSC_VER /* MSVC */
    uint32_t x;
#if _WIN64 /* 64-bit */
    return _BitScanReverse64(&x, n) ? (int8_t)x : -1;
#else /* 32-bit */
    return _BitScanReverse(&x, (uint32_t)(n>>32)) ? (int8_t)(x+32) :
        _BitScanReverse(&x, (uint32_t)n) ? (int8_t)x : -1;
#endif /* Word size */
#else /* Not GCC, Clang, or MSVC */
    union {
        float f;
        uint32_t u;
    } _ = { .f = (float)n };
    uint8_t x = (_.u>>(FLT_MANT_DIG-1))-(FLT_MAX_EXP-1);
    return (int8_t)(x-1+(x==64 ? 0 : n>>x));
#endif /* Compiler */
}

/*
As sqrt is commonly performed in hardware, use (int)sqrt. This may not be
accurate for large values, and a correction step must be applied.
If the implementation supports IEEE floating-point arithmetic, for the
range of uint64_t, (int)sqrt yields either the floor or ceil of sqrt.
https://github.com/python/cpython/issues/81138

Even if the implementation does not fully conform to IEEE floating-point
arithmetic, take (int)sqrt to still satisfy the above condition. In the
final correction, use x*x-1 >= n instead of the simpler x*x > n in order
to get the correct result in the corner case where x = 1<<32.
*/
uint32_t mathy_isqrt(uint64_t n) {
    uint64_t x = n ? (uint64_t)sqrt((double)n) : 1;
    return (uint32_t)(x-(x*x-1>=n));
}

// Computes the greatest common denominator (GCD) of two numbers.
uintmax_t mathy_gcd(uintmax_t a, uintmax_t b) {
    uintmax_t t;
    while (b) {
        t = b;
        b = a%b;
        a = t;
    }
    return a;
}

// Computes the least common multiple (LCM) of two numbers.
uintmax_t mathy_lcm(uintmax_t a, uintmax_t b) {
    return a|b ? a*b/mathy_gcd(a, b) : 0;
}

// Computes the number of permutations.
uintmax_t mathy_permutation(uint8_t n, uint8_t k) {
    uintmax_t prod = 1;
    for (uint8_t i = 0; i < k; i++)
        prod *= n-i;
    return prod;
}

// Computes the number of combinations.
uintmax_t mathy_combination(uint8_t n, uint8_t k) {
    uintmax_t prod = 1;
    k = k<n-k ? k : n-k;
    for (uint8_t i = 0; i < k; i++)
        prod = prod*(n-i)/(i+1); // limit overflow
    return prod;
}

/*
C++20 introduces the lerp function to the standard library. The goal is to
guarantee the basic properties of exactness, monotonicity, determinacy,
boundedness, and consistency, as detailed in this document.
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0811r3.html

(1-t)*a+t*b
This does not overflow if a and b have the largest exponent and opposite
signs. As noted in the document, this is not monotonic unless a*b <= 0.

t<0.5 ? a+t*(b-a) : b-(1-t)*(b-a)
This modified version is monotonic, according to the following discussion.
https://math.stackexchange.com/q/907327
As noted in the document, this can overflow if a and b have the largest
exponent and opposite signs. Note that this version is exact at t = 1.

Note that the implementation in the paper does not satisfy the standard when
one endpoint is zero and t is infinite, as the value returned is NaN whereas
it should be infinite. Therefore, use the following hybrid implementation.
*/
float mathy_lerpf(float a, float b, float t) {
    return (a<0 && b>0) || (a>0 && b<0) ? (1-t)*a+t*b :
        t<0.5 ? a+t*(b-a) : b-(1-t)*(b-a);
}
double mathy_lerp(double a, double b, double t) {
    return (a<0 && b>0) || (a>0 && b<0) ? (1-t)*a+t*b :
        t<0.5 ? a+t*(b-a) : b-(1-t)*(b-a);
}
long double mathy_lerpl(long double a, long double b, long double t) {
    return (a<0 && b>0) || (a>0 && b<0) ? (1-t)*a+t*b :
        t<0.5 ? a+t*(b-a) : b-(1-t)*(b-a);
}

/*
Wichura, M. J. "Algorithm AS241: The Percentage Points of the
Normal Distribution". Journal of the Royal Statistical Society.
Series C (Applied Statistics), Vol. 37, No. 3 (1988), pp. 477-484.
https://doi.org/10.2307/2347330

The following function is modified from the CPython statistics module
and is subject to the following license agreement.
https://github.com/python/cpython/blob/main/Modules/_statisticsmodule.c

Copyright (c) 2001 Python Software Foundation; All Rights Reserved
https://docs.python.org/3/license.html#psf-license
*/
float mathy_norminvf(float p) {
    if (signbit(p) || p > 1)
        return NAN;
    float q, num, den, r, x;
    q = p-0.5;
    if (fabsf(q) <= 0.425f) { // p close to 1/2
        r = 0.180625f-q*q; // assert(r >= 0);
        num = ((((5.9109374720e+1f)*r+
                  1.5929113202e+2f)*r+
                  5.0434271938e+1f)*r+
                  3.3871327179e+0f)*q;
        den = ((((6.7187563600e+1f)*r+
                  7.8757757664e+1f)*r+
                  1.7895169469e+1f)*r+
                  1.0f); // assert(den != 0);
        x = num/den;
        return x;
    }
    r = signbit(q) ? p : 1-p; // r = min(p, 1-p);
    r = sqrtf(-logf(r)); // assert(r > 1.6);
    if (r <= 5.0f) { // p neither close to 1/2 nor 0 or 1
        r -= 1.6f; // assert(r > 0);
        num = ((((1.7023821103e-1f)*r+
                  1.3067284816e0f)*r+
                  2.7568153900e0f)*r+
                  1.4234372777e0f);
        den = ((((1.2021132975e-1f)*r+
                  7.3700164250e-1f)*r+
                  1.0f)); // assert(den != 0);
    } else if (r != INFINITY) { // p near 0 or 1
        r -= 5.0f; // assert(r > 0);
        num = ((((1.7337203997e-2f)*r+
                  4.2868294337e-1f)*r+
                  3.0812263860e0f)*r+
                  6.6579051150e0f);
        den = ((((1.2258202635e-2f)*r+
                  2.4197894225e-1f)*r+
                  1.0f)); // assert(den != 0);
    } else { // p equal to 0 or 1
        num = INFINITY, den = 1; // return +-inf
    }
    x = num/den;
    x = signbit(q) ? -x : x;
    return x;
}
double mathy_norminv(double p) {
    if (signbit(p) || p > 1)
        return NAN;
    double q, num, den, r, x;
    q = p-0.5;
    if (fabs(q) <= 0.425) { // p close to 1/2
        r = 0.180625-q*q; // assert(r > 0);
        num = ((((((((2.5090809287301226727e+3)*r+
                      3.3430575583588128105e+4)*r+
                      6.7265770927008700853e+4)*r+
                      4.5921953931549871457e+4)*r+
                      1.3731693765509461125e+4)*r+
                      1.9715909503065514427e+3)*r+
                      1.3314166789178437745e+2)*r+
                      3.3871328727963666080e+0)*q;
        den = ((((((((5.2264952788528545610e+3)*r+
                      2.8729085735721942674e+4)*r+
                      3.9307895800092710610e+4)*r+
                      2.1213794301586595867e+4)*r+
                      5.3941960214247511077e+3)*r+
                      6.8718700749205790830e+2)*r+
                      4.2313330701600911252e+1)*r+
                      1.0); // assert(den != 0);
        x = num/den;
        return x;
    }
    r = signbit(q) ? p : 1-p; // r = min(p, 1-p);
    r = sqrt(-log(r)); // assert(r > 1.6);
    if (r <= 5.0) { // p neither close to 1/2 nor 0 or 1
        r -= 1.6; // assert(r > 0);
        num = ((((((((7.74545014278341407640e-4)*r+
                      2.27238449892691845833e-2)*r+
                      2.41780725177450611770e-1)*r+
                      1.27045825245236838258e+0)*r+
                      3.64784832476320460504e+0)*r+
                      5.76949722146069140550e+0)*r+
                      4.63033784615654529590e+0)*r+
                      1.42343711074968357734e+0);
        den = ((((((((1.05075007164441684324e-9)*r+
                      5.47593808499534494600e-4)*r+
                      1.51986665636164571966e-2)*r+
                      1.48103976427480074590e-1)*r+
                      6.89767334985100004550e-1)*r+
                      1.67638483018380384940e+0)*r+
                      2.05319162663775882187e+0)*r+
                      1.0); // assert(den != 0);
    } else if (r != INFINITY) { // p near 0 or 1
        r -= 5.0; // assert(r > 0);
        num = ((((((((2.01033439929228813265e-7)*r+
                      2.71155556874348757815e-5)*r+
                      1.24266094738807843860e-3)*r+
                      2.65321895265761230930e-2)*r+
                      2.96560571828504891230e-1)*r+
                      1.78482653991729133580e+0)*r+
                      5.46378491116411436990e+0)*r+
                      6.65790464350110377720e+0);
        den = ((((((((2.04426310338993978564e-15)*r+
                      1.42151175831644588870e-7)*r+
                      1.84631831751005468180e-5)*r+
                      7.86869131145613259100e-4)*r+
                      1.48753612908506148525e-2)*r+
                      1.36929880922735805310e-1)*r+
                      5.99832206555887937690e-1)*r+
                      1.0); // assert(den != 0);
    } else { // p equal to 0 or 1
        num = INFINITY, den = 1; // return +-inf
    }
    x = num/den;
    x = signbit(q) ? -x : x;
    return x;
}
