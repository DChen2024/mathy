#include "mathy.h"

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.707106781186547524401
#endif
#define M_1_SQRT2PI 0.398942280401432702863

#ifdef _MSC_VER
#include <intrin.h>
#endif

#define MATHY_VERSION_MAJOR 1
#define MATHY_VERSION_MINOR 0
#define MATHY_VERSION_PATCH 0
// Encodes version information
const uint16_t MATHY_VERSION = MATHY_VERSION_MAJOR<<12 |
    MATHY_VERSION_MINOR<<8 | MATHY_VERSION_PATCH<<4;

// Calculate the integer base 2 logarithm of a number, rounded down.
int8_t ilog2(uint64_t n) {
    if (n == 0)
        return -1;
#if __GNUC__ // GCC or Clang
    return __builtin_clzll(n)^0x3F;
#elif _MSC_VER // MSVC
    return __lzcnt64(n)^0x3F;
#else
    #error "Compile using GCC, Clang, or MSVC"
#endif
}

// Calculate the integer square root of a number, rounded down.
uint32_t isqrt(uint64_t n) {
    // assume implementation follows IEEE-754
    // sqrt is required by IEEE-754 to be correctly rounded
    // however, at large values, there may not be enough precision
    // such that sqrt is rounded up to the nearest integer
    // long double is only required to be at least as precise as double,
    // and the size of long double varies across implementations
    // these values are numerically determined
    // assume the implementation complies with IEEE-754
    // note that compiling with -ffast-math will break IEEE-754 compliance
    if (n < 16785407)
        return (uint32_t)sqrtf(n);
    if (n < 4503599761588224)
        return (uint32_t)sqrt(n);
#if __GNUC__ // GCC and Clang
    return (uint32_t)sqrtl(n);
#else // MSVC
    // for MSVC, double == long double
    // time complexity O(log(n))
    // Newton's method
    uint64_t x0 = (uint64_t)1<<(ilog2(n)/2+1);
    uint32_t x1 = (x0+n/x0)/2;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0+n/x0)/2;
    }
    return (uint32_t)x0;
#endif
}

// Calculate the integer cube root of a number, rounded down.
uint32_t icbrt(uint64_t n) {
    // time complexity O(log(n))
    // Newton's method
    if (n == 0) 
        return 0;
    uint32_t x0 = (uint32_t)1<<(ilog2(n)/3+1);
    uint64_t x1 = (2*x0+n/(x0*x0))/3;
    while (x1 < x0) {
        x0 = x1;
        x1 = (2*x0+n/(x0*x0))/3;
    }
    return x0;
}

// Calculate the greatest common denominator of two numbers.
intmax_t gcd(intmax_t a, intmax_t b) {
    // time complexity O(log(min(a,b)))
    // Euclid's algorithm
    while (b) {
        intmax_t temp = b;
        b = a%b;
        a = temp;
    }
    return a<0 ? -a : a; // return abs(a)
}

// Calculate the least common multiple of two numbers.
intmax_t lcm(intmax_t a, intmax_t b) {
    // accurate if in range of intmax_t
    if ((a|b) == 0) // if (a == 0 && b == 0)
        return 0; // avoid undefined behavior, gcd(a, b) will return 0
    a *= b/gcd(a, b); // limit overflow
    return a<0 ? -a : a; // return abs(a)
}

// Check if a number is prime.
bool prime(uint64_t n) {
    // handle basic cases
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n%2 == 0 || n%3 == 0)
        return false;
    // time complexity O(sqrt(n))
    // prime numbers greater than 3 are of the form 6k +- 1
    for (uint64_t i = 5, stop = isqrt(n); i <= stop; i += 6)
        if (n%i == 0 || n%(i+2) == 0)
            return false;
    return true;
}

// Calculate the roots of a quadratic. Check whether there are real roots.
// For real roots, x1 is the smaller root and x2 is the larger root.
// For complex roots, x1 is the real part and x2 is the imaginary part.
bool quadraticf(float a, float b, float c,
                float *x1, float *x2) {
    float d = b*b-4*a*c;
    // reduce logic to sort roots
    if (signbit(a)) // if (a < 0)
        a = -a, b = -b, c = -c;
    if (signbit(d)) { // if (d < 0)
        *x1 = (-b)/(2*a), *x2 = sqrtf(-d)/(2*a);
        return false;
    }
    // better precision when b*b >> 4*a*c
    // https://math.stackexchange.com/a/2007723
    if (signbit(b)) // if (b < 0)
        d = -b+sqrtf(d), *x1 = (2*c)/(d), *x2 = (d)/(2*a);
    else
        d = -b-sqrtf(d), *x1 = (d)/(2*a), *x2 = (2*c)/(d);
    return true;
}
bool quadratic(double a, double b, double c,
               double *x1, double *x2) {
    double d = b*b-4*a*c;
    // reduce logic to sort roots
    if (signbit(a)) // if (a < 0)
        a = -a, b = -b, c = -c;
    if (signbit(d)) { // if (d < 0)
        *x1 = (-b)/(2*a), *x2 = sqrt(-d)/(2*a);
        return false;
    }
    // better precision when b*b >> 4*a*c
    // https://math.stackexchange.com/a/2007723
    if (signbit(b)) // if (b < 0)
        d = -b+sqrt(d), *x1 = (2*c)/(d), *x2 = (d)/(2*a);
    else
        d = -b-sqrt(d), *x1 = (d)/(2*a), *x2 = (2*c)/(d);
    return true;
}
bool quadraticl(long double a, long double b, long double c,
                long double *x1, long double *x2) {
    long double d = b*b-4*a*c;
    // reduce logic to sort roots
    if (signbit(a)) // if (a < 0)
        a = -a, b = -b, c = -c;
    if (signbit(d)) { // if (d < 0)
        *x1 = (-b)/(2*a), *x2 = sqrtl(-d)/(2*a);
        return false;
    }
    // better precision when b*b >> 4*a*c
    // https://math.stackexchange.com/a/2007723
    if (signbit(b)) // if (b < 0)
        d = -b+sqrtl(d), *x1 = (2*c)/(d), *x2 = (d)/(2*a);
    else
        d = -b-sqrtl(d), *x1 = (d)/(2*a), *x2 = (2*c)/(d);
    return true;
}

