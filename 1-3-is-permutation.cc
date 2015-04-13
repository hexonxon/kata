//
// 1.3. Given two strings, write a method to decide if one is a permutation of the other.
// --------------------------------------------------------------------------------------
//
// Create a histogram for each string and compare them
// A histogram is an array covering [0 - CHAR_MAX] range which counts how often each character was seen in a string
// Two strings are considered permutations if their histogram matches exactly
//

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <climits>

#include <iostream>
#include <vector>

using namespace std;

typedef std::vector<size_t> histogram_t;

bool operator == (const histogram_t& h1, const histogram_t& h2)
{
    if (h1.size() != h2.size()) {
        return false;
    }

    for (size_t i = 0; i < h1.size(); ++i) {
        if (h1[i] != h2[i]) {
            return false;
        }
    }

    return true;
}

static void string_histogram(const char* str, histogram_t& histogram)
{
    assert(str != NULL);

    histogram.resize(CHAR_MAX, 0);
    while (*str != '\0') {
        histogram[*str] += 1;
        ++str;
    }
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        cerr << "Please provide two strings to compare\n";
        return EXIT_FAILURE;
    }

    histogram_t str1;
    string_histogram(argv[1], str1);

    histogram_t str2;
    string_histogram(argv[2], str2);

    if (str1 == str2) {
        cout << "Strings are permutations of each other\n";
    } else {
        cout << "Strings are not permutation of each other\n";
    }

    return EXIT_SUCCESS;
}

