#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Euclid algorithm
intmax_t gcd_euclid(intmax_t a, intmax_t b) {
    while (b != 0) {
        intmax_t temp = b;
        b = a%b;
        a = temp;
    }
    return a<0 ? -a : a; // return abs(a)
}

// Steins algorithm
intmax_t gcd_steins(intmax_t a, intmax_t b) {
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    int8_t k = 0;
    while (((a | b) & 1) == 0) {
        a >>= 1;
        b >>= 1;
        k++;
    }
    while ((a & 1) == 0)
        a >>= 1;
    while (b != 0) {
        while ((b & 1) == 0)
            b >>= 1;
        if (a > b) {
            intmax_t temp = b;
            b = a;
            a = temp;
        }
        b -= a;
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
        double random = INTMAX_MAX/2*-log((double)rand()/RAND_MAX);
        arr1[i] = random<=INTMAX_MAX ? (intmax_t)random : INT64_MAX;
        random = INTMAX_MAX/2*-log((double)rand()/RAND_MAX);
        arr2[i] = random<=INTMAX_MAX ? (intmax_t)random : INT64_MAX;
    }

    // Euclid algorithm is faster than Steins algorithm
    start = clock();
    for (int i = 0; i < n; i++)
        x = gcd_euclid(arr1[i], arr2[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Euclid gcd took %f seconds\n", duration);

    // Steins algorithm is slower than Euclid algorithm
    start = clock();
    for (int i = 0; i < n; i++)
        x = gcd_steins(arr1[i], arr2[i]);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Binary gcd took %f seconds\n", duration);

    return 0;
}
