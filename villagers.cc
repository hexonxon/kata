#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <climits>

using namespace std;

size_t count_leading_ones(const vector<bool>& v)
{
    if (v.empty()) {
        return 0;
    }

    int start = 0;
    int end = v.size() - 1;

    while (start <= end) {
        int pos = start + (end - start) / 2;
        if (v[pos] == 0) {
            if (pos == 0 || v[pos - 1] == 1) {
                return pos;
            }

            end = pos - 1;
        } else {
            start = pos + 1;
        }
    }
}

vector<vector<bool>>::const_iterator find_least_leading_ones(const vector<vector<bool>>& rows)
{
    size_t min = SIZE_MAX;
    auto min_pos = rows.end();

    for (auto i = rows.begin(); i != rows.end(); ++i) {
        size_t leading_ones = count_leading_ones(*i);
        cout << leading_ones << endl;
        if (min > leading_ones) {
            min = leading_ones;
            min_pos = i;
        }
    }

    return min_pos;
}

int main(void)
{
    vector<vector<bool>> v = {
        {0, 0, 0, 0, 0, 0, 0, 0 },
        {1, 0, 0, 0, 0, 0, 0, 0 },
        {1, 1, 0, 0, 0, 0, 0, 0 },
        {1, 1, 1, 0, 0, 0, 0, 0 },
        {1, 1, 1, 1, 0, 0, 0, 0 },
        {1, 1, 1, 1, 1, 0, 0, 0 },
        {1, 1, 1, 1, 1, 1, 0, 0 },
        {1, 1, 1, 1, 1, 1, 1, 0 },
        {1, 1, 1, 1, 1, 1, 1, 1 },
    };

    auto pos = find_least_leading_ones(v);
    cout << count_leading_ones(*pos) << endl;
    return 0;
}
