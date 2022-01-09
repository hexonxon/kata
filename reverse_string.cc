/**
 * Reverse the order of words in a given sentence(an array of characters).
 */

#include <cctype>
#include <cassert>
#include <cstring>
#include <utility>

static void skip_spaces(char** str)
{
    assert(str);

    char *pstr = *str;
    while (isspace(*pstr) && *pstr != '\0')
        ++pstr;

    *str = pstr;
}

/* Reverse symbols in range [begin; end] */
static void reverse_range(char* begin, char *end)
{
    while (begin < end) {
        std::swap(*begin, *end);
        ++begin;
        --end;
    }
}

void reverse_string(char* str)
{
    char *pstr = str;

    if (!str)
        return;

    /* Skip trailing spaces */
    skip_spaces(&pstr);

    /* Scan for words and reverse letters in each one */
    while (*pstr != '\0') {
        char* begin = pstr;
        char* end;
        while (!isspace(*pstr) && *pstr != '\0') {
            end = pstr++;
        }
    
        reverse_range(begin, end);
        skip_spaces(&pstr);
    }

    /* Reverse entire string */
    reverse_range(str, pstr - 1);
}

int main(void)
{
    reverse_string(nullptr);

    {
        char str[] = "";
        reverse_string(str);
    }

    {
        char str[] = "  abc def  ghij  ";
        reverse_string(str);
        assert(0 == strcmp("  ghij  def abc  ", str));
    }
}
