/*
 * Reverse word order in a string
 * "aa bbb cccc" -> "cccc bbb aa"
 */

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

char* skip_spaces(char* ptr)
{
    while (*ptr != '\0' && isspace(*ptr)) {
        ++ptr;
    }

    return ptr;
}

void reverse_range(char* start, char* end)
{
    while (start < end) {
        *start ^= *end;
        *end ^= *start;
        *start ^= *end;

        --end;
        ++start;
    }
}

char* reverse_words(char* str)
{
    if (!str) {
        return str;
    }

    // Skip trailing spaces
    char* ptr = str;
    ptr = skip_spaces(ptr);
    if (*ptr == '\0') {
        return str;
    }

    char* start = ptr;
    while (*ptr != '\0') {
        // Invariant: start always points to the beginning of a word
        assert(!isspace(*start));

        if (isspace(*ptr)) {
            // Reverse characters in a word
            reverse_range(start, ptr - 1);

            // Skip trailing spaces
            ptr = skip_spaces(ptr);
            start = ptr;
        } else {
            ++ptr;
        }
    }

    // Reverse last word if needed
    reverse_range(start, ptr - 1);

    // Reverse entire string
    reverse_range(str, ptr - 1);

    return str;
}

int main(void)
{
    {
        char str[] = "  abc def  ghij  ";
        reverse_words(str);
        assert(0 == strcmp("  ghij  def abc  ", str));
    }

    {
        char str[] = "abc def";
        reverse_words(str);
        assert(0 == strcmp("def abc", str));
    }

    {
        char str[] = "abc";
        reverse_words(str);
        assert(0 == strcmp("abc", str));
    }

    {
        char str[] = "";
        reverse_words(str);
        assert(0 == strcmp("", str));
    }

    return 0;
}

