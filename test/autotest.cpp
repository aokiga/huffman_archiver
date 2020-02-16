#include "autotest.h"
#include <iostream>

int Autotest::failedNum = 0;
int Autotest::totalNum = 0;

void Autotest::check(bool expr) {
    if (!expr)
        failedNum++;
    totalNum++;
}

void Autotest::show_final_result() {
    if (failedNum == 0) {
        std::cout << "All tests have passed.\n";
    }
    else {
        std::cout << "Failed " << failedNum << " of " << totalNum << " tests.\n";
    }
}