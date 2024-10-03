#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int parse_nibble(char nibble)
{
    int digit = isdigit(nibble) ? nibble - '0' : tolower(nibble) - 'a' + 10;
    assert(digit >= 0 && digit <= 16);
    return digit;
}

char add_hex_digits(char a, char b, int* carry)
{
    int sum = parse_nibble(a) + parse_nibble(b) + *carry;
    *carry = sum / 16;
    sum = sum % 16;

    return sum < 10 ? sum + '0' : sum - 10 + 'A';
}

const char* skip_prefix(const char* hex, size_t* len)
{
    assert(*len > 0);

    if (hex[0] == '0' && *len >= 2) {
        if (hex[1] == 'x' || hex[1] == 'X') {
            *len -= 2;
            return hex + 2;
        } else {
            // Malformed prefix
            assert(0);
        }
    }

    return hex;
}

char* add_hex(const char* hex1, const char* hex2)
{
    size_t len1 = strlen(hex1);
    size_t len2 = strlen(hex2);

    assert(len1 > 0);
    assert(len2 > 0);

    // Handle optional prefix
    hex1 = skip_prefix(hex1, &len1);
    hex2 = skip_prefix(hex2, &len2);

    char* res = (char*)calloc(1, MAX(len1, len2) + 2);
    int carry = 0;
    int pos = 0;
    
    while (pos < MIN(len1, len2)) {
        res[pos] = add_hex_digits(hex1[len1 - pos - 1], hex2[len2 - pos - 1], &carry);
        pos++;
    }

    while (pos < MAX(len1, len2)) {
        res[pos] = add_hex_digits(len1 > len2 ? hex1[len1 - pos - 1] : hex2[len2 - pos - 1], '0', &carry);
        pos++;
    }

    if (carry) {
        res[pos] = '1';
    }

    // reverse the result string
    char* p1 = res;
    char* p2 = res + pos - 1;
    while (p1 < p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;

        p1++;
        p2--;
    }

    return res;
}

int main()
{
    const char* hex1 = "0x1A3F9C";
    const char* hex2 = "F7B9A";

    char* result = add_hex(hex1, hex2);
    printf("%s\n", result);

    return 0;
}
