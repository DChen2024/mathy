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

// Standard library function icbrtf
uint32_t icbrt_binaryf(uint64_t n) {
    return (uint32_t)cbrtf(n);
}

// Standard library function icbrt
uint32_t icbrt_binaryd(uint64_t n) {
    return (uint32_t)cbrt(n);
}

#if LDBL_MANT_DIG >= 64
// Standard library function icbrtl
uint32_t icbrt_binaryl(uint64_t n) {
    return (uint32_t)cbrtl(n);
}
#endif /* Extended precision floating-point */

// Find the result bit-by-bit
uint32_t icbrt_bitwise(uint64_t n) {
    uint32_t x = 0;
    for (uint32_t i = UINT32_C(1)<<(ilog2(n)/3); i; i >>= 1) {
        uint64_t temp = x|i;
        if (temp*temp*temp <= n)
            x = temp;
    }
    return x;
}

// Newton's method
uint32_t icbrt_newtons(uint64_t n) {
    if (n == 0) 
        return 0;
    uint64_t x0 = UINT64_C(1)<<(ilog2(n)/3+1);
    uint64_t x1 = (2*x0+n/(x0*x0))/3;
    while (x1 < x0) {
        x0 = x1;
        x1 = (2*x0+n/(x0*x0))/3;
    }
    return x0;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    clock_t start, end;
    double duration;
    const int n = 100000000;
    volatile uint32_t x;

    if (argc > 1) { // Only run if additional command line argument is given
    // Varies based on implementation
    for (uint32_t i = 1; i != 0; i++) {
        if (icbrt_bitwise(i) != icbrt_binaryf(i)) {
            printf("For cbrtf n=%"PRIu32"\n", i);
            break;
        }
    }

    // Varies based on implementation
    for (uint64_t i = 1; i != 0; i++) {
        if (icbrt_bitwise(i) != icbrt_binaryd(i)) {
            printf("For cbrtd n=%"PRIu64"\n", i);
            break;
        }
    }

#if LDBL_MANT_DIG >= 64
    // Varies based on implementation
    for (uint64_t i = 1; i != 0; i++) {
        if (icbrt_bitwise(i) != icbrt_binaryl(i)) {
            printf("For cbrtl n=%"PRIu64"\n", i);
            break;
        }
    }
#endif /* Extended precision floating-point */
    }

    // Exponential distribution to represent common values passed to icbrt
    uint64_t* arr = (uint64_t*)malloc(n*sizeof(uint64_t));
    if (arr == NULL)
        return 0;
    for (int i = 0; i < n; i++) {
        double random = UINT32_MAX*-log((double)rand()/RAND_MAX);
        arr[i] = random<=UINT64_MAX ? (uint64_t)random : UINT64_MAX;
    }

    // cbrtf slightly faster than cbrt
    start = clock();
    for (int i = 0; i < n; i++)
        x = icbrt_binaryf(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryf icbrt took %f seconds\n", duration);

    // cbrt slightly slower than cbrtf
    start = clock();
    for (int i = 0; i < n; i++)
        x = icbrt_binaryd(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryd icbrt took %f seconds\n", duration);

#if LDBL_MANT_DIG >= 64
    // cbrtl slower than cbrtf and cbrt
    start = clock();
    for (int i = 0; i < n; i++)
        x = icbrt_binaryl(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryl icbrt took %f seconds\n", duration);
#endif /* Extended precision floating-point */

    // Bitwise is faster than Newtons
    start = clock();
    for (int i = 0; i < n; i++)
        x = icbrt_bitwise(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Bitwise icbrt took %f seconds\n", duration);

    // Newtons is slower than bitwise
    start = clock();
    for (int i = 0; i < n; i++)
        x = icbrt_newtons(arr[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Newtons icbrt took %f seconds\n", duration);

    return 0;
}
