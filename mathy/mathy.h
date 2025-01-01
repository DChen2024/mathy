#ifndef MATHY_H
#define MATHY_H

#define MATHY_VERSION_MAJOR 1
#define MATHY_VERSION_MINOR 1
#define MATHY_VERSION_PATCH 0

#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#error "mathy requires at least C99"
#endif

#include <stdint.h>

#ifdef _WIN32
#ifdef MATHY_EXPORTS
#define MATHY_API __declspec(dllexport)
#else
#define MATHY_API __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Computes the integer base-2 logarithm of a number, rounded down.
 * 
 * @param   `n` Unsigned integer value
 * 
 * @return  The integer base-2 logarithm of `n`.
 * 
 *          If `n == 0`, returns `-1`.
 * 
 * @note    This function is always accurate, whereas `ilogb` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API int8_t mathy_ilog2(uint64_t n);

/**
 * @brief   Computes the integer square root of a number, rounded down.
 * 
 * @param   `n` Unsigned integer value
 * 
 * @return  The integer square root of `n`.
 * 
 * @note    This function is always accurate, whereas `(int)sqrt` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API uint32_t mathy_isqrt(uint64_t n);

/**
 * @brief   Computes the integer cube root of a number, rounded down.
 * 
 * @param   `n` Unsigned integer value
 * 
 * @return  The integer cube root of `n`.
 * 
 * @note    This function is always accurate, whereas `(int)cbrt` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API uint32_t mathy_icbrt(uint64_t n);

/** 
 * @brief   Computes the linear interpolation between two numbers.
 * 
 *          This is also known as linear extrapolation if `t` is outside
 *          the range `[0.0, 1.0]`.
 * 
 * @param   `a`, `b`, `t` Floating-point value
 * 
 * @return  The interpolated value between `a` and `b` using parameter `t`.
 * 
 * @warning Passing a non-finite value to `a` or `b` may yield unexpected
 *          results.
 * 
 * @note    If `isfinite(a) && isfinite(b)`, the following properties are
 *          guaranteed:
 * 
 *            - If `t == 0`, the value returned is equal to `a`.
 * 
 *            - If `t == 1`, the value returned is equal to `b`.
 * 
 *            - If `t >= 0 && t <= 1`, the value returned is finite.
 * 
 *            - If `isfinite(t) && a == b`, the value returned is equal to
 *              `a` and `b`.
 * 
 *            - If `isfinite(t) || isinf(t) && b-a != 0`, the value returned
 *              is not NaN.
 * 
 *          Let `CMP(x, y)` be `1` if `x > y`, `-1` if `x < y`, and `0`
 *          otherwise. For any `t1` and `t2`, the product of `CMP(mathy_lerp(a,
 *          b, t2), mathy_lerp(a, b, t1))`, `CMP(t2, t1)`, and `CMP(b, a)` is
 *          non-negative. In other words, this function is monotonic in `t`.
 */
MATHY_API float mathy_lerpf(float a, float b, float t);
MATHY_API double mathy_lerp(double a, double b, double t);
MATHY_API long double mathy_lerpl(long double a, long double b, long double t);

/**
 * @brief   Computes the greatest common denominator (GCD) of two numbers.
 * 
 * @param   `a`, `b` Integer value
 * 
 * @return  The greatest common denominator of `abs(a)` and `abs(b)`.
 * 
 * @warning This function is accurate only if the result can be represented
 *          by `intmax_t`.
 */
MATHY_API intmax_t mathy_gcd(intmax_t a, intmax_t b);

/**
 * @brief   Computes the least common multiple (LCM) of two numbers.
 * 
 * @param   `a`, `b` Integer value
 * 
 * @return  The least common multiple of `abs(a)` and `abs(b)`.
 * 
 * @warning This function is accurate only if the result can be represented
 *          by `intmax_t`.
 */
MATHY_API intmax_t mathy_lcm(intmax_t a, intmax_t b);

/**
 * @brief   Computes the factorial of a number.
 * 
 * @param   `n` Unsigned integer value
 * 
 * @return  The factorial of `n`.
 * 
 * @warning This function is accurate only if the result can be represented
 *          by `uintmax_t`.
 * 
 *            - For `uint64_t`, this function is always accurate if `n <= 20`.
 */
MATHY_API uintmax_t mathy_factorial(uint8_t n);

