// Unicode 各种编码方式的方法请看:
// http://baike.baidu.com/view/40801.htm

#include "charset.h"

namespace tools {
namespace charset {

// 单个字符的大小头转换
// 参数:
//      src_char: 源字符
// 返回:
//      返回目标字符
template <typename T>
T reverseChar(const T &src_char) {
    size_t bytes = sizeof(T);
    T des_char = 0;
    for (size_t i = 0; i < bytes; ++i) {
        des_char = des_char << 8;
        des_char |= *((unsigned char*)(&src_char) + i);
    }
    return des_char;
}

// 字符串的大头转小头或小头转大头
// 参数:
//      str: 需要进行大小头转换的字符串
template <typename T>
void reverseBytes(T &str) {
    for (size_t idx_char = 0; idx_char < str.length(); ++idx_char) {
        str[idx_char] = reverseChar(str.c_str()[idx_char]);
    }
}

// Unicode (hex)   | UTF-8 (bin)
// 000000 - 00007F | 0XXXXXXX
// 000080 - 0007FF | 110XXXXX 10XXXXXX
// 000800 - 00FFFF | 1110XXXX 10XXXXXX 10XXXXXX
// 010000 - 10FFFF | 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
bool UnicodeToUtf8(const std::u32string &unicode_str,
                   std::string &str8) {
    size_t idx_8 = 0;
    char buff8[kMaxStrLen] = {0}; // 临时空间
    for (size_t idx_uni = 0; idx_uni < unicode_str.length(); ++idx_uni) {
        char32_t char_uni = unicode_str[idx_uni];
        size_t offset = 0;
        if (char_uni < 0x00000080) {
            if (idx_8 < kMaxStrLen) {
                buff8[idx_8++] = 0x000000FF & char_uni;
            } else {
                return false;
            }
        } else if (0x00000080 <= char_uni && char_uni < 0x00000800) {
            if (idx_8 < kMaxStrLen - 1) {
                buff8[idx_8++] = 0x000000C0 + (0x0000001F & (char_uni >> 6));
                offset = 1;
            } else {
                return false;
            }
        } else if (0x00000800 <= char_uni && char_uni < 0x00010000) {
            if (idx_8 < kMaxStrLen - 2) {
                buff8[idx_8++] = 0x000000E0 + (0x0000000F & (char_uni >> 12));
                offset = 2;
            } else {
                return false;
            }
        } else if (0x00010000 <= char_uni && char_uni < 0x00110000) {
            if (idx_8 < kMaxStrLen - 3) {
                buff8[idx_8++] = 0x000000F0 + (0x00000007 & (char_uni >> 18));
                offset = 3;
            } else {
                return false;
            }
        } else { // 暂时应该还没有这个字段
            return false;
        }

        // 编码的偏移量
        while (offset > 0) {
            uint32_t bit_offset = 6 * (offset - 1);
            buff8[idx_8++] = 0x00000080 +
                (0x0000003F & (char_uni >> bit_offset));
            offset--;
        }
    }

    str8.assign(buff8);
    return true;
}


// Unicode (hex) | UTF-16 (bin)
// 000000 - 00FFFF | XXXXXXXX XXXXXXXX
// 010000 - 10FFFF | 110110XX XXXXXXXX 110110XX XXXXXXXX (先要减去 0x10000)
bool UnicodeToUtf16(const std::u32string &unicode_str,
                    std::u16string &str16,
                    bool is_be) {
    size_t idx_16 = 0;
    char16_t buff16[kMaxStrLen] = {0}; // 临时空间
    for (size_t idx_uni = 0; idx_uni < unicode_str.length(); ++idx_uni) {
        char32_t char_uni = unicode_str[idx_uni];
        if (char_uni < 0x00010000) {
            if (idx_16 < kMaxStrLen) { // 判断是否超出了最大长度
                char16_t char16 = char_uni & 0x0000FFFF;
                buff16[idx_16++] = static_cast<char16_t>(char16);
            } else {
                return false;
            }
        } else if (0x00010000 <= char_uni && char_uni < 0x00110000) {
            char_uni -= 0x00010000; // 首先减去 0x00010000
            if (idx_16 < kMaxStrLen - 1) { // 判断是否超出了最大长度
                // 高16位
                char32_t char16_high = 0x0000D800 + (char_uni >> 10);
                buff16[idx_16++] = static_cast<char16_t>(char16_high);
                // 低16位
                char32_t char16_low = 0x0000DC00 + (char_uni & 0x000003FF);
                buff16[idx_16++] = static_cast<char16_t>(char16_low);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    str16.assign(buff16);

    // 小头则需要转一下
    if (! is_be) {
        reverseBytes(str16);
    }
    return true;
}


bool Utf16ToUnicode(const std::u16string &str16,
                    std::u32string &unicode_str,
                    bool is_be) {
    size_t idx_uni = 0;
    char32_t buff_uni[kMaxStrLen] = {0}; // 临时空间

    // 先转为大头数据
    std::u16string str16be;
    if (! is_be) {
        char16_t buff16[kMaxStrLen] = {0};
        str16.copy(buff16, kMaxStrLen);
        str16be.assign(buff16);
        reverseBytes(str16be);
    } else {
        str16be.assign(str16.c_str());
    }

    for (size_t idx_16 = 0; idx_16 < str16be.length(); ++idx_16) {
        char16_t char16 = str16be[idx_16];

        if (idx_uni >= kMaxStrLen) { // 空间不够
            return false;
        }

        // 编码过程
        if (0xD800 <= char16 && char16 <= 0xDFFF) { // 代理区
            buff_uni[idx_uni] = static_cast<char32_t>(char16 & 0x03FF) << 10;
            char16_t char16_next = str16be[++idx_16];
            buff_uni[idx_uni] += static_cast<char32_t>(char16_next & 0x03FF);
            buff_uni[idx_uni] += 0x00010000;
        } else {
            buff_uni[idx_uni] = static_cast<char32_t>(char16) & 0x0000FFFF;
        }
    }

    unicode_str.assign(buff_uni);
    return true;

}


// Unicode (hex)   | UTF-8 (bin)
// 000000 - 00007F | 0XXXXXXX
// 000080 - 0007FF | 110XXXXX 10XXXXXX
// 000800 - 00FFFF | 1110XXXX 10XXXXXX 10XXXXXX
// 010000 - 10FFFF | 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
bool Utf8ToUnicode(const std::string &str8,
                   std::u32string &unicode_str) {
    size_t idx_uni = 0;
    char32_t buff_uni[kMaxStrLen] = {0};

    for (size_t idx_8 = 0; idx_8 < str8.length(); ++idx_8) {
        char char8 = str8[idx_8];
        size_t offset = 0;

        if (idx_uni >= kMaxStrLen) { // 空间不够
            return false;
        }

        // 编码过程
        if ((uint8_t)char8 < 0x80) {
            buff_uni[idx_uni] = static_cast<char32_t>(char8);
        } else if ((uint8_t)char8 < 0xE0) {
            buff_uni[idx_uni] = static_cast<char32_t>(char8 & 0x1F);
            offset = 1;
        } else if ((uint8_t)char8 < 0xF0) {
            buff_uni[idx_uni] = static_cast<char32_t>(char8 & 0x0F);
            offset = 2;
        } else if ((uint8_t)char8 < 0xF8) {
            buff_uni[idx_uni] = static_cast<char32_t>(char8 & 0x07);
            offset = 3;
        }

        // 偏移量
        for (size_t idx_offset = 0; idx_offset < offset; ++idx_offset) {
            buff_uni[idx_uni] = buff_uni[idx_uni] << 6;
            char char8_next = str8[++idx_8];
            buff_uni[idx_uni] |= static_cast<char32_t>(char8_next & 0x3F);
        }

        idx_uni++;
    }
    unicode_str.assign(buff_uni);
    return true;
}


bool Utf32ToUnicode(const std::u32string &str32,
                    std::u32string &unicode_str,
                    bool is_be) {
    if (str32.length() > kMaxStrLen) {
        return false;
    }

    char32_t buff32[kMaxStrLen] = {0};
    str32.copy(buff32, kMaxStrLen);
    unicode_str.assign(buff32);
    if (! is_be) {
        reverseBytes(unicode_str);
    }
    return true;
}


bool UnicodeToUtf32(const std::u32string &unicode_str,
                    std::u32string &str32,
                    bool is_be) {
    if (unicode_str.length() > kMaxStrLen) {
        return false;
    }

    char32_t buff32[kMaxStrLen] = {0};
    unicode_str.copy(buff32, kMaxStrLen);
    str32.assign(buff32);
    if (! is_be) {
        reverseBytes(str32);
    }
    return true;
}


} // end of namespace charset
} // end of namespace tools
