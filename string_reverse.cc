#include <string>
#include <utility>
#include <iostream>

using namespace std;

void reverse_str(string& str)
{
    auto start = str.begin();
    auto end = prev(str.end());

    while (start < end) {
        swap(*start++, *end--);
    }
}

int main(void)
{
    string res("abc");
    reverse_str(res);
    cout << res;
}
