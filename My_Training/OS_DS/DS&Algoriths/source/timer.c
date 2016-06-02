/* timer.c */

#include <time.h>

static double last = 0.0;

double Elapsed() {
    double t, diff;
    t = clock();
    diff = t - last;
    last = t;
    return diff/CLOCKS_PER_SEC;
    }
