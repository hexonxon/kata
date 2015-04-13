//
// 1.1. Implement an algorithm to determine if a string has all unique characters.
// -------------------------------------------------------------------------------
//
// We can easily represent the char value space using a bitmap
// CHAR_MAX is 127 so we will need 16 bytes to store the bitmap
//
// We can also use an array indexed by a char and containing number of characters seen
// Or a hash map with a character as a key 
//
// If i can't use additional data structures i would sort the string and then run through it looking for 2 consective chars
// which would yeild a O(nlogn) for sorting (assuming qsort) + O(n) for searching
//
// If we don't sort the string we would have to run a O(n^2) comparison
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include "bitmap.h"

using namespace std;

int main(int argc, char** argv) 
{
    if (argc != 2) {
        cerr << "Please provide a string to test\n";
        return EXIT_FAILURE;
    }

    const char* str = argv[1];


    Bitmap chars(CHAR_MAX);
    
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i) {
        if (chars.IsSet(str[i])) {
            cout << "String has a repeating character \'" << str[i] << "\'\n";
            return 0;
        }

        chars.Set(str[i]);
    }

    cout << "String has no repeating characters\n";
    return 0;
}

