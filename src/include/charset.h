#ifndef OLIME_TOOLS_CHARSET_H_
#define OLIME_TOOLS_CHARSET_H_
// 用来支持字符集之间的转换
// Unicode 各种编码方式的方法请看:
// http://baike.baidu.com/view/40801.htm
// 现支持 Unicode 转 UTF-8, UTF-16, UTF-32
// TODO: 还可以支持 BOM, 但是现在的使用场景不需要

#include "define.h"

namespace tools {
namespace charset {

// UTF-8 转 Unicode
// param[in] str8: UTF-8 字符串
// param[out] unicode_str: Unicode 字符串
// return: 是否成功
bool Utf8ToUnicode(const u8string &str8, u32string &unicode_str);

// UTF-16 转 Unicode
// param[in] str16: UTF-16 字符串
// param[out] unicode_str: Unicode 字符串
// param[in] is_be: 是否是大头
// return: 是否成功
bool Utf16ToUnicode(const u16string &str16, u32string &unicode_str,
        bool is_be=true);

// UTF-32 转 Unicode
// param[in] str32: UTF-32 字符串
// param[out] unicode_str: Unicode 字符串
// param[in] is_be: 是否是大头
// return: 是否成功
bool Utf32ToUnicode(const u32string &str32, u32string &unicode_str,
        bool is_be=true);

// Unicode 转 UTF-8
// param[in] unicode_str: Unicode 字符串
// param[out] str8: UTF-8 字符串
// return: 是否成功
bool UnicodeToUtf8(const u32string &unicode_str, u8string &str8);

// Unicode 转 UTF-16
// param[in] unicode_str: Unicode 字符串
// param[out] str16: UTF-16 字符串
// param[in] is_be: 是否是大头
// return: 是否成功
bool UnicodeToUtf16(const u32string &unicode_str, u16string &str16,
        bool is_be=true);

// Unicode 转 UTF-32
// param[in] unicode_str: Unicode 字符串
// param[out] str32: UTF-32 字符串
// return: 是否成功
bool UnicodeToUtf32(const u32string &unicode_str, u32string &str32,
        bool is_be=true);

} // end of namespace charset
} // end of namespace tools

#endif // OLIME_TOOLS_CHARSET_H_
