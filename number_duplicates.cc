//
// Find a duplicate number in unsorted array of integers in O(N) and unlimited memory
//

#include "bitmap.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstdint>
#include <limits.h>

#include <vector>
#include <iostream>
#include <new>

static bool find_duplicate(const std::vector<int>& v, int& found)
{
    // We will use a bitmap, covering range [0, UINT_MAX].
    //
    // Each set bit indicates that we've seen this number in an array.
    // Duplicates will have the same bit set in both maps.
    //
    // Assuming integers are 32-bits long we will need 512 MB per map.
    // But since we have no explicit requirements on memory efficiency we can live with that.
    // If however memory conservation was an issue
    //

    Bitmap map(UINT_MAX);

    auto i = v.begin();
    while (i != v.end()) {
        int num = *i;
        if (map.IsSet(num)) {
            found = num;
            return true;
        } else {
            map.Set(num);
        }

        ++i;
    }

    return false;
}

static bool find_non_duplicate(const std::vector<int>& v, int& found)
{
    // A reverse task - all numbers are duplicates and we need to find one which is not a duplicate
    //
    // We will use two bitmaps to keep track of the numbers we've seen twice.
    // At the end we will XOR two bitmaps to highlight a bit which will be set for a number that was seen only once

    Bitmap map1(UINT_MAX);
    Bitmap map2(UINT_MAX);

    auto i = v.begin();
    while (i != v.end()) {
        int num = *i;
        if (map1.IsSet(num)) {
            map2.Set(num);
        } else {
            map1.Set(num);
        }
        ++i;
    }

    map1.Xor(map2); // To conserve memory we will use destructive XOR

    size_t setbit = 0;
    if (map1.FindFirstSetBit(setbit)) {
        found = (int)setbit;
        return true;
    }

    return false;
}


int main(int argc, char** argv)
{
    int found = 0;

    const std::vector<int> dup_test_1 = {1, 2, 42, 64, INT_MAX};
    assert(find_duplicate(dup_test_1, found) == false);

    const std::vector<int> dup_test_2 = {73, INT_MAX, 194, 73, 0, 42};
    assert(find_duplicate(dup_test_2, found) == true);
    assert(found == 73);

    const std::vector<int> non_dup_test_1 = {1, 1, INT_MAX, INT_MAX, 42, 42};
    assert(find_non_duplicate(non_dup_test_1, found) == false);

    const std::vector<int> non_dup_test_2 = {1, 1, INT_MAX, INT_MAX, 42};
    assert(find_non_duplicate(non_dup_test_2, found) == true);
    assert(found == 42); 

    return 0;
}


