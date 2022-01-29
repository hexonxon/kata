/**
 * 1.2. Given 2 strings, check if strings are permutations of each other.
 */

#include <string.h>

bool are_permutations(const char* a, const char* b)
{
    /* Note: scans both a and b in linear time */
    if (strlen(a) != strlen(b))
        return false;

    int hist[128] = {0};
    while (*a != '\0') {
        ++hist[*a];
        ++a;
    }

    while (*b != '\0') {
        if (hist[*b] == 0)
            return false;

        --hist[*b];
        ++b;
    }

    return true;
}

#include <assert.h>

int main (void)
{
    assert(are_permutations("abc", "cba"));
    assert(!are_permutations("abc", "aba"));
    assert(!are_permutations("abc", "abca"));
}
