#include <stdio.h>
#include "mathy.h"

#define n 11

int main() {
    // Anscombe's quartet
    const double x[][n] = {
        {10.0, 8.0, 13.0, 9.0, 11.0, 14.0, 6.0, 4.0, 12.0, 7.0, 5.0},
        {8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 19.0, 8.0, 8.0, 8.0},
    };
    const double y[][n] = {
        {8.04, 6.95, 7.58, 8.81, 8.33, 9.96, 7.24, 4.26, 10.84, 4.82, 5.68},
        {9.14, 8.14, 8.74, 8.77, 9.26, 8.10, 6.13, 3.10, 9.13, 7.26, 4.74},
        {7.46, 6.77, 12.74, 7.11, 7.81, 8.84, 6.08, 5.39, 8.15, 6.42, 5.73},
        {6.58, 5.76, 7.71, 8.84, 8.47, 7.04, 5.25, 12.50, 5.56, 7.91, 6.89},
    };

    for (int i = 0; i < 4; i++) {
        const double mu_x = mathy_stats_mean(x[i!=3], n);
        const double mu_y = mathy_stats_mean(y[i], n);
        const double var_x = mathy_stats_var(x[i!=3], n);
        const double var_y = mathy_stats_var(y[i], n);
        const double cov_xy = mathy_stats_cov(x[i!=3], y[i], n);
        const double corr_xy = mathy_stats_corr(x[i!=3], y[i], n);

        const double m = cov_xy/var_x;
        const double b = mu_y-m*mu_x;
        const double r2 = corr_xy*corr_xy;
        // Linear regression: 0.500*x+3.00 with r^2=0.67
        printf("Linear regression: %.3f*x+%.2f with r^2=%.2f\n", m, b, r2);
    }

    return 0;
}
