# Documentation

You can check the version of mathy with `MATHY_VERSION`, where the
version information is encoded as follows:
- Bits 15-12: Major version
- Bits 11-8: Minor version
- Bits 7-4: Patch version
- Bits 3-0: Currently unused

---

```c
int8_t ilog2(uint64_t n);
```

Calculate the integer base-2 logarithm of a number, rounded down.

### Parameters
`n` Unsigned integer value

### Returns
The integer base-2 logarithm of `n`. If `n == 0`, returns `-1`.

### Warning
Passing a negative signed integer might yield unexpected results due to
implicit conversion.

### Note
This function is always accurate, whereas `ilogb` might suffer from
floating-point inaccuracies.

---

```c
uint32_t isqrt(uint64_t n);
```

Calculate the integer square root of a number, rounded down.

### Parameters
`n` Unsigned integer value

### Returns
The integer square root of `n`.

### Warning
Passing a negative signed integer might yield unexpected results due to
implicit conversion.

### Note
This function is always accurate, whereas `(int)sqrt` might suffer from
floating-point inaccuracies.

---

```cpp
uint32_t icbrt(uint64_t n);
```

Calculate the integer cube root of a number, rounded down.

### Parameters
`n` Unsigned integer value

### Returns
The integer cube root of `n`.

### Warning
Passing a negative signed integer might yield unexpected results due to
implicit conversion.

### Note
This function is always accurate, whereas `(int)cbrt` might suffer from
floating-point inaccuracies.

---

```c
intmax_t gcd(intmax_t a, intmax_t b);
```

Calculate the greatest common denominator (GCD) of two numbers.

### Parameters
`a, b` Integer value

### Returns
The greatest common denominator of `abs(a)` and `abs(b)`.
If both `a` and `b` are `0`, returns `0`.

### Warning
This function is only accurate if the result is within the range of `intmax_t`.

---

```c
intmax_t lcm(intmax_t a, intmax_t b);
```

Calculate the least common multiple (LCM) of two numbers.

### Parameters
`a, b` Integer value

### Returns
The least common multiple of `abs(a)` and `abs(b)`.
If both `a` and `b` are `0`, returns `0`.

### Warning
This function is only accurate if the result is within the range of `intmax_t`.

---

```c
bool prime(uint64_t n);
```

Check whether a number is prime.

### Parameters
`n` Unsigned integer value

### Returns
Whether `n` is prime.

### Warning
Passing a negative signed integer might yield unexpected results due to
implicit conversion.

---

```c
bool quadraticf(float a, float b, float c,
                float *x1, float *x2);
bool quadratic(double a, double b, double c,
               double *x1, double *x2);
bool quadraticl(long double a, long double b, long double c,
                long double *x1, long double *x2);
```

Calculate the roots of a quadratic.

### Parameters
`a, b, c` Coefficient according to the quadratic formula  
`x1, x2` Pointer to floating-point value to store the roots to

### Returns
Whether the roots are real.

If the roots are real, the roots are sorted with `x1 <= x2`.
Otherwise, `x1` is the real part and `x2` is the imaginary part.

### Warning
Passing a non-finite value to `a`, `b`, or `c` might yield unexpected results.

Passing `0` to `a` might yield unexpected results.

Passing `NULL` to `x1` or `x2` will result in undefined behavior.

---

```c
float lerpf(float a, float b, float t);
double lerp(double a, double b, double t);
long double lerpl(long double a, long double b, long double t);
```

Calculate the linear interpolation between two numbers.

### Parameters
`a, b` Floating-point value  
`t` Interpolation factor

### Returns
The interpolated value.

### Note

Let `r` be the value returned. If `isfinite(a) && isfinite(b)`,
the following properties are guaranteed:
- If `t == 0`, then `r == a`.
- If `t == 1`, then `r == b`.
- If `t >= 0 && t <= 1`, then `isfinite(r)`.
- If `isfinite(t) && a == b`, then `r == a`.
- If `isfinite(t) || !isnan(t) && b-a != 0`, then `!isnan(r)`.

Let `CMP(x, y)` be `1` if `x > y`, `-1` if `x < y`, and `0` otherwise.
For any `t1` and `t2`, the product of `CMP(lerp(a, b, t2), lerp(a, b, t1))`,
`CMP(t2, t1)`, and `CMP(b, a)` is non-negative.
In other words, `lerp` is monotonic.

---

```c
uintmax_t factorial(uint8_t n);
```

