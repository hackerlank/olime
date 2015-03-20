#include "config.h"
#include <iostream>


int main() {
    tools::config::Parser parser;
    parser.Load("config.ini");
    std::cout << parser["kaka"]["c"] << std::endl;
    return 0;
}
