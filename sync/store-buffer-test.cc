#include <thread>

static volatile unsigned long x;
static volatile unsigned long y;
static volatile unsigned long r1, r2, r3, r4;
static volatile unsigned long barrier;

int main(void)
{
    auto f1 = [&]() {
        asm volatile ("lock incl %0\r\n" : "+m"(barrier));
        while (barrier != 2)
            ;

        x = 1;
        r1 = x;
        r2 = y;
    };

    auto f2 = [&]() {
        asm volatile ("lock incl %0\r\n" : "+m"(barrier));
        while (barrier != 2)
            ;

        y = 1;
        r3 = y;
        r4 = x;
    };

    int total = 0;

    for (int i = 0; i < 1000000; ++i) {
        x = 0;
        y = 0;
        r1 = r2 = r3 = r4 = 0;
        barrier = 0;

        std::thread t1(f1);
        std::thread t2(f2);

        t1.join();
        t2.join();

        if (r2 == 0 && r4 == 0) {
            ++total;
        }
    }

    printf("Seen %d cases\n", total);
    return 0;
}
