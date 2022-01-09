/**
 * Given a string find all non-single letter substrings that are palindromes.
 */

#include <string>
#include <vector>
#include <cassert>

void scan_palindromes_at(const char* str, int lpos, int rpos, std::vector<std::string>& res)
{
    assert(lpos != rpos);
    assert(str);

    while (lpos >= 0 && str[rpos] != '\0' && str[lpos] == str[rpos]) {
        res.push_back(std::string(&str[lpos], rpos - lpos + 1));
        --lpos;
        ++rpos;
    }
}


/**
 * Scan string and expand potential palindrome at each symbol to left and right
 */
std::vector<std::string> find_palindrome_substrings(const char* str)
{
    if (!str)
        return {};

    if (str[0] == '\0' || str[1] == '\0')
        return {};

    std::vector<std::string> res;
    for (int i = 1; str[i] != '\0'; ++i) {
        /* Look for odd palindrome */
        scan_palindromes_at(str, i - 1, i + 1, res);

        /* Look for even palindrome. Always look at one symbol behind because we start at index 1 */
        scan_palindromes_at(str, i - 1, i, res);
    }

    return res;
}

#include <unordered_set>

int main(void)
{
    std::vector<std::string> res = find_palindrome_substrings("aabbbaa");
    std::unordered_multiset<std::string> table;
    for (const std::string& i : res) {
        table.insert(i);
    }

    assert(table.size() == 7);
    assert(table.count("aa") == 2);
    assert(table.count("bb") == 2);
    assert(table.count("bbb") == 1);
    assert(table.count("abbba") == 1);
    assert(table.count("aabbbaa") == 1);
}
