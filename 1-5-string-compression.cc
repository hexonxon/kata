//
// 1.5. Implement a method to perform basic string compression using the counts
// of repeated characters. For example, the string aabcccccaaa would become
// a2blc5a3. If the "compressed" string would not become smaller than the original
// string, your method should return the original string.
//

#include <cstring>
#include <cstdlib>

#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv) 
{
    if (argc != 2) {
        cerr << "Please provide a string\n";
        return EXIT_FAILURE;
    }

    const char* str = argv[1];
    ostringstream stream;

    while (*str != '\0') {

        // I am basically reimplementing strspn here. But i guess it is expected.
        // char buf[2] = {*str, '\0'};
        // size_t span = strspn(str,  buf);
        char c = *str;
        size_t span = 0;
        while ((c == *str) && (*str != '\0')) {
            ++span;
            ++str;
        }

        stream << span << c;
    }

    string compressed = stream.str();
    if (compressed.size() >= strlen(argv[1])) {
        cout << argv[1] << endl;
    } else {
        cout << compressed << endl;
    }

    return EXIT_SUCCESS;
}

