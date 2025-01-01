#include <stdio.h>
#include <stdint.h>

#ifdef __GNUC__
__uint128_t factorial(uint8_t n) {
    __uint128_t prod = 1;
    for (uint8_t i = n; i; i--)
        prod *= i;
    return prod;
}
#endif

int main() {
#ifdef __GNUC__
    // For uint64_t, limit of factorial accuracy
    for (int n = 0; n < 256; n++) {
        __uint128_t x = factorial(n);
        if (x != (uint64_t)x) {
            printf("%d\n", n);
            break;
        }
    }
#endif

    return 0;
}
