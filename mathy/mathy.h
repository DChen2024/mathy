#ifndef MATHY_H
#define MATHY_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#error "mathy requires C99 or later"
#endif

#define MATHY_VERSION_MAJOR 0
#define MATHY_VERSION_MINOR 1
#define MATHY_VERSION_PATCH 0

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
 *          This is also `k-1` where `k` is the bit width of `n`.
 * 
 * @param   `n` Unsigned integer value
 * 
 * @return  The integer base-2 logarithm of `n`.
 * 
 *          If `n == 0`, returns `-1`.
 * 
 * @warning This function accepts `uint64_t`, and passing values outside
 *          this range may yield unexpected results.
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
 * @warning This function accepts `uint64_t`, and passing values outside
 *          this range may yield unexpected results.
 * 
 *          This function returns `uint32_t`, and in certain arithmetic
 *          expressions may yield unexpected results.
 * 
 * @note    This function is always accurate, whereas `(int)sqrt` may suffer
 *          from floating-point inaccuracies.
 */
MATHY_API uint32_t mathy_isqrt(uint64_t n);

/**
 * @brief   Computes the greatest common denominator (GCD) of two numbers.
 * 
 * @param   `a`, `b` Unsigned integer value
 * 
 * @return  The greatest common denominator of `a` and `b`.
 * 
 *          If `a == 0 && b == 0`, returns `0`.
 * 
 * @warning This function accepts `uintmax_t`, and passing values outside
 *          this range may yield unexpected results.
 * 
 *          This function returns `uintmax_t`, and in certain arithmetic
 *          expressions may yield unexpected results.
 */
MATHY_API uintmax_t mathy_gcd(uintmax_t a, uintmax_t b);

/**
 * @brief   Computes the least common multiple (LCM) of two numbers.
 * 
 * @param   `a`, `b` Unsigned integer value
 * 
 * @return  The least common multiple of `a` and `b`.
 * 
 *          If `a == 0 || b == 0`, returns `0`.
 * 
 * @warning This function is accurate only if the result can be represented
 *          by `uintmax_t`.
 * 
 *          This function accepts `uintmax_t`, and passing values outside
 *          this range may yield unexpected results.
 * 
 *          This function returns `uintmax_t`, and in certain arithmetic
 *          expressions may yield unexpected results.
 */
MATHY_API uintmax_t mathy_lcm(uintmax_t a, uintmax_t b);

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
 * 
 *          This function accepts `uint8_t`, and passing values outside
 *          this range may yield unexpected results.
 * 
 *          This function returns `uintmax_t`, and in certain arithmetic
 *          expressions may yield unexpected results.
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
 * 
 *          This function accepts `uint8_t`, and passing values outside
 *          this range may yield unexpected results.
 * 
 *          This function returns `uintmax_t`, and in certain arithmetic
 *          expressions may yield unexpected results.
 */
MATHY_API uintmax_t mathy_combination(uint8_t n, uint8_t k);

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
 * @brief   Computes the percent point function (PPF) of the standard
 *          normal distribution.
 * 
 * @param   `p` Floating-point value
 * 
 * @return  The percent point function of `p`.
 * 
 *          If `p` is outside the range `[0.0, 1.0]`, returns NaN.
 * 
 * @note    If `p == 0`, the value returned is equal to negative infinity.
 * 
 *          If `p == 1`, the value returned is equal to positive infinity.
 */
MATHY_API float mathy_norminvf(float p);
MATHY_API double mathy_norminv(double p);

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#undef MATHY_API
#endif

#endif /* MATHY_H */
