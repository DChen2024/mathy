#ifndef MATHY_H
#define MATHY_H

#ifdef __STDC_VERSION__
#if __STDC_VERSION__ < 199901L
#error "mathy.h requires at least C99 as it uses stdbool.h and stdint.h"
#endif
#include <stdbool.h>
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
 * Encodes version information as follows:
 * - Bits 15-12: Major version
 * - Bits 11-8: Minor version
 * - Bits 7-4: Patch version
 * - Bits 3-0: Currently unused
 */
extern const uint16_t MATHY_VERSION;

/**
 * @brief   Calculate the integer base-2 logarithm of a number, rounded down.
 * 
 * @param   n Unsigned integer value
 * 
 * @return  The integer base-2 logarithm of `n`. If `n == 0`, returns `-1`.
 * 
 * @warning Passing a negative signed integer may yield unexpected results
 *          due to implicit conversion.
 * 
 * @note    This function is always accurate, whereas `ilogb` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API int8_t ilog2(uint64_t n);

/**
 * @brief   Calculate the integer square root of a number, rounded down.
 * 
 * @param   n Unsigned integer value
 * 
 * @return  The integer square root of `n`.
 * 
 * @warning Passing a negative signed integer may yield unexpected results
 *          due to implicit conversion.
 * 
 * @note    This function is always accurate, whereas `(int)sqrt` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API uint32_t isqrt(uint64_t n);

/**
 * @brief   Calculate the integer cube root of a number, rounded down.
 * 
 * @param   n Unsigned integer value
 * 
 * @return  The integer cube root of `n`.
 * 
 * @warning Passing a negative signed integer may yield unexpected results
 *          due to implicit conversion.
 * 
 * @note    This function is always accurate, whereas `(int)cbrt` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API uint32_t icbrt(uint64_t n);

/**
 * @brief   Calculate the greatest common denominator of two numbers.
 * 
 * @param   a, b Integer value
 * 
 * @return  The greatest common denominator of `abs(a)` and `abs(b)`.
 *          If both `a` and `b` are `0`, returns `0`.
 * 
 * @warning This function is only accurate if the result is within the range
 *          of `intmax_t`.
 */
MATHY_API intmax_t gcd(intmax_t a, intmax_t b);

/**
 * @brief   Calculate the least common multiple of two numbers.
 * 
 * @param   a, b Integer value
 * 
 * @return  The least common multiple of `abs(a)` and `abs(b)`.
 * 
 * @warning This function is only accurate if the result is within the range
 *          of `intmax_t`.
 */
MATHY_API intmax_t lcm(intmax_t a, intmax_t b);

/**
 * @brief   Check if a number is prime.
 * 
 * @param   n Unsigned integer value
 * 
 * @return  Whether `n` is prime.
 * 
 * @warning Passing a negative signed integer may yield unexpected results
 *          due to implicit conversion.
 */
MATHY_API bool prime(uint64_t n);

/**
 * @brief   Calculate the roots of a quadratic.
 * 
 * @param   a, b, c Coefficient according to the quadratic formula
 * @param   x1, x2 Pointer to floating-point value to store the roots to
 * 
 * @return  Whether the roots are real.
 * 
 *          If the roots are real, the roots are sorted with `x1 <= x2`.
 *          Otherwise, `x1` is the real part and `x2` is the imaginary part.
 * 
 * @warning Passing a non-finite value to `a`, `b`, or `c` may yield
 *          unexpected results.
 * 
 *          Passing `0` to `a` may yield unexpected results.
 * 
 *          Passing `NULL` to `x1` or `x2` will result in undefined behavior.
 */
MATHY_API bool quadraticf(float a, float b, float c,
                          float *x1, float *x2);
MATHY_API bool quadratic(double a, double b, double c,
                         double *x1, double *x2);
MATHY_API bool quadraticl(long double a, long double b, long double c,
                          long double *x1, long double *x2);

/** 
 * @brief   Calculate the linear interpolation between two numbers.
 * 
 * @param   a, b Floating-point value
 * @param   t Interpolation factor
 * 
 * @return  The interpolated value.
 * 
 * @note    Let `r` be the value returned. If `isfinite(a) && isfinite(b)`,
 *          the following properties are guaranteed:
 * 
 *            - If `t == 0`, then `r == a`.
 * 
 *            - If `t == 1`, then `r == b`.
 * 
 *            - If `t >= 0 && t <= 1`, then `isfinite(r)`.
 * 
 *            - If `isfinite(t) && a == b`, then `r == a`.
 * 
 *            - If `isfinite(t) || !isnan(t) && b-a != 0`, then `!isnan(r)`.
 * 
 *          Let `CMP(x, y)` be `1` if `x > y`, `-1` if `x < y`, and `0`
 *          otherwise. For any `t1` and `t2`, the product of `CMP(lerp(a, b,
 *          t2), lerp(a, b, t1))`, `CMP(t2, t1)`, and `CMP(b, a)` is
 *          non-negative. In other words, `lerp` is monotonic.
 */
MATHY_API float lerpf(float a, float b, float t);
MATHY_API double lerp(double a, double b, double t);
MATHY_API long double lerpl(long double a, long double b, long double t);

