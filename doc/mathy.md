# Documentation

You can check the version of Mathy with `MATHY_VERSION_MAJOR`,
`MATHY_VERSION_MINOR`, and `MATHY_VERSION_PATCH`.

- **mathy_ilog2**: Computes the integer base-2 logarithm of a number,
rounded down.
- **mathy_isqrt**: Computes the integer square root of a number, rounded down.
- **mathy_gcd**: Computes the greatest common denominator (GCD) of two numbers.
- **mathy_lcm**: Computes the least common multiple (LCM) of two numbers.
- **mathy_permutation**: Computes the number of permutations.
- **mathy_combination**: Computes the number of combinations.
- **mathy_lerp**: Computes the linear interpolation between two numbers.
- **mathy_norminv**: Computes the the percent point function of the standard
normal distribution.

---

```c
int8_t mathy_ilog2(uint64_t n);
```

Computes the integer base-2 logarithm of a number, rounded down.

This is also `k-1` where `k` is the bit width of `n`.

### Parameters

`n` Unsigned integer value

### Returns

The integer base-2 logarithm of `n`.

If `n == 0`, returns `-1`.

### Warning

This function accepts `uint64_t`, and passing values outside this range
may yield unexpected results.

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

### Warning

This function accepts `uint64_t`, and passing values outside this range
may yield unexpected results.

This function returns `uint32_t`, and in certain arithmetic expressions
may yield unexpected results.

### Note

This function is always accurate, whereas `(int)sqrt` may suffer from
floating-point inaccuracies.

---

```c
uintmax_t mathy_gcd(uintmax_t a, uintmax_t b);
```

Computes the greatest common denominator (GCD) of two numbers.

### Parameters

`a`, `b` Unsigned integer value

### Returns

The greatest common denominator of `a` and `b`.

If `a == 0 && b == 0`, returns `0`.

### Warning

This function accepts `uintmax_t`, and passing values outside this range
may yield unexpected results.

This function returns `uintmax_t`, and in certain arithmetic expressions
may yield unexpected results.

---

```c
uintmax_t mathy_lcm(uintmax_t a, uintmax_t b);
```

Computes the least common multiple (LCM) of two numbers.

### Parameters

`a`, `b` Unsigned integer value

### Returns

The least common multiple of `a` and `b`.

If `a == 0 || b == 0`, returns `0`.

### Warning

This function is accurate only if the result can be represented by `uintmax_t`.

This function accepts `uintmax_t`, and passing values outside this range
may yield unexpected results.

This function returns `uintmax_t`, and in certain arithmetic expressions
may yield unexpected results.

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

This function accepts `uint8_t`, and passing values outside this range
may yield unexpected results.

This function returns `uintmax_t`, and in certain arithmetic expressions
may yield unexpected results.

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

This function accepts `uint8_t`, and passing values outside this range
may yield unexpected results.

This function returns `uintmax_t`, and in certain arithmetic expressions
may yield unexpected results.

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
float mathy_norminvf(float p);
double mathy_norminv(double p);
```

Computes the percent point function of the standard normal distribution.

### Parameters

`p` Floating-point value

### Returns

The percent point function of `p`.

If `p` is outside the range `[0.0, 1.0]`, returns NaN.

### Note

If `p == 0`, the value returned is equal to negative infinity.

If `p == 1`, the value returned is equal to positive infinity.
