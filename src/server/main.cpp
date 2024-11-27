#include <iostream>
#include "common/log.h"

int main() {
    std::cout << "This is crms server" << std::endl;
    LOG_INFO("This is a test log");
    return 0;
}
