/**
 * 1.4. Given a string check if it is a permutation of a palindrome.
 */

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

bool is_palindrome_permutation(const char* str)
{
    assert(str);

    size_t len = 0;
    size_t num_odd = 0;
    bool hist[128] = {false};
    for (size_t i = 0; str[i] != '\0'; ++i) {
        if (!isalpha(str[i]))
            continue;

        hist[str[i]] ^= 1;
        if (hist[str[i]])
            num_odd++;
        else
            num_odd--;

        ++len;
    }

    return (len & 0x1 ? num_odd == 1 : num_odd == 0);
}

int main(void)
{
    assert(is_palindrome_permutation("tact coa"));
    assert(!is_palindrome_permutation("tact toa"));
    assert(!is_palindrome_permutation("tact coaz"));
}