// Calculate the linear interpolation between two numbers.
float lerpf(float a, float b, float t) {
    // matches endpoints, no overflow, and is monotonic
    // https://math.stackexchange.com/a/1798323
    return a*b<0 ? (1-t)*a+t*b : t<=0.5 ? a+(b-a)*t : b-(b-a)*(1-t);
}
double lerp(double a, double b, double t) {
    // matches endpoints, no overflow, and is monotonic
    // https://math.stackexchange.com/a/1798323
    return a*b<0 ? (1-t)*a+t*b : t<=0.5 ? a+(b-a)*t : b-(b-a)*(1-t);
}
long double lerpl(long double a, long double b, long double t) {
    // matches endpoints, no overflow, and is monotonic
    // https://math.stackexchange.com/a/1798323
    return a*b<0 ? (1-t)*a+t*b : t<=0.5 ? a+(b-a)*t : b-(b-a)*(1-t);
}

// Calculate the factorial of a number.
uintmax_t factorial(uint8_t n) {
    // time complexity O(n)
    // accurate if in range of uintmax_t
    // uint64_t can store up to 20!
    uintmax_t prod = 1;
    for (uint8_t i = n; i; --i)
        prod *= i;
    return prod;
}

// Calculate the number of permutations.
uintmax_t permutation(uint8_t n, uint8_t k) {
    if (k > n)
        return 0;
    // time complexity O(k)
    // accurate if in range of uintmax_t
    // uint64_t accurate if (n <= 20 || k <= 8)
    uintmax_t prod = 1;
    for (uint8_t i = k; i; --n, --i)
        prod *= n;
    return prod;
}

// Calculate the number of combinations.
uintmax_t combination(uint8_t n, uint8_t k) {
    if (k > n)
        return 0;
    if (n-k < k) // nCr = nCn-r
        k = n-k;
    // time complexity O(k)
    // accurate if in range of uintmax_t
    // uint64_t accurate if (n <= 62 || k <= 10)
    uintmax_t prod = 1;
    for (uint8_t i = 1; i <= k; --n, ++i)
        prod = prod*n/i; // limit overflow
    return prod;
}

// Calculate the probability mass function of a binomial distribution.
double binom_pmf(uint8_t k, uint8_t n, double p) {
    // accuracy depends on nCr(n, k)
    // returns nan for invalid input
    // signbit(p) catches p = -0.0, !(p <= 1) catches p = nan("")
    if (signbit(p) || !(p <= 1)) // if (p < 0 || p > 1)
        return nan("");
    return combination(n, k)*pow(p, k)*pow(1-p, n-k); // pow(0, 0) returns 1
}

// Calculate the cumulative density function of a binomial distribution.
double binom_cdf(uint8_t k, uint8_t n, double p) {
    // accuracy depends on nCr(n, k)
    // returns nan for invalid input
    // signbit(p) catches p = -0.0, !(p <= 1) catches p = nan("")
    if (signbit(p) || !(p <= 1)) // if (p < 0 || p > 1)
        return nan("");
    double x = 0;
    for (int16_t i = 0; i <= k; ++i)
        x += combination(n, i)*pow(p, i)*pow(1-p, n-i); // pow(0, 0) returns 1
    return x;
}

// Calculate the probability density function of a normal distribution.
double normal_pdf(double x, double mu, double sigma) {
    // returns nan for invalid input
    if (sigma <= 0)
        return nan("");
    double z = (x-mu)/sigma;
    return M_1_SQRT2PI*exp(-0.5*z*z)/sigma;
}

// Calculate the cumulative density function of a normal distribution.
double normal_cdf(double x, double mu, double sigma) {
    // returns nan for invalid input
    if (sigma <= 0)
        return nan("");
    double z = (x-mu)/sigma;
    return 0.5*(1+erf(M_SQRT1_2*z));
}

// Calculate the inverse cumulative density function of a normal distribution.
double normal_inv_cdf(double p, double mu, double sigma) {
    // returns nan for invalid input
    // signbit(p) catches p = -0.0, !(p <= 1) catches p = nan("")
    if (sigma <= 0 || signbit(p) || !(p <= 1))
        return nan("");
    // uses the implementation of statistics.NormalDist().inv_cdf() in Python
    // https://github.com/python/cpython/blob/main/Modules/_statisticsmodule.c
    double q, num, den, r, x;
    q = p-0.5;
    if (fabs(q) <= 0.425) {
        // use the following polynomial fit near the mean
        r = 0.180625-q*q; // r cannot be negative
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
                      1.0); // den cannot be zero
        x = num/den;
        return mu+(x*sigma);
    }
    r = (q <= 0) ? p : (1-p);
    r = sqrt(-log(r));
    if (r <= 5.0) {
        // use the following polynomial fit far from the mean
        r -= 1.6; // r cannot be negative
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
                      1.0); // den cannot be zero
    } else {
            // use the following polynomial fit very far from the mean
            r -= 5.0; // r cannot be negative
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
                          1.0); // den cannot be zero
    }
    x = num/den;
    if (q < 0)
        x = -x;
    return mu+(x*sigma);
}
