#include "config.h"
#include <iostream>


int main() {
    tools::config::Parser parser;
    parser.Load("config.ini");
    std::cout << parser["log"]["format"] << std::endl;
    return 0;
}
