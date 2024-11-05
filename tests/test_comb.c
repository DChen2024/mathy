#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef __GNUC__ // GCC or Clang
__uint128_t combination(uint8_t n, uint8_t k) {
    if (k > n)
        return 0;
    if (n-k < k) // nCr = nCn-r
        k = n-k;
    __uint128_t* arr = (__uint128_t*)malloc((k+1)*sizeof(__uint128_t));
    if (arr == NULL)
        return (__uint128_t)-1;
    for (uint8_t i = 0; i <= k; i++)
        arr[i] = 0;
    arr[0] = 1;
    for (int16_t i = 1; i <= n; ++i)
        for (uint8_t j = (i < k ? i : k); j; --j)
            arr[j] += arr[j-1];
    __uint128_t x = arr[k];
    free(arr);
    return x;
}
#endif

// Product implementation
uint64_t comb_product(uint8_t n, uint8_t k) {
    if (k > n)
        return 0;
    if (n-k < k) // nCr = nCn-r
        k = n-k;
    uint64_t prod = 1;
    for (uint8_t i = 1; i <= k; --n, ++i)
        prod = prod*n/i; // limit overflow
    return prod;
}

// Pascal's triangle for better accuracy
uint64_t comb_pascals(uint8_t n, uint8_t k) {
    if (k > n)
        return 0;
    if (n-k < k) // nCr = nCn-r
        k = n-k;
    uint64_t* arr = (uint64_t*)malloc((k+1)*sizeof(uint64_t));
    if (arr == NULL)
        return UINT64_MAX;
    for (uint8_t i = 0; i <= k; i++)
        arr[i] = 0;
    arr[0] = 1;
    for (int16_t i = 1; i <= n; ++i)
        for (uint8_t j = (i < k ? i : k); j; --j)
            arr[j] += arr[j-1];
    uint64_t x = arr[k];
    free(arr);
    return x;
}

int main() {
    srand(time(NULL));
    clock_t start, end;
    double duration;
    const int n = 100000000;
    volatile uint64_t x;

#ifdef __GNUC__
    // For uint64_t, limit of combination accuracy
    for (int n = 0; n < 256; ++n) {
        for (int k = 0; k < 256; ++k) {
            __uint128_t x = combination(n, k);
            if (x != (uint64_t)x) {
                printf("%d\t%d\n", n, k);
                break;
            }
        }
    }
#endif

    uint8_t* arr1 = (uint8_t*)malloc(n*sizeof(uint8_t));
    uint8_t* arr2 = (uint8_t*)malloc(n*sizeof(uint8_t));
    if (arr1 == NULL || arr2 == NULL)
        return 0;
    for (int i = 0; i < n; ++i) {
        // Generate exponential distribution
        // Represent realistic values passed to comb
        double random = 0x0F*-log((double)rand()/RAND_MAX);
        arr1[i] = random<=UINT8_MAX ? (uint8_t)random : UINT8_MAX;
        arr2[i] = rand()%(arr1[i]+1);
    }

    // For uint64_t, limit of combination accuracy
    for (int n = 0; n < 256; ++n) {
        for (int k = 0; k < 256; ++k) {
            uint64_t a = comb_product(n, k);
            uint64_t b = comb_pascals(n, k);
            if (a != b) {
                printf("%d\t%d\t%llu\t%llu\n", n, k, b, a);
                break;
            }
        }
    }

    // Product is faster
    start = clock();
    for (int i = 0; i < n; ++i)
        x = comb_product(arr1[i], arr2[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Product comb took %f seconds\n", duration);

    // Pascals is slower
    start = clock();
    for (int i = 0; i < n; ++i)
        x = comb_pascals(arr1[i], arr2[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Pascals comb took %f seconds\n", duration);

    free(arr1);
    free(arr2);

    return 0;
}
