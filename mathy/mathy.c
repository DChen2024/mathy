#include "mathy.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

#define abs(n) ((n)<0 ? -(n) : (n))
#define min(a,b) ((b)<(a) ? (b) : (a))
#define max(a,b) ((b)>(a) ? (b) : (a))

// Computes the integer base 2 logarithm of a number, rounded down.
int8_t mathy_ilog2(uint64_t n) {
    if (n == 0)
        return -1;
    // Compiler intrinsics
#if __GNUC__ /* GCC or Clang */
    return __builtin_clzll(n)^0x3F;
#elif _MSC_VER /* MSVC */
    return __lzcnt64(n)^0x3F;
#else
    #error "Compile using GCC, Clang, or MSVC"
#endif
}

/**
 * The sqrt function is required by the IEEE standard to be correctly rounded
 * from the infinitely precise result. For large floating-point values, it is
 * possible for sqrt to round up when expressed as the nearest representable
 * floating-point value. The values below are numerically determined.
 * 
 * For MSVC, long double is the same as double. This is not enough to cover the
 * range of uint64_t. For this, use the following CPython implementation.
 * http://github.com/python/cpython/blob/main/Modules/mathmodule.c
 */

// Computes the integer square root of a number, rounded down.
uint32_t mathy_isqrt(uint64_t n) {
    if (n < 16785407)
        return (uint32_t)sqrtf(n);
    if (n < 4503599761588224)
        return (uint32_t)sqrt(n);
#if __GNUC__ /* GCC or Clang */
    return (uint32_t)sqrtl(n);
#else /* MSVC */
    // CPython implementation
    uint64_t a = 1;
    for (int8_t c = (__lzcnt64(n)^0x3F)/2, s = __lzcnt64(c)^0x3F, d = 0, e;
        s >= 0; s--) {
        e = d;
        d = c>>s;
        a = (a<<d-e-1)+(n>>2*c-d-e+1)/a;
    }
    return a>>32 ? UINT32_MAX : (uint32_t)(a-(a*a>n));
#endif
}

// Computes the integer cube root of a number, rounded down.
uint32_t mathy_icbrt(uint64_t n) {
    // Bitwise algorithm
    uint32_t x = 0;
    for (uint32_t i = (uint32_t)1<<(mathy_ilog2(n)/3); i; i >>= 1) {
        uint64_t temp = x|i;
        if (temp*temp*temp <= n)
            x = temp;
    }
    return x;
}

/**
 * C++20 introduces the lerp function to the standard library. The goal is to
 * guarantee the basic properties of exactness, monotonicity, determinacy,
 * boundedness, and consistency, as explained in this paper.
 * https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0811r3.html
 * 
 * (1-t)*a+t*b
 * As discussed in the paper, this is not monotonic unless a*b<=0.
 * t<0.5 ? a+t*(b-a) : b-(1-t)*(b-a)
 * This is monotonic, according to https://math.stackexchange.com/q/907327
 * As discussed in the paper, this can overflow.
 * 
 * Note that the implementation discussed in the paper does not satisfy the
 * standard when one endpoint is zero and t is infinite, as the value returned
 * is NaN whereas it should be infinite.
 */

// Computes the linear interpolation between two numbers.
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

// Computes the greatest common denominator (GCD) of two numbers.
intmax_t mathy_gcd(intmax_t a, intmax_t b) {
    // Euclid's algorithm
    while (b != 0) { // a, b = b, a%b
        intmax_t temp = b;
        b = a%b;
        a = temp;
    }
    return abs(a);
}

// Computes the least common multiple (LCM) of two numbers.
intmax_t mathy_lcm(intmax_t a, intmax_t b) {
    if ((a|b) == 0) // if (a == 0 && b == 0)
        return 0; // avoid division by zero as gcd(a, b) will return 0
    a *= b/mathy_gcd(a, b); // limit overflow
    return abs(a);
}

// Computes the factorial of a number.
uintmax_t mathy_factorial(uint8_t n) {
    uintmax_t prod = 1;
    for (uint8_t i = n; i; i--)
        prod *= i;
    return prod;
}

// Computes the number of permutations.
uintmax_t mathy_permutation(uint8_t n, uint8_t k) {
    uintmax_t prod = 1;
    for (uint8_t i = k; i; i--)
        prod *= n+1-i;
    return prod;
}

// Computes the number of combinations.
uintmax_t mathy_combination(uint8_t n, uint8_t k) {
    uintmax_t prod = 1;
    for (uint8_t r = min(k, n-k), i = r; i; i--)
        prod = prod*(n+1-i)/(r+1-i); // limit overflow
    return prod;
}

/**
 * The Python 3.12 built-in sum uses Neumaier summation for floats. This is a
 * compensated summation that has improved accuracy over a naive summation.
 * This is slower than the NumPy np.sum pairwise summation, but faster than the
 * Python math.fsum Shewmuck summation. See the following GitHub issue for how
 * the various implementations perform for a badly conditioned sum.
 * https://github.com/numpy/numpy/issues/8786
 * 
 * Note that unsafe optimizations such as -ffast-math should be avoided.
 */

