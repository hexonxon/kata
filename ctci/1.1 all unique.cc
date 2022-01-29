/**
 * 1.1. Implement an algorithm to determine if string has all unique characters
 */

bool all_unique_chars(const char* str)
{
    if (!str) {
        return true;
    }

    bool freq[128] = {0};
    while (*str != '\0') {
        char key = *str & 0x7F;
        if (freq[key]) {
            return false;
        }

        freq[key] = true;
        ++str;
    }

    return true;
}

#include <assert.h>

int main(void)
{
    assert(true == all_unique_chars("abc"));
    assert(false == all_unique_chars("aabc"));
}
