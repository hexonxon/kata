//
// The infamous fizz buzz, but not using a modulo
//


#include <iostream>

int main(void)
{
    int to_fizz = 2;
    int to_buzz = 4;

    for (int i = 1; i <= 100; ++i, --to_fizz, --to_buzz) {

        if ((to_fizz == 0) && (to_buzz == 0)) {
            std::cout << "FizzBuzz\n";
            to_fizz = 3;
            to_buzz = 5;
        } else if (to_fizz == 0) {
            std::cout << "Fizz\n";
            to_fizz = 3;
        } else if (to_buzz == 0) {
            std::cout << "Buzz\n";
            to_buzz = 5;
        } else {
            std::cout << i << std::endl;
        }
    }


    return 0;
}

