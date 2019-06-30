/*
 * Transform list of sorted numbers into consequitive ranges:
 * 1,2,3,6,7,10 -> "1-3,6-7,10"
 */

#include <vector>
#include <string>
#include <sstream>
#include <cassert>

static void add_range(std::ostringstream& res, std::vector<int>::const_iterator rstart, int len)
{
    if (!res.str().empty()) {
        res << ",";
    }

    if (len == 1) {
        res << *rstart;
    } else {
        res << *rstart << "-" << *(rstart + len - 1);
    }
}

std::string make_ranges(const std::vector<int>& numbers)
{
    std::ostringstream res;

    if (numbers.empty()) {
        return res.str();
    }

    auto rstart = numbers.begin();
    auto next = std::next(rstart);

    while (next != numbers.end()) {
        if (*std::prev(next) + 1 == *next) {
            ++next;
            continue;
        }

        // Range completed
        add_range(res, rstart, next - rstart);

        rstart = next;
        ++next;
    }

    // Close last range
    add_range(res, rstart, next - rstart);
    return res.str();
}

////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    std::string res;

    res = make_ranges(std::vector<int>{0});
    assert(res == "0");

    res = make_ranges(std::vector<int>{0,1,2,3});
    assert(res == "0-3");

    res = make_ranges(std::vector<int>{0,2,3,6,7,8,10});
    assert(res == "0,2-3,6-8,10");

    res = make_ranges(std::vector<int>{});
    assert(res == "");
}