// Computes the sum of an array of numbers.
double mathy_stats_sum(const double *arr, size_t n) {
    // Neumaier summation
    double sum = 0;
    double c = 0;
    for (size_t i = 0; i < n; i++) {
        const double t = sum+arr[i];
        c += fabs(sum)>=fabs(arr[i]) ? (sum-t)+arr[i] : (arr[i]-t)+sum;
        sum = t;
    }
    return sum+c;
}

// Computes the dot product of two arrays of numbers.
static double _mathy_linalg_dot(const double *arr1, const double *arr2,
    size_t n) {
    // Neumaier summation
    double sum = 0;
    double c = 0;
    for (size_t i = 0; i < n; i++) {
        const double p = arr1[i]*arr2[i];
        const double t = sum+p;
        c += fabs(sum)>=fabs(p) ? (sum-t)+p : (p-t)+sum;
        sum = t;
    }
    return sum+c;
}

// Computes the weighted average of an array of numbers.
double mathy_stats_average(const double *arr1, const double *arr2, size_t n) {
    return _mathy_linalg_dot(arr1, arr2, n)/mathy_stats_sum(arr2, n);
}

// Computes the mean of an array of numbers.
double mathy_stats_mean(const double *arr, size_t n) {
    return mathy_stats_sum(arr, n)/n;
}

// Computes the standard deviation of an array of numbers.
double mathy_stats_std(const double *arr, size_t n) {
    return sqrt(mathy_stats_var(arr, n)); 
}

// Computes the sum of product of deviations about the means of two arrays
// of numbers.
static double _mathy_stats_cov_impl(const double *arr1, const double *arr2,
    size_t n, double mean1, double mean2) {
    // Neumaier summation
    double sum = 0;
    double c = 0;
    for (size_t i = 0; i < n; i++) {
        const double p = (arr1[i]-mean1)*(arr2[i]-mean2);
        const double t = sum+p;
        c += fabs(sum)>=fabs(p) ? (sum-t)+p : (p-t)+sum;
        sum = t;
    }
    return sum+c;
}

// Computes the variance of an array of numbers.
double mathy_stats_var(const double *arr, size_t n) {
    const double mean = mathy_stats_mean(arr, n);
    return _mathy_stats_cov_impl(arr, arr, n, mean, mean)/n;
}

// Computes the covariance of two arrays of numbers.
double mathy_stats_cov(const double *arr1, const double *arr2, size_t n) {
    const double mean1 = mathy_stats_mean(arr1, n);
    const double mean2 = mathy_stats_mean(arr2, n);
    return _mathy_stats_cov_impl(arr1, arr2, n, mean1, mean2)/n;
}

// Computes the Pearson correlation coefficient of two arrays of numbers.
double mathy_stats_corr(const double *arr1, const double *arr2, size_t n) {
    const double mean1 = mathy_stats_mean(arr1, n);
    const double mean2 = mathy_stats_mean(arr2, n);
    const double num = _mathy_stats_cov_impl(arr1, arr2, n, mean1, mean2);
    const double den = sqrt(_mathy_stats_cov_impl(arr1, arr1, n, mean1, mean1)*
        _mathy_stats_cov_impl(arr2, arr2, n, mean2, mean2));
    return num/den;
}

// Computes the probability mass function (PMF) of a binomial distribution.
double mathy_stats_binom_pmf(uint8_t k, uint8_t n, double p) {
    if (signbit(p) || p > 1)
        return NAN;
    if (k > n)
        return 0;
    double prod = 1;
    for (uint8_t i = 1, r = min(k, n-k); i <= r; i++)
        prod *= (double)(n+1-i)/i;
    return prod*pow(p, k)*pow(1-p, n-k); // pow(0, 0) returns 1
}

// Computes the cumulative density function (CDF) of a binomial distribution.
double mathy_stats_binom_cdf(uint8_t k, uint8_t n, double p) {
    if (signbit(p) || p > 1)
        return NAN;
    if (k >= n)
        return 1;
    double prod = 1;
    double sum = prod*pow(p, 0)*pow(1-p, n);
    for (uint8_t i = 1; i <= k; i++) {
        prod *= (double)(n+1-i)/i;
        sum += prod*pow(p, i)*pow(1-p, n-i);
    }
    return min(sum, 1); // clip to 1 if greater
}

// Computes the probability density function (PDF) of a normal distribution.
double mathy_stats_norm_pdf(double x, double mu, double sigma) {
    if (sigma <= 0)
        return NAN;
    const double z = (x-mu)/sigma;
    return 0.5*M_SQRT1_2*M_2_SQRTPI*exp(-0.5*z*z)/sigma;
}

// Computes the cumulative density function (CDF) of a normal distribution.
double mathy_stats_norm_cdf(double x, double mu, double sigma) {
    if (sigma <= 0)
        return NAN;
    const double z = (x-mu)/sigma;
    return 0.5*(1+erf(M_SQRT1_2*z));
}

// Computes the percent point function (PPF) of a normal distribution.
double mathy_stats_norm_ppf(double p, double mu, double sigma) {
    if (sigma <= 0 || signbit(p) || p > 1)
        return NAN;
    // uses the implementation of statistics.NormalDist.inv_cdf in Python
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
    r = signbit(q) ? p : 1-p;
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
        if (isinf(r)) // p is either 0 or 1
            num = INFINITY, den = 1; // return either -inf or +inf
        else {
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
    }
    x = num/den;
    x = signbit(q) ? -x : x;
    return mu+(x*sigma);
}
