#ifndef OLIME_INCLUDE_TYPEDEF_H_
#define OLIME_INCLUDE_TYPEDEF_H_
// 定义一些项目中会用到的类型

#include <stdint.h>
#include <string>

//
// 字符类型
//
typedef uint32_t u32char;
typedef uint16_t u16char;
typedef char u8char;

// u32string, u16string 均是 C++ 11 支持的类型, 但是我们的版本太老了, 暂时先这么用着
// u8string 实际上就是 string, 但是为了和 string 区分开, 重新定义了一个
// 字符串类型, 分别对应每个字符占用 32, 16, 8 比特
// 因为代码中所有的编码类型均使用Unicode (请不要使用GB18030之类的国标码)
typedef std::basic_string<u32char> u32string;
typedef std::basic_string<u16char> u16string;
typedef std::basic_string<u8char> u8string;


//
// 定义的常量
//
const size_t kMaxStrLen = 256; // 分配空间时最大的字符串长度


//
// 错误码的返回结果
//
enum ErrCode {
    ErrSuccess = 0, // 成功, 没有错误
    ErrFailure, // 失败, 具体原因不需说明
    ErrInvalidParams, // 非法的参数
    ErrInternalErr, // 内部错误, 不便明确表示的错误

    // SignDict
    ErrSignDictFull, // signdict 已经满了
    ErrSignDictDisorderedKey, // signdict 插入时必须按从小到大的顺序, 顺序不对时报该错误
    ErrSignDictDuplicateKey, // signdict 使用了相同的 sign, 数据不能同一个 sign, 也不能有重复项
    ErrSignDictInvalidFile, // signdict 读取文件时文件魔数不匹配
};


//
// 定义一些常用的宏
//
#ifndef DELETE
#define DELETE(x) if(NULL != (x)) {delete (x); (x) = NULL;}
#endif


#endif // OLIME_INCLUDE_TYPEDEF_H_
