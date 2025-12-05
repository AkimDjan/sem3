#include <stdio.h>
#include <math.h>

long double f(long double x) {
    return powl((4 - x * x), 0.5);
}

int main() {
    double r = 2.0;
    long long N;
    scanf("%lld", &N);
    long double dx = r / N;
    long double pi = 0.0;
    long double last_x = 0;
    for (long long i = 0; i < N; i++) {
        pi += f(last_x) * dx;
        last_x += dx;
    }
    printf("Pi=%.15Lf\n", pi);
    return 0;
}