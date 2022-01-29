/**
 * 1.5 There are three types of edits that can be performed on strings: insert a character,
 *     remove a character, or replace a character. Given two strings, write a function to check if they are
 *     one edit (or zero edits) away.
 */

#include <iostream>
#include <assert.h>
#include <string.h>

bool most_one_edit_away(const char* a, const char* b)
{
    if (!a || !b)
        return false;
  
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    if (alen > blen) {
        std::swap(a, b);
        std::swap(alen, blen);
    }

    assert(alen <= blen);
    if (blen - alen > 1)
        return false;

    bool diff = false;
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            if (diff) {
                return false;
            }

            diff = true;
            if (alen == blen) {
                /* Can be fixed by edit, advance both strings */
                ++a;
                ++b;
            } else {
                /* Shorter string can be fixed by insertion,
                   advance longer one */
                ++b;
            }
        } else {
            ++a;
            ++b;
        }
    }

    assert(*a == *b && *b == '\0');
  
    /* If string are not equal len, then we can't have difference now */
    return true;
}

int main() {
    assert(most_one_edit_away("pale", "ple"));
    assert(most_one_edit_away("pale", "pale"));
    assert(!most_one_edit_away("pale", "bake"));
    assert(!most_one_edit_away("pale", "palest"));
    return 0;
}
