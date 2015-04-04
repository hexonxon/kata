#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>

using namespace std;

class heap {
public:

    typedef bool(*ordering_t)(int, int);

    heap(ordering_t ordering) : m_ordering(ordering) { m_v.clear(); }

    size_t size() const {
        return m_v.size();
    }

    void push(int num) {
        m_v.push_back(num);
        std::make_heap(m_v.begin(), m_v.end(), m_ordering);
    }

    int pop () {
        int num = top();
        m_v.erase(m_v.begin());
        std::make_heap(m_v.begin(), m_v.end(), m_ordering);
        return num;
    }

    int top() const {
        return m_v.front();
    }

private:
    
    ordering_t m_ordering;
    vector<int> m_v;
};

// Left heap, all numbers < median
static heap gMaxHeap([=] (int a, int b) -> bool { return a < b; });

// Right heap, all numbers >= median
static heap gMinHeap([=] (int a, int b) -> bool { return a > b; });

// All generated numbers, for tests
static vector<int> gNums;

// This is brute force median calculator, used only for tests so efficiency is not an issue
static int recalc_median()
{
    if (gNums.size() == 0) {
        return 0;
    }

    if (gNums.size() == 1) {
        return gNums[0];
    }

    sort(gNums.begin(), gNums.end());
    size_t middle = gNums.size() >> 1;
    if (gNums.size() & 0x1) {
        return gNums[middle];
    } else {
        return (gNums[middle] + gNums[middle - 1]) >> 1;
    }
}

int main (int argc, char** argv)
{
    long median = 0; // effective median

    srand((unsigned int)time(NULL));

    for (;;) {
        int num = rand() % 100;
        gNums.push_back(num);

        // Push to one of the heaps depending on median value
        (num < median) ? gMaxHeap.push(num) : gMinHeap.push(num);

        // Balance heaps so that they differ in size by 1 element max
        if (gMaxHeap.size() > (gMinHeap.size() + 1)) {
            gMinHeap.push(gMaxHeap.pop());
        } else if (gMinHeap.size() > (gMaxHeap.size() + 1)) {
            gMaxHeap.push(gMinHeap.pop());
        }

        // Compute new median
        if (gMaxHeap.size() == gMinHeap.size()) {
            median = (gMaxHeap.top() + gMinHeap.top()) >> 1;
        } else {
            if (gMaxHeap.size() > gMinHeap.size()) {
                median = gMaxHeap.top();
            } else {
                median = gMinHeap.top();
            }
        }

        // Debug output
        cout << "(";
        for (size_t i = 0; i < gNums.size(); ++i) {
            cout << gNums[i];
            if ((i + 1) < gNums.size()) 
                cout << ", ";
        }

        if (gNums.size() > 1) {
            cout << ") -> " << "[" << gMaxHeap.top() << " <= " << median << " <= " << gMinHeap.top() << "] " << " (" << recalc_median() << ") \n";
        } else {
            cout << ") -> " << median << " (" << recalc_median() << ") \n";
        }

        sleep(1);
    }

    return 0;
}


