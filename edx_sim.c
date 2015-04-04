#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Constant xor key
static const char key[8] = { 0x01, 0x0A, 0x05, 0x07, 0x0F, 0x0C, 0x08, 0x02, };

// Constant index substitution table
static const uint32_t table[8] = {2, 6, 3, 0, 1, 7, 4, 5};

// gmtime->tm_sec value at the start of the grading 
// We set it to 1 so it makes some calculation that depend on it constant
static const char tm_sec = 0x1;

// First 4 digits TODO
// Last 4 digits - number from EDX
static char num[8] = {'1', '3', '9', '1', '0', '0', '0', '0'};

static uint32_t foo() 
{
    // Originally there were lots of computations with constants that all depend on tm_sec value
    // We can jump straight into result here
    return 0x5B;
}

int main (int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Please give a number from EdX lab page\n");
        return EXIT_FAILURE;
    }

    if (strlen(argv[1]) != 4) {
        fprintf(stderr, "Invalid edx number, should be 4 digits\n");
        return EXIT_FAILURE;
    }

    num[4] = argv[1][0];
    num[5] = argv[1][1];
    num[6] = argv[1][2];
    num[7] = argv[1][3];

    char answer[] = "12345678";

    char dl;
    char bl;
    char al;

    for (char i = 0; i < 8; ++i) 
    {
        char dl = (key[i] - '0') ^ num[i];
        char bl = ((((dl & 0x8) * 2) + (dl & 0x4)) * 2) + (dl & 0x3);
        char al = -(i * tm_sec) & 0x4;
        answer[table[i]] = (bl ^ al) + 'A';
    }

    printf("Answer: %s\n", answer);
    return 0;
}

