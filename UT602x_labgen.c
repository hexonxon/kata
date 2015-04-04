#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Constant xor key
static const char key1[8] = { 0x01, 0x0A, 0x05, 0x07, 0x0F, 0x0C, 0x08, 0x02, };

// First 2 digits lab number (+ 50 for real board)
// Next 2 digits is score value in range [90 - 99] for perfect score. 
// Actual number depends on tm_sec value and is hardcoded to 0x92 for our needs which still gives you 100 score.
// Last 4 digits - number from EDX
static char key2[8] = {'0', '0', '9', '2', '0', '0', '0', '0'};

// Constant index substitution table
static const uint32_t table[8] = {2, 6, 3, 0, 1, 7, 4, 5};

// gmtime->tm_sec value at the start of the grading 
// We set it to foxed value so it makes some calculation that depend on it constant
static const char tm_sec = 15;

static void generate(char* answer)
{
    for (char i = 0; i < 8; ++i) 
    {
        char dl = (key1[i] - '0') ^ key2[i];
        char bl = ((((dl & 0x8) * 2) + (dl & 0x4)) * 2) + (dl & 0x3);
        char al = -(i * tm_sec) & 0x4;
        answer[table[i]] = (bl ^ al) + 'A';
    }
}

int main (int argc, char** argv)
{
    if (argc != 4) {
        fprintf(stderr, "edxlabgen [sim|real] [lab number] [number from EdX]\n");
        return EXIT_FAILURE;
    }

    int is_real;
    if (0 == strcmp(argv[1], "real")) {
        is_real = 1;
    } else if (0 == strcmp(argv[1], "sim")) {
        is_real = 0;
    } else {
        fprintf(stderr, "Invalid board type\n");
        return EXIT_FAILURE;
    }

    if (strlen(argv[2]) > 2) {
        fprintf(stderr, "Invalid lab number\n");
        return EXIT_FAILURE;
    }

    if (strlen(argv[3]) != 4) {
        fprintf(stderr, "Invalid edx number\n");
        return EXIT_FAILURE;
    }

    // Prepare key2
    int labnum = atoi(argv[2]);
    if (labnum < 10) {
        key2[0] = '0';
        key2[1] = argv[2][0];
    } else {
        key2[0] = argv[2][0];
        key2[1] = argv[2][1];
    }

    // The only difference between a simulated and real generators is that lab number is +50
    if (is_real) {
        key2[0] += 5;
    }

    key2[2] = '9';
    key2[3] = '2';

    // Last 4 digits in num array
    key2[4] = argv[3][0];
    key2[5] = argv[3][1];
    key2[6] = argv[3][2];
    key2[7] = argv[3][3];

    // Generate answer
    char answer[] = "12345678";
    generate(answer);
    printf("Copy this to EdX:  %s\n", answer);

    return 0;
}

