#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int8_t ilog2(uint64_t n) {
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

// Standard library function isqrtf
uint32_t isqrt_binaryf(uint64_t n) {
    return (uint32_t)sqrtf(n);
}

// Standard library function isqrt
uint32_t isqrt_binaryd(uint64_t n) {
    return (uint32_t)sqrt(n);
}

// Standard library function isqrtl
uint32_t isqrt_binaryl(uint64_t n) {
    return (uint32_t)sqrtl(n);
}

// Find the result bit-by-bit
uint32_t isqrt_bitwise(uint64_t n) {
    uint32_t x = 0;
    for (uint32_t i = (uint32_t)1<<(ilog2(n)/2); i; i >>= 1) {
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
    uint64_t x0 = (uint64_t)1<<(ilog2(n)/2+1);
    uint32_t x1 = (x0+n/x0)/2;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0+n/x0)/2;
    }
    return (uint32_t)x0;
}

// Hybrid implementation for accuracy
uint32_t isqrt_binary2(uint64_t n) {
    if (n < 16785407)
        return (uint32_t)sqrtf(n);
    if (n < 4503599761588224)
        return (uint32_t)sqrt(n);
#if __GNUC__ // GCC or Clang
    return (uint32_t)sqrtl(n);
#elif _MSC_VER // MSVC
    return isqrt_newtons(n); // For MSVC, double == long double
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

    // For IEEE-754, limit of sqrtf accuracy is n=16785407
    for (uint32_t i = 1; i != UINT32_MAX; ++i) {
        if (isqrt_bitwise(i) != isqrt_binaryf(i)) {
            printf("For float32_t n=%d\n", i);
            break;
        }
    }

    // For IEEE-754, limit of sqrt accuracy is n=4503599761588224
    for (uint64_t i = 1; i != UINT32_MAX; ++i) {
        if (isqrt_bitwise(i*i-1) != isqrt_binaryd(i*i-1)) {
            printf("For float64_t n=%lld\n", i*i-1);
            break;
        }
    }

    // For uint64_t, (uint32_t)sqrtl is always accurate

    uint64_t* arr = (uint64_t*)malloc(n*sizeof(uint64_t));
    if (arr == NULL)
        return 0;
    for (int i = 0; i < n; ++i) {
        // Generate exponential distribution
        // Represent realistic values passed to isqrt
        double random = UINT32_MAX*-log((double)rand()/RAND_MAX);
        arr[i] = random<=UINT64_MAX ? (uint64_t)random : UINT64_MAX;
    }

    // sqrtf slightly faster than sqrt
    start = clock();
    for (int i = 0; i < n; ++i)
        x = isqrt_binaryf(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryf isqrt took %f seconds\n", f);

    // sqrt slightly slower than sqrtf
    start = clock();
    for (int i = 0; i < n; ++i)
        x = isqrt_binaryd(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryd isqrt took %f seconds\n", f);

    // sqrtl slower than sqrtf and sqrt
    start = clock();
    for (int i = 0; i < n; ++i)
        x = isqrt_binaryl(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binaryl isqrt took %f seconds\n", f);

    // Bitwise is faster than Newtons for small values
    start = clock();
    for (int i = 0; i < n; ++i)
        x = isqrt_bitwise(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Bitwise isqrt took %f seconds\n", f);

    // Newtons is faster than bitwise for large values
    start = clock();
    for (int i = 0; i < n; ++i)
        x = isqrt_newtons(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Newtons isqrt took %f seconds\n", f);

    // Hybrid implementation performs well
    start = clock();
    for (int i = 0; i < n; ++i)
        x = isqrt_binary2(arr[i]);
    end = clock();
    f = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binary2 isqrt took %f seconds\n", f);

    free(arr);

    return 0;
}
