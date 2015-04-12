//
// This tiny problem is from http://www.thousandtyone.com/blog/EasierThanFizzBuzzWhyCantProgrammersPrint100To1.aspx
// 

#include <iostream>

int main(void) {

    for (int i = 0; i < 100; ++i) {
        std::cout << (100 - i) << std::endl;
    }

    return 0;
}
