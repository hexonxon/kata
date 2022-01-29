#include <iostream>
#include <assert.h>
#include <string.h>

const char* encode_string(const char* src, char* buf)
{
    if (!src || !buf)
        return NULL;

    size_t srclen = strlen(src);
    if (srclen == 0)
        return src;
  
    size_t enclen = 0;
    size_t spanlen = 0;
    char cur = src[0];
    const char* psrc = src;
    while (*psrc != '\0') {
        if (*psrc != cur) {
            enclen += sprintf(&buf[enclen], "%c%zu", cur, spanlen);
            if (enclen >= srclen)
                return src;
          
            cur = *psrc;
            spanlen = 1;
        } else {
            ++spanlen;
        }

        ++psrc;
    }

    enclen += sprintf(&buf[enclen], "%c%zu", cur, spanlen);
    return (enclen >= srclen ? src : buf);
}

int main() {
    char buf[128];
    assert(strcmp(encode_string("aabcccccaaa", buf), "a2b1c5a3") == 0);
    assert(strcmp(encode_string("abc", buf), "abc") == 0);
    assert(strcmp(encode_string("aabc", buf), "aabc") == 0);
    return 0;
}
