#include "charset.h"
#include "result.h"
#include <iostream>

int main(int argc, char** argv) {
    Result result;
    u8string input = "ウエイ";
    u32string pron;
    tools::charset::Utf8ToUnicode(input, pron);
    result.pron = pron;
    std::cout << "haha" << std::endl;
    return 0;
}
