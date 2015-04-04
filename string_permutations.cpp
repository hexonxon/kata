//
// Generate all permutations of input string
//

#include <cstdlib>
#include <iostream>
#include <string>
#include <set>

using namespace std;

static void do_permutation(string prefix, string str, set<string>& res)
{
    if (str.length() == 0) {
        res.insert(prefix);
        return;
    }

    for (size_t i = 0; i < str.length(); ++i) {
        do_permutation(prefix + str[i], str.substr(0, i) + str.substr(i + 1, str.length()), res);
    }
}

static void permutation(string& str, set<string>& res)
{
    do_permutation("", str, res);
}

int main (int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Give a tring please\n";
        return EXIT_FAILURE;
    }

    string str(argv[1]);
    if (str.length() == 0) {
        cerr << "String is too short\n";
        return EXIT_FAILURE;
    }

    cout << "Generating permutation for \"" << str << "\"\n";

    set<string> permutations;
    permutation(str, permutations);

    auto i = permutations.begin();
    while(i != permutations.end()) {
        cout << *i << std::endl;
        ++i;
    }

    return EXIT_SUCCESS;
}

