#include "config.h"
#include <iostream>


int main() {
    tools::config::Parser parser;
    bool result = parser.Load("config.ini");
    std::cout << result << " " << parser["log"]["format"] << std::endl;
    return 0;
}
