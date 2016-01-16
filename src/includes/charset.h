#ifndef OLIME_TOOLS_CHARSET_H_
#define OLIME_TOOLS_CHARSET_H_
// 用来支持字符集之间的转换
// Unicode 各种编码方式的方法请看:
// http://baike.baidu.com/view/40801.htm
// 现支持 Unicode 转 UTF-8, UTF-16, UTF-32
// TODO: 还可以支持 BOM, 但是现在的使用场景不需要

#include <string>
#include <cstddef>

namespace tools {
namespace charset {

const size_t kMaxStrLen = 512;

// UTF-8 转 Unicode
// 参数:
//      str8: UTF-8 字符串
//      unicode_str: Unicode 字符串
// 返回:
//      返回是否成功
bool Utf8ToUnicode(const std::string &str8,
                   std::u32string &unicode_str);

// UTF-16 转 Unicode
// 参数:
//      str16: UTF-16 字符串
//      unicode_str: Unicode 字符串
//      is_be: 是否是大头
// 返回:
//      返回是否成功
bool Utf16ToUnicode(const std::u16string &str16,
                    std::u32string &unicode_str,
                    bool is_be=true);

// UTF-32 转 Unicode
// 参数:
//      str32: UTF-32 字符串
//      unicode_str: Unicode 字符串
//      is_be: 是否是大头
// 返回:
//      返回是否成功
bool Utf32ToUnicode(const std::u32string &str32,
                    std::u32string &unicode_str,
                    bool is_be=true);

// Unicode 转 UTF-8
// 参数:
//      unicode_str: Unicode 字符串
//      str8: UTF-8 字符串
// 返回:
//      返回是否成功
bool UnicodeToUtf8(const std::u32string &unicode_str,
                   std::string &str8);

// Unicode 转 UTF-16
// 参数:
//      unicode_str: Unicode 字符串
//      str16: UTF-16 字符串
//      is_be: 是否是大头
// 返回:
//      返回是否成功
bool UnicodeToUtf16(const std::u32string &unicode_str,
                    std::u16string &str16,
                    bool is_be=true);

// Unicode 转 UTF-32
// 参数:
//      unicode_str: Unicode 字符串
//      str32: UTF-32 字符串
// 返回:
//      返回是否成功
bool UnicodeToUtf32(const std::u32string &unicode_str,
                    std::u32string &str32,
                    bool is_be=true);

} // end of namespace charset
} // end of namespace tools

#endif // OLIME_TOOLS_CHARSET_H_
