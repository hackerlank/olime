#ifndef OLIME_INCLUDE_TYPEDEF_H_
#define OLIME_INCLUDE_TYPEDEF_H_
/*
 * 定义一些项目中会用到的类型
 * 在以往的编码过程中总会出现某个定义的类型或者变量找不到问题,
 * 本文件旨在将所有的定义都放在这一个文件里面, 让维护代码的人找起来方便
 */

#include <stdint.h>
#include <string>

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
#define DELETE(x) if((x)) {delete (x); (x) = NULL;}
#endif


#endif // OLIME_INCLUDE_TYPEDEF_H_
