#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#ifdef _MSC_VER
#include <intrin.h> /* _BitScanForward */
#endif

#define abs(n) ((n)<0 ? -(n) : (n))
#define min(a,b) ((b)<(a) ? (b) : (a))

int8_t bsf(uint64_t n) {
#if __GNUC__ /* GCC or Clang */
    // Warning: undefined if (n == 0)
    // assert(x != 0);
    return (int8_t)__builtin_ctzll(n);
#elif _MSC_VER_ /* MSVC */
    uint32_t i;
#if _WIN64 /* 64-bit */
    _BitScanForward64((unsigned long*)&i, n);
    return (int8_t)i;
#else /* 32-bit */
    if ((uint32_t)n) {
        _BitScanForward((unsigned long*)&i, n);
        return (int8_t)i;
    }
    else {
        _BitScanForward((unsigned long*)&i, n>>32);
        return (int8_t)(i+32);
    }
#endif /* Word size */
#else /* Not GCC, Clang, or MSVC */
    // Warning: non-terminating loop if (n == 0)
    // assert(x != 0);
    int8_t x = 0;
    for (; (n&1) == 0; x++) {
        n >>= 1;
    }
    return x;
#endif /* Compiler */
}

// Euclid algorithm
intmax_t gcd_euclid(intmax_t a, intmax_t b) {
    while (b != 0) {
        intmax_t temp = b;
        b = a%b;
        a = temp;
    }
    return a<0 ? -a : a; // return abs(a)
}

// Binary algorithm
intmax_t gcd_binary(intmax_t a, intmax_t b) {
    a = abs(a), b = abs(b);
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    int8_t x = bsf(a), y = bsf(b);
    int8_t k = min(x, y);
    a >>= x, b >>= y;
    while (1) {
        if (a > b) {
            intmax_t temp = b;
            b = a;
            a = temp;
        }
        b -= a;
        if (b == 0)
            break;
        b >>= bsf(b);
    }
    return a << k;
}

int main() {
    srand(time(NULL));
    clock_t start, end;
    double duration;
    const int n = 50000000;
    volatile intmax_t x;

    // Exponential distribution to represent common values passed to gcd
    intmax_t* arr1 = (intmax_t*)malloc(n*sizeof(intmax_t));
    intmax_t* arr2 = (intmax_t*)malloc(n*sizeof(intmax_t));
    if (arr1 == NULL || arr2 == NULL)
        return 0;
    for (int i = 0; i < n; i++) {
        double random = INT_MAX*-log((double)rand()/RAND_MAX);
        arr1[i] = random<=INTMAX_MAX ? (intmax_t)random : INTMAX_MAX;
        random = INT_MAX*-log((double)rand()/RAND_MAX);
        arr2[i] = random<=INTMAX_MAX ? (intmax_t)random : INTMAX_MAX;
    }

    // Euclid algorithm is slightly faster than binary algorithm
    start = clock();
    for (int i = 0; i < n; i++)
        x = gcd_euclid(arr1[i], arr2[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Euclid gcd took %f seconds\n", duration);

    // Binary algorithm is slightly slower than Euclid algorithm
    start = clock();
    for (int i = 0; i < n; i++)
        x = gcd_binary(arr1[i], arr2[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binary gcd took %f seconds\n", duration);

    return 0;
}
