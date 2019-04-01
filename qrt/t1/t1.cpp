#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <stdexcept>

/* (F0 F1) * P^n = (Fn Fn+1), where Pn is operator
 * 0 1
 * 1 1 to power of n */
static unsigned long fibn_modm(unsigned long n, unsigned long m)
{
    unsigned long p[4] = {
        1, 1,
        1, 0
    };

    unsigned long f[2] = {
        0,
        1
    };

    while (n > 0) {
        if (n & 1) {
            unsigned long f1 = (f[0] * p[0] + f[1] * p[2]);
            unsigned long f2 = (f[0] * p[1] + f[1] * p[3]);
            f[0] = f1 % m;
            f[1] = f2 % m;
        }

        unsigned long p11 = (p[0] * p[0] + p[1] * p[2]);
        unsigned long p12 = (p[0] * p[1] + p[1] * p[3]);
        unsigned long p21 = (p[2] * p[0] + p[3] * p[2]);
        unsigned long p22 = (p[2] * p[1] + p[3] * p[3]);

        p[0] = p11 % m;
        p[1] = p12 % m;
        p[2] = p21 % m;
        p[3] = p22 % m;

        n >>= 1;
    }

    return f[0];
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Provide arguments for N and M");
        return EXIT_FAILURE;
    }

    unsigned long n = strtoul(argv[1], NULL, 0);
    unsigned long m = strtoul(argv[2], NULL, 0);
    
    printf("%lu\n", fibn_modm(n, m));

    return EXIT_SUCCESS;
}
