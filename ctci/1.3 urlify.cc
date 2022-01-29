/**
 * 1.3. Replaces all spaces in a string with '%20'. Buffer for extra chars is preallocated.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void urlify(char* buf, size_t slen)
{
    if (!buf || !slen)
        return;

    size_t nspaces = 0;
    for (size_t i = 0; i < slen; ++i) {
        if (isspace(buf[i]))
            ++nspaces;
    }

    char* p1 = &buf[slen - 1];
    size_t reserved_size = nspaces * 2;
    if (slen + reserved_size > strlen(buf))
        return;

    char* p2 = p1 + reserved_size;
    while (nspaces > 0) {
        if (!isspace(*p1)) {
            *p2-- = *p1--;
        } else {
            *p2-- = '0';
            *p2-- = '2';
            *p2-- = '%';
            --p1;
            --nspaces;
        }
    }
}

int main(void)
{
    {
        char s[] = "a b  cd      ";
        urlify(s, 7);
        assert(0 == strcmp(s, "a%20b%20%20cd"));
    }
    {
        char s[] = "      ";
        urlify(s, 2);
        assert(0 == strcmp(s, "%20%20"));
    }
}
