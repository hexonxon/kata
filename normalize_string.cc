/*
 * Normalize string whitespace inplace
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>

char* normalize(char* str)
{
    if (*str == '\0') {
        return str;
    }

    // Invariant: start always points to position in string where we copy next symbol
    char* start = str;
    char* next = str;

    // Skip trailing whitespace
    while (isspace(*next) && *next != '\0') {
        ++next;
    }

    if (*next == '\0') {
        goto exit;
    }

    while (*next != '\0') {
        if (isspace(*next) && isspace(*(next - 1))) {
            ++next;
            continue;
        }

        *start++ = *next++;
    }

    if (isspace(*(start - 1))) {
        start--;
    }

exit:
    *start = '\0';
    return str;
}

int main(void)
{
    {
        char str[] = "   abc   cba d ";
        printf("\"%s\"\n", normalize(str));
        assert(0 == strcmp("abc cba d", str));
    }

    {
        char str[] = "abc";
        printf("\"%s\"\n", normalize(str));
        assert(0 == strcmp("abc", str));
    }

    {
        char str[] = " ";
        printf("\"%s\"\n", normalize(str));
        assert(0 == strcmp("", str));
    }

    {
        char str[] = "";
        printf("\"%s\"\n", normalize(str));
        assert(0 == strcmp("", str));
    }

    return 0;
}
