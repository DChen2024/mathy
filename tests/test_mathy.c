#include "mathy.h"

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define pass() \
    do { \
        if (fail >= 0) { \
            pass++; \
            printf("%s ... \033[1;32mPASS\033[0m\n", __func__); \
        } \
        else { \
            fail = -fail; \
        } \
    } \
    while (0)
#define fail() \
    do { \
        if (fail >= 0) { \
            fail++; \
            printf("%s ... \033[1;31mFAIL\033[0m\n", __func__); \
            fail = -fail; \
        } \
    } \
    while (0)
#define assert(_) \
    do { \
        if (!(_)) { \
            fail(); \
            printf("  File %s, line %d, in %s\n", __FILE__, __LINE__, __func__); \
            printf("    %s\n", #_); \
        } \
    } while (0)

int approx(double a, double b) {
    return fabs(a-b) <= 1e-7;
}

int pass;
int fail;

void test_ilog2() {
    for (int8_t i = 0; i < 64; i++)
        assert(mathy_ilog2(1ULL<<i) == i);
    assert(mathy_ilog2((1ULL<<60)-1) == 59);
    assert(mathy_ilog2((1ULL<<60)+1) == 60);
    assert(mathy_ilog2(0) == -1);
    pass();
}

void test_isqrt() {
    for (uint32_t i = 0; i < 20; i++)
        assert(mathy_isqrt(i*i) == i);
    assert(mathy_isqrt(20*20-1) == 19);
    assert(mathy_isqrt(20*20+1) == 20);
    pass();
}

void test_lerp() {
    assert(approx(mathy_lerp(1, 3, 0.25), 1.5));
    assert(approx(mathy_lerp(1, 3, 0.75), 2.5));
    pass();
}

void test_norminv() {
    assert(approx(mathy_norminv(0.5), 0.0));
    // https://en.wikipedia.org/wiki/97.5th_percentile_point
    assert(approx(mathy_norminv(0.975), 1.959964));
    assert(mathy_norminv(0.0) == -INFINITY);
    assert(mathy_norminv(1.0) == +INFINITY);
    pass();
}

int main() {
    clock_t start = clock();
    test_ilog2();
    test_isqrt();
    test_lerp();
    test_norminv();
    clock_t end = clock();
    printf("\n----------------------------------------------------------------------\n");
    printf("Ran %d tests in %fs\n\n", pass+fail, (double)(end-start)/CLOCKS_PER_SEC);
    printf("%s=%d\n", fail ? "\033[1;31mFAILED\033[0m" : "\033[1;32mPASSED\033[0m", fail ? fail : pass);
    printf("Finished running tests!\n");
    return 0;
}
