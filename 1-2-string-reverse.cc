//
// 1.2. Implement a function void reverse(char* str) in C or C++ which reverses a null-terminated string.
// ------------------------------------------------------------------------------------------------------
//

#include <iostream>

#include <cstdlib>
#include <cstring>

using namespace std;

int main(int argc, char** argv) {

    if (argc != 2) {
        cerr << "Please provide a string to reverse\n";
        return EXIT_FAILURE;
    }

    char* str = argv[1];
    size_t len = strlen(str);
    if (len == 0) {
        // Empty string, nothing to do
        return EXIT_SUCCESS;
    }

    char* pstart = str;
    char* pend = str + len - 1;

    while (pend > pstart) {
        swap(*pstart, *pend);

        ++pstart;
        --pend;
    }

    cout << str << endl;
    return EXIT_SUCCESS;
}