/**
 * @brief   Computes the number of permutations.
 * 
 * @param   `n`, `k` Unsigned integer value
 * 
 * @return  The number of ways to arrange `k` items from `n` items
 *          without repetition and with order.
 * 
 * @warning This function is accurate only if the result can be represented
 *          by `uintmax_t`.
 * 
 *            - For `uint64_t`, this function is always accurate if `n <= 20`.
 */
MATHY_API uintmax_t mathy_permutation(uint8_t n, uint8_t k);

/**
 * @brief   Computes the number of combinations.
 * 
 *          This is also known as the binomial coefficient.
 * 
 * @param   `n`, `k` Unsigned integer value
 * 
 * @return  The number of ways to arrange `k` items from `n` items
 *          without repetition and without order.
 * 
 * @warning This function may not be accurate for large values of `n`,
 *          even if the result can be represented by `uintmax_t`.
 * 
 *            - For `uint64_t`, this function is always accurate if `n <= 62`.
 */
MATHY_API uintmax_t mathy_combination(uint8_t n, uint8_t k);

/**
 * @brief   Computes the sum of an array of numbers.
 * 
 * @param   `arr` Pointer to the array of numbers
 * @param   `n` Number of elements
 * 
 * @return  The sum of the numbers in the array.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow.
 * 
 * @note    If `n == 0`, returns `0.0`.
 */
MATHY_API double mathy_stats_sum(const double *arr, size_t n);

/**
 * @brief   Computes the weighted average of an array of numbers.
 * 
 * @param   `arr1` Pointer to the array of numbers
 * @param   `arr2` Pointer to an array of weights
 * @param   `n` Number of elements
 * 
 * @return  The weighted average of the numbers in the array.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow or underflow, or if the sum of the weights is zero.
 * 
 * @note    If `n == 0`, returns NaN.
 */
MATHY_API double mathy_stats_average(const double *arr1, const double *arr2,
    size_t n);

/**
 * @brief   Computes the mean of an array of numbers.
 * 
 * @param   `arr` Pointer to the array of numbers
 * @param   `n` Number of elements
 * 
 * @return  The mean of the numbers in the array.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow.
 * 
 * @note    If `n == 0`, returns NaN.
 */
MATHY_API double mathy_stats_mean(const double *arr, size_t n);

/**
 * @brief   Computes the standard deviation of an array of numbers.
 * 
 *          If the array is a sample from the population, this is a biased
 *          estimate of the population standard deviation.
 * 
 * @param   `arr` Pointer to the array of numbers
 * @param   `n` Number of elements
 * 
 * @return  The standard deviation of the numbers in the array.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow.
 * 
 * @note    If `n == 0`, returns NaN.
 * 
 *          If the value returned is not NaN, the value returned is always
 *          non-negative.
 */
MATHY_API double mathy_stats_std(const double *arr, size_t n);

/**
 * @brief   Computes the variance of an array of numbers.
 * 
 *          If the array is a sample from the population, this is a biased
 *          estimate of the population variance.
 * 
 * @param   `arr` Pointer to the array of numbers
 * @param   `n` Number of elements
 * 
 * @return  The variance of the numbers in the array.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow.
 * 
 * @note    If `n == 0`, returns NaN.
 * 
 *          If the value returned is not NaN, the value returned is always
 *          non-negative.
 */
MATHY_API double mathy_stats_var(const double *arr, size_t n);

/**
 * @brief   Computes the covariance of two arrays of numbers.
 * 
 *          If the array is a sample from the population, this is a biased
 *          estimate of the population covariance.
 * 
 * @param   `arr1`, `arr2` Pointer to the array of numbers
 * @param   `n` Number of elements
 * 
 * @return  The covariance of the numbers in the two arrays.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow.
 * 
 * @note    If `n == 0`, returns NaN.
 */
MATHY_API double mathy_stats_cov(const double *arr1, const double *arr2,
    size_t n);

/**
 * @brief   Computes the Pearson correlation coefficient of two arrays
 *          of numbers.
 * 
 * @param   `arr1`, `arr2` Pointer to the array of numbers
 * @param   `n` Number of elements
 * 
 * @return  The correlation coefficient of the numbers in the two arrays.
 * 
 * @warning The value returned may be infinite or NaN due to intermediate
 *          overflow or underflow, or if all numbers in either array
 *          are equal.
 * 
 * @note    If `n == 0`, returns NaN.
 * 
 *          If overflow or underflow do not occur, the value returned is always
 *          inside the range `[-1.0, 1.0]`.
 */
