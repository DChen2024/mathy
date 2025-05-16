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

// Compiler intrinsics
int8_t ilog2_builtin(uint64_t n) {
    if (n == 0)
        return -1;
#if __GNUC__ /* GCC or Clang */
    return (int8_t)__builtin_clzll(n)^0x3F;
#elif _MSC_VER /* MSVC */
    uint32_t i;
#if _WIN64 /* 64-bit */
    _BitScanReverse64((unsigned long*)&i, n);
    return (int8_t)i;
#else /* 32-bit */
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

// Standard library function ilogbf
int8_t ilog2_binaryf(uint64_t n) {
    if (n == 0)
        return -1;
    return (int8_t)ilogbf(n);
}

// Standard library function ilogb
int8_t ilog2_binaryd(uint64_t n) {
    if (n == 0)
        return -1;
    return (int8_t)ilogb(n);
}

#if LDBL_MANT_DIG >= 64 /* long double */
// Standard library function ilogbl
int8_t ilog2_binaryl(uint64_t n) {
    if (n == 0)
        return -1;
    return (int8_t)ilogbl(n);
}
#endif /* Extended precision floating-point */

// Binary search for BSR
int8_t ilog2_bsearch(uint64_t n) {
    if (n == 0)
        return -1;
    int8_t x = 0;
    for (uint8_t i = 32; i; i >>= 1)
        if (n >> i)
            n >>= i, x |= i;
    return x;
}

// Hybrid implementation for accuracy
int8_t ilog2_binary2(uint64_t n) {
    if (n == 0)
        return -1;
    if (n < (1ULL<<FLT_MANT_DIG+1)-1)
        return (int8_t)ilogbf(n);
    if (n < (1ULL<<DBL_MANT_DIG+1)-1)
        return (int8_t)ilogb(n);
#if LDBL_MANT_DIG >= 64 /* long double */
    return (int8_t)ilogbl(n);
#else /* double */
    return ilog2_bsearch(n);
#endif /* Extended precision floating-point */
}

int main() {
    srand(time(NULL));
    clock_t start, end;
    double duration;
    const int n = 100000000;
    volatile uint8_t x;

    // For IEEE-754, limit of log2f accuracy is n=33554431
    // This is (1<<25)-1
    for (uint8_t i = 1; i <= 64; i++) {
        if (ilog2_binaryf(i) != ilog2_builtin(i)) {
            printf("For log2f n=%"PRIu32"\n", i);
            break;
        }
    }

    // For IEEE-754, limit of log2d accuracy is n=18014398509481983
    // This is (1<<54)-1
    for (uint8_t i = 1; i <= 64; i++) {
        if (ilog2_binaryd((1ULL<<i)-1) != ilog2_builtin((1ULL<<i)-1)) {
            printf("For log2d n=%"PRIu64"\n", (1ULL<<i)-1);
            break;
        }
    }

#if LDBL_MANT_DIG >= 64 /* long double */
    // For uint64_t, (uint32_t)log2l is always accurate
    for (uint8_t i = 1; i <= 64; i++) {
        if (ilog2_binaryl((1ULL<<i)-1) != ilog2_builtin((1ULL<<i)-1)) {
            printf("For log2d n=%"PRIu64"\n", (1ULL<<i)-1);
            break;
        }
    }
#endif /* Extended precision floating-point */

    // Exponential distribution to represent common values passed to ilog2
    uint64_t* arr = (uint64_t*)malloc(n*sizeof(uint64_t));
    if (arr == NULL)
        return 0;
    for (int i = 0; i < n; i++) {
        double random = UINT32_MAX*-log((double)rand()/RAND_MAX);
        arr[i] = random<=UINT64_MAX ? (uint64_t)random : UINT64_MAX;
    }

    // Builtin is the fastest
    start = clock();
    for (int i = 0; i < n; i++)
        x = ilog2_builtin(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Builtin ilog2 took %f seconds\n", duration);

    // ilog2f and ilog2 are roughly similar
    start = clock();
    for (int i = 0; i < n; i++)
        x = ilog2_binaryf(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryf ilog2 took %f seconds\n", duration);

    // ilog2 and ilog2f are roughly similar
    start = clock();
    for (int i = 0; i < n; i++)
        x = ilog2_binaryd(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryd ilog2 took %f seconds\n", duration);

#if LDBL_MANT_DIG >= 64 /* long double */
    // ilog2l slower than ilog2f and ilog2
    start = clock();
    for (int i = 0; i < n; i++)
        x = ilog2_binaryl(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryl ilog2 took %f seconds\n", duration);
#endif /* Extended precision floating-point */

    // Bsearch and ilog2l are roughly similar
    start = clock();
    for (int i = 0; i < n; i++)
        x = ilog2_bsearch(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Bsearch ilog2 took %f seconds\n", duration);

    // Hybrid implementation performs well
    start = clock();
    for (int i = 0; i < n; i++)
        x = ilog2_binary2(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binary2 ilog2 took %f seconds\n", duration);

    free(arr);

    return 0;
}
