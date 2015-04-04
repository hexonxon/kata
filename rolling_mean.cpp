#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

int main(void) 
{
    long prev_mean = 0;
    size_t n = 0;

    srand((unsigned int)time(NULL));

    for (;;) {
        int x = rand() % 100;
        long mean = (prev_mean * n + x) / (n + 1);
        prev_mean = mean;
        ++n;

        std::cout << "+" << x << " -> " << mean << std::endl;
        sleep(1);
    };

    return 0;
}

