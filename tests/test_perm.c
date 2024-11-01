#include <stdio.h>
#include <stdint.h>

#ifdef __GNUC__
__uint128_t permutation(uint8_t n, uint8_t k) {
    if (k > n)
        return 0;
    __uint128_t prod = 1;
    for (uint8_t i = k; i; --n, --i)
        prod *= n;
    return prod;
}
#endif

int main() {
#ifdef __GNUC__
    // For uint64_t, limit of permutation accuracy
    for (int n = 0; n < 256; ++n) {
        for (int k = 0; k < 256; ++k) {
            __uint128_t x = permutation(n, k);
            if (x != (uint64_t)x) {
                printf("%d\t%d\n", n, k);
                break;
            }
        }
    }
#endif

    return 0;
}