MATHY_API double mathy_stats_corr(const double *arr1, const double *arr2,
    size_t n);

/**
 * @brief   Computes the probability mass function (PMF) of a
 *          binomial distribution.
 * 
 * @param   `k` Number of successes
 * @param   `n` Number of trials
 * @param   `p` Probability of success
 * 
 * @return  The probability the random variable takes on the value of `k`.
 * 
 *          If `p` is outside the range `[0.0, 1.0]`, returns NaN.
 * 
 * @note    If `p` is inside the range `[0.0, 1.0]`, the value returned is
 *          inside the range `[0.0, 1.0]`.
 * 
 *          If `n` is large, consider approximating the binomial distribution
 *          as a normal distribution with mean `mu = n*p` and standard
 *          deviation `sigma = sqrt(n*p*(1-p))`.
 */
MATHY_API double mathy_stats_binom_pmf(uint8_t k, uint8_t n, double p);

/**
 * @brief   Computes the cumulative density function (CDF) of a
 *          binomial distribution.
 * 
 * @param   `k` Number of successes
 * @param   `n` Number of trials
 * @param   `p` Probability of success
 * 
 * @return  The probability the random variable is less than or equal to `k`.
 * 
 *          If `p` is outside the range `[0.0, 1.0]`, returns NaN.
 * 
 * @note    If `p` is inside the range `[0.0, 1.0]`, the value returned is
 *          inside the range `[0.0, 1.0]`.
 * 
 *          If `n` is large, consider approximating the binomial distribution
 *          as a normal distribution with mean `mu = n*p` and standard
 *          deviation `sigma = sqrt(n*p*(1-p))`.
 * 
 *          This function is monotonic in `k`.
 */
MATHY_API double mathy_stats_binom_cdf(uint8_t k, uint8_t n, double p);

/**
 * @brief   Computes the probability density function (PDF) of a
 *          normal distribution.
 * 
 * @param   `x` Sample value of the random variable
 * @param   `mu` Mean of the normal distribution
 * @param   `sigma` Standard deviation of the normal distribution
 * 
 * @return  The relative likelihood the random variable is close to `x`.
 * 
 *          If `sigma <= 0`, returns NaN.
 * 
 * @warning Passing a non-finite value to `mu` or `sigma` may yield
 *          unexpected results.
 * 
 * @note    If `sigma > 0` and `isfinite(mu) && isfinite(sigma)`, the return
 *          value is not NaN.
 */
MATHY_API double mathy_stats_norm_pdf(double x, double mu, double sigma);

/**
 * @brief   Computes the cumulative density function (CDF) of a
 *          normal distribution.
 * 
 * @param   `x` Sample value of the random variable
 * @param   `mu` Mean of the normal distribution
 * @param   `sigma` Standard deviation of the normal distribution
 * 
 * @return  The probability the random variable is less than or equal to `x`.
 * 
 *          If `sigma <= 0`, returns NaN.
 * 
 * @warning Passing a non-finite value to `mu` or `sigma` may yield
 *          unexpected results.
 * 
 * @note    If `sigma > 0` and `isfinite(mu) && isfinite(sigma)`, the return
 *          value is inside the range `[0.0, 1.0]`.
 * 
 *          This function is monotonic in `x`.
 */
MATHY_API double mathy_stats_norm_cdf(double x, double mu, double sigma);

/**
 * @brief   Computes the percent point function (PPF) of a
 *          normal distribution.
 * 
 *          This is also known as the quantile funciton.
 * 
 * @param   `p` Probability of the lower tail
 * @param   `mu` Mean of the normal distribution
 * @param   `sigma` Standard deviation of the normal distribution
 * 
 * @return  The value of the random variable such that the probability of the
 *          random variable is less than or equal to that value equals `p`.
 * 
 *          If `sigma <= 0` or `p` is outside the range `[0.0, 1.0]`,
 *          returns NaN.
 * 
 * @warning Passing a non-finite value to `mu` or `sigma` may yield
 *          unexpected results.
 * 
 * @note    If `sigma > 0` and `isfinite(mu) && isfinite(sigma)` and `p` is
 *          inside the range `[0.0, 1.0]`, the value returned is not NaN.
 * 
 *          This function is monotonic in `p` if `p` is inside the range
 *          `[0.0, 1.0]`.
 */
MATHY_API double mathy_stats_norm_ppf(double p, double mu, double sigma);

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#undef MATHY_API
#endif

#endif // MATHY_H