Calculate the factorial of a number.

### Parameters
`n` Unsigned integer value

### Returns
The factorial of `n`.

### Warning
Passing a negative signed integer might yield unexpected results due to
implicit conversion.

This function is only accurate if the result is within the range
of `uintmax_t`.
- For `uint64_t`, this function is always accurate if `n <= 20`.

---

```c
uintmax_t permutation(uint8_t n, uint8_t k);
```

Calculate the number of permutations.

### Parameters
`n` Number of items to select from  
`k` Number of items to select

### Returns
The number of ways to arrange `k` items from `n` items without repetition
and with order. If `k > n`, returns `0`.

### Warning
Passing a negative signed integer to `n` or `k` might yield unexpected results
due to implicit conversion.

This function is only accurate if the result is within the range
of `uintmax_t`.
- For `uint64_t`, this function is always accurate if `n <= 20`.

---

```c
uintmax_t combination(uint8_t n, uint8_t k);
```

Calculate the number of combinations.

This is also known as the binomial coefficient.

### Parameters
`n` Number of items to select from  
`k` Number of items to select

### Returns
The number of ways to arrange `k` items from `n` items without repetition
and without order. If `k > n`, returns `0`.

### Warning
Passing a negative signed integer to `n` or `k` might yield unexpected results
due to implicit conversion.

This function might not be accurate for large values of `n`,
even if the result is within the range of `uintmax_t`.
- For `uint64_t`, this function is always accurate if `n <= 62`.

---

```c
double binom_pmf(uint8_t k, uint8_t n, double p);
```

Calculate the probability mass function (PMF) of a binomial distribution.

### Parameters
`k` Number of successes  
`n` Number of trials  
`p` Probability of success

### Returns
The relative likelihood the random variable will take on the value of `k`.
If `p` is outside the range `[0.0, 1.0]`, returns `nan`. Otherwise, the value
returned is within the range `[0.0, 1.0]`.

### Warning
Passing a negative signed integer to `n` or `k` might yield unexpected results
due to implicit conversion.

### Note
If `n` is large, consider approximating the binomial distribution as a
normal distribution with mean `mu = n*p` and standard deviation
`sigma = sqrt(n*p*(1-p))`.

---

```c
double binom_cdf(uint8_t k, uint8_t n, double p);
```

Calculate the cumulative density function (CDF) of a binomial distribution.

### Parameters
`k` Number of successes  
`n` Number of trials  
`p` Probability of success

### Returns
The relative likelihood the random variable will less than or equal to `k`.
If `p` is outside the range `[0.0, 1.0]`, returns `nan`. Otherwise, the value
returned is within the range `[0.0, 1.0]`.

### Warning
Passing a negative signed integer to `n` or `k` might yield unexpected results
due to implicit conversion.

### Note
If `n` is large, consider approximating the binomial distribution as a
normal distribution with mean `mu = n*p` and standard deviation
`sigma = sqrt(n*p*(1-p))`.

---

```c
double normal_pdf(double x, double mu, double sigma);
```

Calculate the probability density function (PDF) of a normal distribution.

### Parameters
`x` Point at which to evaluate the PDF  
`mu` Mean of the normal distribution  
`sigma` Standard deviation of the normal distribution

### Returns
The relative likelihood the random variable will be near the value of `x`.
If `sigma <= 0`, returns `nan`. Otherwise, the value returned is within the
range `[0.0, 1.0]`.

---

```c
double normal_cdf(double x, double mu, double sigma);
```

Calculate the cumulative density function (CDF) of a normal distribution.

### Parameters
`x` Point at which to evaluate the CDF  
`mu` Mean of the normal distribution  
`sigma` Standard deviation of the normal distribution

### Returns
The probability the random variable will be less than or equal to `x`.
If `sigma <= 0`, returns `nan`. Otherwise, the value returned is within the
range `[0.0, 1.0]`.

---

```c
double normal_inv_cdf(double p, double mu, double sigma);
```

Calculate the inverse cumulative density function (CDF) of a
normal distribution.

This is also known as the quantile function.

### Parameters
`p` Point at which to evaluate the inverse CDF  
`mu` Mean of the normal distribution  
`sigma` Standard deviation of the normal distribution

### Returns
The value of the random variable such that the probability of the
random variable is less than or equal to `p`.
If `sigma <= 0` or if `p` is outside the range `[0.0, 1.0]`, returns `nan`.
