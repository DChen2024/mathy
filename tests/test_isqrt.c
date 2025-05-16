#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

#ifdef _MSC_VER
#include <intrin.h> /* _BitScanReverse */
#endif

int8_t ilog2(uint64_t n) {
    if (n == 0)
        return -1;
#if __GNUC__ /* GCC or Clang */
    return (int8_t)__builtin_clzll(n)^0x3F;
#elif _MSC_VER /* MSVC */
    uint32_t i;
#if _WIN64 /* 64-bit */
    _BitScanReverse64((unsigned long*)&i, n);
    return (int8_t)i;
#else _WIN32 /* 32-bit */
    if (n >> 32) {
        _BitScanReverse((unsigned long*)&i, n>>32);
        return (int8_t)(i+32);
    }
    else {
        _BitScanReverse((unsigned long*)&i, n);
        return (int8_t)i;
    }
#endif /* Word size */
#else /* Not GCC, Clang, or MSVC */
    int8_t x = 0;
    for (uint8_t i = 32; i; i >>= 1)
        if (n >> i)
            n >>= i, x |= i;
    return x;
#endif /* Compiler */
}

// Standard library function isqrtf
uint32_t isqrt_binaryf(uint64_t n) {
    return (uint32_t)sqrtf(n);
}

// Standard library function isqrt
uint32_t isqrt_binaryd(uint64_t n) {
    return (uint32_t)sqrt(n);
}

#if LDBL_MANT_DIG >= 64 /* long double */
// Standard library function isqrtl
uint32_t isqrt_binaryl(uint64_t n) {
    return (uint32_t)sqrtl(n);
}
#endif /* Extended precision floating-point */

// Find the result bit-by-bit
uint32_t isqrt_bitwise(uint64_t n) {
    uint32_t x = 0;
    for (uint32_t i = UINT32_C(1)<<(ilog2(n)/2); i; i >>= 1) {
        uint64_t temp = x|i;
        if (temp*temp <= n)
            x = temp;
    }
    return x;
}

// Newton's method
uint32_t isqrt_newtons(uint64_t n) {
    if (n == 0)
        return 0;
    uint64_t x0 = UINT64_C(1)<<(ilog2(n)/2+1);
    uint64_t x1 = (x0+n/x0)/2;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0+n/x0)/2;
    }
    return (uint32_t)x0;
}

// CPython implementation
uint32_t isqrt_cpython(uint64_t n) {
    // https://github.com/mdickinson/snippets/blob/main/papers/isqrt/isqrt.pdf
    uint64_t a = 1;
    const int8_t c = ilog2(n)/2;
    for (int8_t s = ilog2(c), d = 0, e; s >= 0; s--) {
        e = d;
        d = c>>s;
        a = (a<<(d-e-1))+(n>>(2*c-d-e+1))/a;
    }
    return (uint32_t)(a-(a*a-1>=n));
}

// Hybrid implementation for accuracy
uint32_t isqrt_binary2(uint64_t n) {
    if (n < (1ULL<<FLT_MANT_DIG/2*2)+(1ULL<<(FLT_MANT_DIG+2)/2)-1)
        return (uint32_t)sqrtf(n);
    if (n < (1ULL<<DBL_MANT_DIG/2*2)+(1ULL<<(DBL_MANT_DIG+2)/2))
        return (uint32_t)sqrt(n);
#if LDBL_MANT_DIG >= 64 /* long double */
    return (uint32_t)sqrtl(n);
#else /* double */
    return isqrt_cpython(n);
#endif /* Extended precision floating-point */
}

int main(int argc, char **argv) {
    srand(time(NULL));
    clock_t start, end;
    double duration;
    const int n = 100000000;
    volatile uint32_t x;

    // For IEEE-754, limit of sqrtf accuracy is n=16785407
    // This is (1<<24)+(1<<13)-1
    for (uint32_t i = 1; i <= UINT32_MAX; i++) {
        if (isqrt_binaryf(i) != isqrt_cpython(i)) {
            printf("For sqrtf n=%"PRIu32"\n", i);
            break;
        }
    }

    // For IEEE-754, limit of sqrt accuracy is n=4503599761588224
    // This is (1<<52)+(1<<27)
    for (uint64_t i = 1; i <= UINT32_MAX; i++) {
        if (isqrt_binaryd(i*i-1) != isqrt_cpython(i*i-1)) {
            printf("For sqrtd n=%"PRIu64"\n", i*i-1);
            break;
        }
    }

    if (argc > 1) { // Only run if additional command line argument is given
#if LDBL_MANT_DIG >= 64 /* long double */
    // For uint64_t, (uint32_t)sqrtl is always accurate
    for (uint64_t i = 1; i <= UINT32_MAX; i++) {
        if (isqrt_binaryl(i*i-1) != isqrt_cpython(i*i-1)) {
            printf("For sqrtl n=%"PRIu64"\n", i*i-1);
            break;
        }
    }
#endif /* Extended precision floating-point */
    }

    // Exponential distribution to represent common values passed to isqrt
    uint64_t* arr = (uint64_t*)malloc(n*sizeof(uint64_t));
    if (arr == NULL)
        return 0;
    for (int i = 0; i < n; i++) {
        double random = UINT32_MAX*-log((double)rand()/RAND_MAX);
        arr[i] = random<=UINT64_MAX ? (uint64_t)random : UINT64_MAX;
    }

    // sqrtf slightly faster than sqrt
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_binaryf(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryf isqrt took %f seconds\n", duration);

    // sqrt slightly slower than sqrtf
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_binaryd(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryd isqrt took %f seconds\n", duration);

#if LDBL_MANT_DIG >= 64 /* long double */
    // sqrtl slower than sqrtf and sqrt
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_binaryl(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryl isqrt took %f seconds\n", duration);
#endif /* Extended precision floating-point */

    // Bitwise is the slowest
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_bitwise(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Bitwise isqrt took %f seconds\n", duration);

    // Newtons is slightly slower than CPython
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_newtons(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Newtons isqrt took %f seconds\n", duration);

    // CPython is slightly faster than Newtons
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_cpython(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("CPython isqrt took %f seconds\n", duration);

    // Hybrid implementation performs well
    start = clock();
    for (int i = 0; i < n; i++)
        x = isqrt_binary2(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binary2 isqrt took %f seconds\n", duration);

    free(arr);

    return 0;
}
