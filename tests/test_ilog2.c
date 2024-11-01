#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

// Compiler intrinsics
int8_t ilog2_countlz(uint64_t n) {
    if (n == 0)
        return -1;
#if __GNUC__ // GCC or Clang
    return __builtin_clzll(n)^0x3F;
#elif _MSC_VER // MSVC
    return __lzcnt64(n)^0x3F;
#else
    #error "Compile using GCC, Clang, or MSVC"
#endif
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

// Standard library function ilogbl
int8_t ilog2_binaryl(uint64_t n) {
    if (n == 0)
        return -1;
    return (int8_t)ilogbl(n);
}

// Find first set most significant bit
int8_t ilog2_bitwise(uint64_t n) {
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
    if (n < ((uint64_t)FLT_RADIX<<FLT_MANT_DIG)-FLT_RADIX/2)
        return (int8_t)ilogbf(n);
    if (n < ((uint64_t)FLT_RADIX<<DBL_MANT_DIG)-FLT_RADIX/2)
        return (int8_t)ilogb(n);
#if __GNUC__ // GCC and Clang
    return (int8_t)ilogbl(n);
#elif _MSC_VER // MSVC
    return ilog2_bitwise(n); // For MSVC, double == long double
#else
    #error "Compile using GCC, Clang, or MSVC"
#endif
}

int main() {
    srand(time(NULL));
    clock_t start, end;
    double f;
    const int n = 100000000;
    volatile uint32_t x;

    uint64_t* arr = (uint64_t*)malloc(n*sizeof(uint64_t));
    if (arr == NULL)
        return 0;
    for (int i = 0; i < n; ++i) {
        // Generate exponential distribution
        // Represent realistic values passed to ilog2
        double random = UINT32_MAX*-log((double)rand()/RAND_MAX);
        arr[i] = random<=UINT64_MAX ? (uint64_t)random : UINT64_MAX;
    }

    // Intrinsics is the fastest
    start = clock();
    for (int i = 0; i < n; ++i)
        x = ilog2_countlz(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Countlz ilog2 took %f seconds\n", f);

    // ilog2f and ilog2 are roughly similar
    start = clock();
    for (int i = 0; i < n; ++i)
        x = ilog2_binaryf(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryf ilog2 took %f seconds\n", f);

    // ilog2 and ilog2f are roughly similar
    start = clock();
    for (int i = 0; i < n; ++i)
        x = ilog2_binaryd(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryd ilog2 took %f seconds\n", f);

    // ilog2l slower than ilog2f and ilog2
    start = clock();
    for (int i = 0; i < n; ++i)
        x = ilog2_binaryl(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryl ilog2 took %f seconds\n", f);

    // Bitwise and ilog2l are roughly similar
    start = clock();
    for (int i = 0; i < n; ++i)
        x = ilog2_bitwise(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Bitwise ilog2 took %f seconds\n", f);

    // Hybrid implementation performs well
    start = clock();
    for (int i = 0; i < n; ++i)
        x = ilog2_binary2(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binary2 ilog2 took %f seconds\n", f);

    free(arr);

    return 0;
}
