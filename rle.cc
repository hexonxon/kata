#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cctype>

std::string rle_encode(const std::string& str)
{
    std::ostringstream res;

    if (str.empty()) {
        return str;
    }

    auto start = str.begin();
    auto next = start;

    while (next != str.end()) {
        if (*next != *start) {
            res << next - start << *start;
            start = next;
        }

        ++next;
    }

    assert(start != str.end());
    res << next - start << *start;

    return res.str();
}

std::string rle_decode(const std::string& str)
{
    std::ostringstream res;

    if (str.empty()) {
        return str;
    }

    for (auto i = str.begin(); i != str.end(); i += 2) {
        auto j = std::next(i);
        if (j == str.end()) {
            // Malformed string
            return res.str();
        }

        if (!isdigit(*i)) {
            return res.str();
        }

        int num = *i - '0';
        while (num-- > 0) {
            res << *j;
        }
    }

    return res.str();
}

int main(void)
{
    assert(rle_encode("") == "");
    assert(rle_encode("a") == "1a");
    assert(rle_encode("aaaabcccaa") == "4a1b3c2a");

    assert(rle_decode("") == "");
    assert(rle_decode("1a") == "a");
    assert(rle_decode("4a1b3c2a") == "aaaabcccaa");
    assert(rle_decode("4abb3c2a") == "aaaa");
    assert(rle_decode("4a1b3c2") == "aaaabccc");

    return 0;
}
