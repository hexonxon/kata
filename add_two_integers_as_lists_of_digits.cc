/**
 * Given the head pointers of two linked lists where each linked list represents an integer number (each node is a digit),
 * add them and return the resulting linked list.
 */

#include <cassert>
#include <list>

/* Handles inverted lists: head points to least-significant digit */
struct std::list<int> add_digits(const std::list<int> &l1, const std::list<int> &l2)
{
    int carry = 0;

    std::list<int> res;
    std::list<int>::const_iterator h1 = l1.begin();
    std::list<int>::const_iterator h2 = l2.begin();

    while (h1 != l1.end() || h2 != l2.end()) {
        int d1 = 0;
        int d2 = 0;

        if (h1 != l1.end()) {
            d1 = *h1++;
        }

        if (h2 != l2.end()) {
            d2 = *h2++;
        }

        int sum = d1 + d2 + carry;
        carry = sum / 10;

        res.push_back(sum % 10);
    }

    if (carry) {
        res.push_back(carry);
    }

    return res;
}

int main(void)
{
    {
        /* 9901 + 237 = 10138 */
        std::list<int> l1 = {1, 0, 9, 9};
        std::list<int> l2 = {7, 3, 2};
        std::list<int> expected = {8, 3, 1, 0, 1};
        std::list<int> sum = add_digits(l1, l2);
        assert(sum == expected);
    }

    {
        /* One list is empty */
        std::list<int> l1 = {1, 0, 9, 9};
        std::list<int> l2 = {};
        std::list<int> sum = add_digits(l1, l2);
        assert(sum == l1);
    }

    {
        /* Both lists are empty */
        std::list<int> l1 = {};
        std::list<int> l2 = {};
        std::list<int> sum = add_digits(l1, l2);
        assert(sum.empty());
    }
}
