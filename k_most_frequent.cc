#include <unordered_map>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

static void dump_res(const vector<int>& res)
{
    for (auto i = res.begin(); i != res.end(); ++i) {
        cout << *i << endl;
    }
}

static vector<size_t>::const_iterator find_insert_pos(const vector<size_t>& vec, size_t val)
{
    auto start = vec.begin();
    auto end = prev(vec.end());

    while (start < end) {
        auto mid = start + (end - start) / 2;
        if (*mid > val) {
            auto n = next(mid);
            if (*n <= val) {
                return n;
            }

            start = n;
        } else if (*mid < val) {
            auto p = prev(mid);
            if (*p >= val) {
                return mid;
            }

            end = p;
        } else {
            return mid;
        }
    }

    return vec.begin();
}

vector<int> k_most_frequent(const vector<int>& v, int k)
{
    unordered_map<int, size_t> hist;
    for (auto i : v) {
        hist[i]++;
    }

    vector<int> vals(k, 0);
    vector<size_t> freq(k, 0);

    for (auto i = hist.begin(); i != hist.end(); ++i) {
        if (i->second < freq.back()) {
            continue;
        }

        auto pos = find_insert_pos(freq, i->second);
        if (pos != freq.end()) {
            if (freq.size() == k) {
                freq.pop_back();
                vals.pop_back();
            }

            freq.insert(pos, i->second);
            vals.insert(vals.begin() + (pos - freq.begin()), i->first);
        }
    }

    return vals;
}

int main(void)
{
    vector<int> v = {0, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5};
    dump_res(k_most_frequent(v, 3));
    dump_res(k_most_frequent(v, 2));
    dump_res(k_most_frequent(v, 1));
}
