#include "charset.h"
#include <iostream>


void init_unicode(std::u32string& unicode_str) {
    char32_t buff[tools::charset::kMaxStrLen] = {0};
    buff[0] = 0x20C30;
    unicode_str.assign(buff);
}

int main() {
    std::u16string str16;
    std::string str8;
    std::u32string str32;
    std::u32string unicode_str;

    bool result;
    init_unicode(unicode_str);

    // 输出源unicode
    for (size_t i = 0; i < unicode_str.length(); ++i) {
        std::cout << "32: " << std::hex << unicode_str[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出utf-16-be
    result = tools::charset::UnicodeToUtf16(unicode_str, str16);
    std::cout << "Result: " << result << " " << std::endl;
    for (size_t i = 0; i < str16.length(); ++i) {
        std::cout << "16be: " << std::hex << str16[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出Unicode
    result = tools::charset::Utf16ToUnicode(str16, unicode_str);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < unicode_str.length(); ++i) {
        std::cout << "uni_16_be: " << std::hex << unicode_str[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出utf-16-le
    result = tools::charset::UnicodeToUtf16(unicode_str, str16, false);
    std::cout << "Result: " << result << " " << std::endl;
    for (size_t i = 0; i < str16.length(); ++i) {
        std::cout << "16le: " << std::hex << str16[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出Unicode
    result = tools::charset::Utf16ToUnicode(str16, unicode_str, false);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < unicode_str.length(); ++i) {
        std::cout << "uni_16_le: " << std::hex << unicode_str[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出utf8
    result = tools::charset::UnicodeToUtf8(unicode_str, str8);
    std::cout << "Result: " << result << " " << std::endl;
    for (size_t i = 0; i < str8.length(); ++i) {
        std::cout << "8: " << std::hex;
        std::cout << static_cast<uint32_t>((unsigned char)str8[i]) << std::endl;
    }
    std::cout << std::endl;

    // 输出Unicode
    result = tools::charset::Utf8ToUnicode(str8, unicode_str);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < unicode_str.length(); ++i) {
        std::cout << "uni_8: " << std::hex << unicode_str[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出utf-32-be
    result = tools::charset::UnicodeToUtf32(unicode_str, str32);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < str32.length(); ++i) {
        std::cout << "32be: " << std::hex << str32[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出Unicode
    result = tools::charset::Utf32ToUnicode(str32, unicode_str);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < unicode_str.length(); ++i) {
        std::cout << "uni_32: " << std::hex << unicode_str[i] << std::endl;
    }
    std::cout << std::endl;

    // 输出utf-32-be
    result = tools::charset::UnicodeToUtf32(unicode_str, str32, false);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < str32.length(); ++i) {
        std::cout << "32le: " << std::hex;
        std::cout << static_cast<uint32_t>(str32[i]) << std::endl;
    }
    std::cout << std::endl;

    // 输出Unicode
    result = tools::charset::Utf32ToUnicode(str32, unicode_str, false);
    std::cout << "Result: " << result << std::endl;
    for (size_t i = 0; i < unicode_str.length(); ++i) {
        std::cout << "uni_32: " << std::hex << unicode_str[i] << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
