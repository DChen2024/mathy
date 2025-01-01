# Documentation

You can check the version of Mathy with `MATHY_VERSION_MAJOR`,
`MATHY_VERSION_MINOR`, and `MATHY_VERSION_PATCH`.

[General](#general)
- **mathy_ilog2**: Computes the integer base-2 logarithm of a number,
rounded down.
- **mathy_isqrt**: Computes the integer square root of a number, rounded down.
- **mathy_icbrt**: Computes the integer cube root of a number, rounded down.
- **mathy_lerp**: Computes the linear interpolation between two numbers.
- **mathy_gcd**: Computes the greatest common denominator (GCD) of two numbers.
- **mathy_lcm**: Computes the least common multiple (LCM) of two numbers.
- **mathy_factorial**: Computes the factorial of a number.
- **mathy_permutation**: Computes the number of permutations.
- **mathy_combination**: Computes the number of combinations.

[Statistics](#statistics)
- **mathy_stats_sum**: Computes the sum of an array of numbers.
- **mathy_stats_average**: Computes the weighted average of an array of numbers.
- **mathy_stats_mean**: Computes the mean of an array of numbers.
- **mathy_stats_std**: Computes the standard deviation of an array of numbers.
- **mathy_stats_var**: Computes the variance of an array of numbers.
- **mathy_stats_cov**: Computes the covariance of two arrays of numbers.
- **mathy_stats_corr**: Computes the Pearson correlation coefficient of two
arrays of numbers.
- **mathy_stats_binom_pmf**: Computes the probability mass function (PMF)
of a binomial distribution.
- **mathy_stats_binom_cdf**: Computes the cumulative density function (CDF)
of a binomial distribution.
- **mathy_stats_norm_pdf**: Computes the probability density function (PDF)
of a normal distribution.
- **mathy_stats_norm_cdf**: Computes the cumulative density function (CDF)
of a normal distribution.
- **mathy_stats_norm_ppf**: Computes the percent point function (PPF)
of a normal distribution.

## General

```c
int8_t mathy_ilog2(uint64_t n);
```

Computes the integer base-2 logarithm of a number, rounded down.

### Parameters

`n` Unsigned integer value

### Returns

The integer base-2 logarithm of `n`.

If `n == 0`, returns `-1`.

### Note

This function is always accurate, whereas `ilogb` may suffer from
floating-point inaccuracies.

---

```c
uint32_t mathy_isqrt(uint64_t n);
```

Computes the integer square root of a number, rounded down.

### Parameters

`n` Unsigned integer value

### Returns

The integer square root of `n`.

### Note

This function is always accurate, whereas `(int)sqrt` may suffer from
floating-point inaccuracies.

---

```c
uint32_t mathy_icbrt(uint64_t n);
```

Computes the integer cube root of a number, rounded down.

### Parameters

`n` Unsigned integer value

### Returns

The integer cube root of `n`.

### Note

This function is always accurate, whereas `(int)cbrt` may suffer from
floating-point inaccuracies.

---

```c
float mathy_lerpf(float a, float b, float t);
double mathy_lerp(double a, double b, double t);
long double mathy_lerpl(long double a, long double b, long double t);
```

Computes the linear interpolation between two numbers.

This is also known as linear extrapolation if `t` is outside the range
`[0.0, 1.0]`.

### Parameters

`a`, `b`, `t` Floating-point value

### Returns

The interpolated value between `a` and `b` using parameter `t`.

### Warning

Passing a non-finite value to `a` or `b` may yield unexpected results.

### Note

If `isfinite(a) && isfinite(b)`, then:
- If `t == 0`, the value returned is equal to `a`.
- If `t == 1`, the value returned is equal to `b`.
- If `t >= 0 && t <= 1`, the value returned is finite.
- If `isfinite(t) && a == b`, the value returned is equal to `a` and `b`.
- If `isfinite(t) || isinf(t) && b-a != 0`, the value returned is not NaN.

Let `CMP(x, y)` be `1` if `x > y`, `-1` if `x < y`, and `0` otherwise.
For any `t1` and `t2`, the product of `CMP(mathy_lerp(a, b, t2),
mathy_lerp(a, b, t1))`, `CMP(t2, t1)`, and `CMP(b, a)` is non-negative.
In other words, this function is monotonic in `t`.

---

```c
intmax_t mathy_gcd(intmax_t a, intmax_t b);
```

Computes the greatest common denominator (GCD) of two numbers.

### Parameters

`a`, `b` Integer value

### Returns

The greatest common denominator of `abs(a)` and `abs(b)`.

### Warning

This function is accurate only if the result can be represented by `intmax_t`.

---

```c
intmax_t mathy_lcm(intmax_t a, intmax_t b);
```

Computes the least common multiple (LCM) of two numbers.

### Parameters

`a`, `b` Integer value

### Returns

The least common multiple of `abs(a)` and `abs(b)`.

### Warning

This function is accurate only if the result can be represented by `intmax_t`.

---

```c
uintmax_t mathy_factorial(uint8_t n);
```

Computes the factorial of a number.

### Parameters

`n` Unsigned integer value

### Returns

The factorial of `n`.

### Warning

This function is accurate only if the result can be represented by `uintmax_t`.
- For `uint64_t`, this function is always accurate if `n <= 20`.

---

```c
uintmax_t mathy_permutation(uint8_t n, uint8_t k);
```

Computes the number of permutations.

### Parameters

`n`, `k` Unsigned integer value

### Returns

The number of ways to arrange `k` items from `n` items without repetition
and with order.

### Warning

This function is accurate only if the result can be represented by `uintmax_t`.
- For `uint64_t`, this function is always accurate if `n <= 20`.

---

```c
uintmax_t mathy_combination(uint8_t n, uint8_t k);
```

Computes the number of combinations.

This is also known as the binomial coefficient.

### Parameters

`n`, `k` Unsigned integer value

### Returns

The number of ways to arrange `k` items from `n` items without repetition
and without order.

### Warning

This function may not be accurate for large values of `n`,
even if the result can be represented by `uintmax_t`.
- For `uint64_t`, this function is always accurate if `n <= 62`.

## Statistics

```c
double mathy_stats_sum(const double *arr, size_t n);
```

Computes the sum of an array of numbers.

### Parameters

`arr` Pointer to the array of numbers  
`n` Number of elements

### Returns

The sum of the numbers in the array.

### Warning

The value returned may be infinite or NaN due to intermediate overflow.

### Note

If `n == 0`, returns `0.0`.

---

```c
double mathy_stats_average(const double *arr1, const double *arr2, size_t n);
```

Computes the weighted average of an array of numbers.

### Parameters

`arr1` Pointer to the array of numbers  
`arr2` Pointer to an array of weights  
`n` Number of elements

### Returns

The weighted average of the numbers in the array.

### Warning

The value returned may be infinite or NaN due to intermediate overflow
or underflow, or if the sum of the weights is zero.

### Note

If `n == 0`, returns NaN.

---

```c
double mathy_stats_mean(const double *arr, size_t n);
```

Computes the mean of an array of numbers.

### Parameters

`arr` Pointer to the array of numbers  
`n` Number of elements

### Returns

The mean of the numbers in the array.

### Warning

The value returned may be infinite or NaN due to intermediate overflow.

### Note

If `n == 0`, returns NaN.

---

```c
double mathy_stats_std(const double *arr, size_t n);
```

Computes the standard deviation of an array of numbers.

If the array is a sample from the population, this is a biased estimate of the
population standard deviation.

### Parameters

`arr` Pointer to the array of numbers  
`n` Number of elements

### Returns

The standard deviation of the numbers in the array.

### Warning

The value returned may be infinite or NaN due to intermediate overflow.

### Note

If `n == 0`, returns NaN.

If the value returned is not NaN, the value returned is always non-negative.

---

```c
double mathy_stats_var(const double *arr, size_t n);
```

Computes the variance of an array of numbers.

If the array is a sample from the population, this is a biased estimate of the
population variance.

### Parameters

`arr` Pointer to the array of numbers  
`n` Number of elements

### Returns

The variance of the numbers in the array.

### Warning

The value returned may be infinite or NaN due to intermediate overflow.

### Note

If `n == 0`, returns NaN.

If the value returned is not NaN, the value returned is always non-negative.

---

```c
double mathy_stats_cov(const double *arr1, const double *arr2, size_t n);
```

Computes the covariance of two arrays of numbers.

If the array is a sample from the population, this is a biased estimate of the
population covariance.

### Parameters
`arr1`, `arr2` Pointer to the array of numbers  
`n` Number of elements

### Returns

The covariance of the numbers in the two arrays.

### Warning

The value returned may be infinite or NaN due to intermediate overflow.

### Note

If `n == 0`, returns NaN.

---

```c
double mathy_stats_corr(const double *arr1, const double *arr2, size_t n);
```

Computes the Pearson correlation coefficient of two arrays of numbers.

### Parameters
`arr1`, `arr2` Pointer to the array of numbers  
`n` Number of elements

### Returns

The correlation coefficient of the numbers in the two arrays.

### Warning

The value returned may be infinite or NaN due to intermediate overflow
or underflow, or if all numbers in either array are equal.

### Note

If `n == 0`, returns NaN.

If overflow or underflow do not occur, the value returned is always inside the
range `[-1.0, 1.0]`.

---

```c
double mathy_stats_binom_pmf(uint8_t k, uint8_t n, double p);
```

Computes the probability mass function (PMF) of a binomial distribution.

### Parameters

`k` Number of successes  
`n` Number of trials  
`p` Probability of success

### Returns

The probability the random variable takes on the value of `k`.

If `p` is outside the range `[0.0, 1.0]`, returns NaN.

### Note

If `p` is inside the range `[0.0, 1.0]`, the value returned is inside the range
`[0.0, 1.0]`.

If `n` is large, consider approximating the binomial distribution as a
normal distribution with mean `mu = n*p` and standard deviation
`sigma = sqrt(n*p*(1-p))`.

---

```c
double mathy_stats_binom_cdf(uint8_t k, uint8_t n, double p);
```

Computes the cumulative density function (CDF) of a binomial distribution.

### Parameters

`k` Number of successes  
`n` Number of trials  
`p` Probability of success

### Returns

The relative likelihood the random variable is less than or equal to `k`.

If `p` is outside the range `[0.0, 1.0]`, returns NaN.

### Note

If `p` is inside the range `[0.0, 1.0]`, the value returned is inside the range
`[0.0, 1.0]`.

If `n` is large, consider approximating the binomial distribution as a
normal distribution with mean `mu = n*p` and standard deviation
`sigma = sqrt(n*p*(1-p))`.

---

```c
double mathy_stats_norm_pdf(double x, double mu, double sigma);
```

Computes the probability density function (PDF) of a normal distribution.

### Parameters

`x` Sample value of the random variable  
`mu` Mean of the normal distribution  
`sigma` Standard deviation of the normal distribution

### Returns

The relative likelihood the random variable is close to `x`.

If `sigma <= 0`, returns NaN.

### Warning

Passing a non-finite value to `mu` or `sigma` may yield unexpected results.

### Note

If `sigma > 0` and `isfinite(mu) && isfinite(sigma)`, the value returned is
not NaN.

This function is monotonic in `k`.

---

```c
double mathy_stats_norm_cdf(double x, double mu, double sigma);
```

Computes the cumulative density function (CDF) of a normal distribution.

### Parameters

`x` Sample value of the random variable  
`mu` Mean of the normal distribution  
`sigma` Standard deviation of the normal distribution

### Returns

The probability the random variable is less than or equal to `x`.

If `sigma <= 0`, returns NaN.

### Warning

Passing a non-finite value to `mu` or `sigma` may yield unexpected results.

### Note

If `sigma > 0` and `isfinite(mu) && isfinite(sigma)`, the value returned is
inside the range `[0.0, 1.0]`.

This function is monotonic in `x`.

---

```c
double mathy_stats_norm_ppf(double p, double mu, double sigma);
```

Computes the percent point function (PPF) of a normal distribution.

This is also known as the quantile function.

### Parameters

`p` Probability of the lower tail  
`mu` Mean of the normal distribution  
`sigma` Standard deviation of the normal distribution

### Returns

The value of the random variable such that the probability of the
random variable is less than or equal to that value equals `p`.

If `sigma <= 0` or `p` is outside the range `[0.0, 1.0]`, returns NaN.

### Warning

Passing a non-finite value to `mu` or `sigma` may yield unexpected results.

### Note

If `sigma > 0` and `isfinite(mu) && isfinite(sigma)` and `p` is inside the
range `[0.0, 1.0]`, the value returned is not NaN.

This function is monotonic in `p` if `p` is inside the range `[0.0, 1.0]`.