/**
 * @brief   Calculate the factorial of a number.
 * 
 * @param   n Unsigned integer value
 * 
 * @return  The factorial of `n`.
 * 
 * @warning Passing a negative signed integer may yield unexpected results
 *          due to implicit conversion.
 * 
 *          This function is only accurate if the result is within the range
 *          of `uintmax_t`.
 * 
 *            - For `uint64_t`, this function is always accurate if `n <= 20`.
 */
MATHY_API uintmax_t factorial(uint8_t n);

/**
 * @brief   Calculate the number of permutations.
 * 
 * @param   n Number of items to select from
 * @param   k Number of items to select
 * 
 * @return  The number of ways to arrange `k` items from `n` items
 *          without repetition and with order. If `k > n`, returns `0`.
 * 
 * @warning Passing a negative signed integer to `n` or `k` may yield
 *          unexpected results due to implicit conversion.
 * 
 *          This function is only accurate if the result is within the range
 *          of `uintmax_t`.
 * 
 *            - For `uint64_t`, this function is always accurate if `n <= 20`.
 */
MATHY_API uintmax_t permutation(uint8_t n, uint8_t k);

/**
 * @brief   Calculate the number of combinations.
 * 
 *          This is also known as the binomial coefficient.
 * 
 * @param   n Number of items to select from
 * @param   k Number of items to select
 * 
 * @return  The number of ways to arrange `k` items from `n` items
 *          without repetition and without order. If `k > n`, returns `0`.
 * 
 * @warning Passing a negative signed integer to `n` or `k` may yield
 *          unexpected results due to implicit conversion.
 * 
 *          This function is only accurate if the result is within the range
 *          of `uintmax_t`.
 * 
 *            - For `uint64_t`, this function is always accurate if `n <= 62`.
 */
MATHY_API uintmax_t combination(uint8_t n, uint8_t k);

/**
 * @brief   Calculate the probability mass function (PMF) of a
 *          binomial distribution.
 * 
 * @param   k Number of successes
 * @param   n Number of trials
 * @param   p Probability of success
 * 
 * @return  The relative likelihood the random variable will take on the value
 *          of `k`. If `p` is outside the range `[0.0, 1.0]`, returns `nan`.
 * 
 * @warning Passing a negative signed integer to `n` or `k` may yield
 *          unexpected results due to implicit conversion.
 * 
 *          This function may not be accurate for large values of `n`.
 * 
 * @note    If `n` is large, consider approximating the binomial distribution
 *          as a normal distribution with mean `mu = n*p` and standard
 *          deviation `sigma = sqrt(n*p*(1-p))`.
 */
MATHY_API double binom_pmf(uint8_t k, uint8_t n, double p);

/**
 * @brief   Calculate the cumulative density function (CDF) of a
 *          binomial distribution.
 * 
 * @param   k Number of successes
 * @param   n Number of trials
 * @param   p Probability of success
 * 
 * @return  The probability the random variable will be less than or equal
 *          to `k`. If `p` is outside the range `[0.0, 1.0]`, returns `nan`.
 * 
 * @warning Passing a negative signed integer to `n` or `k` may yield
 *          unexpected results due to implicit conversion.
 * 
 *          This function may not be accurate for large values of `n`.
 * 
 * @note    If `n` is large, consider approximating the binomial distribution
 *          as a normal distribution with mean `mu = n*p` and standard
 *          deviation `sigma = sqrt(n*p*(1-p))`.
 */
MATHY_API double binom_cdf(uint8_t k, uint8_t n, double p);

/**
 * @brief   Calculate the probability density function (PDF) of a
 *          normal distribution.
 * 
 *          \f[
 *          f(x)=\frac{1}{\sigma\sqrt{2\pi}}\exp
 *          \left(-\frac{1}{2}\left(\frac{x-\mu}{\sigma}\right)^2\right)
 *          \f]
 * 
 * @param   x Point at which to evaluate the PDF
 * @param   mu Mean of the normal distribution
 * @param   sigma Standard deviation of the normal distribution
 * 
 * @return  The relative likelihood the random variable will be near the
 *          value of `x`. If `sigma <= 0`, returns `nan`.
 */
MATHY_API double normal_pdf(double x, double mu, double sigma);

/**
 * @brief   Calculate the cumulative density function (CDF) of a
 *          normal distribution.
 * 
 * @param   x Point at which to evaluate the CDF
 * @param   mu Mean of the normal distribution
 * @param   sigma Standard deviation of the normal distribution
 * 
 * @return  The probability the random variable will be less than or
 *          equal to `x`. If `sigma <= 0`, returns `nan`.
 */
MATHY_API double normal_cdf(double x, double mu, double sigma);

/**
 * @brief   Calculate the inverse cumulative density function (CDF) of a
 *          normal distribution.
 * 
 *          This is also known as the quantile funciton.
 * 
 * @param   p Point at which to evaluate the inverse CDF
 * @param   mu Mean of the normal distribution
 * @param   sigma Standard deviation of the normal distribution
 * 
 * @return  The value of the random variable such that the probability of the
 *          random variable is less than or equal to `p`. If `sigma <= 0` or
 *          if `p` is outside the range `[0.0, 1.0]`, returns `nan`.
 */
MATHY_API double normal_inv_cdf(double p, double mu, double sigma);

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#undef MATHY_API
#endif

#endif // MATHY_H
