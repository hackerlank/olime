#ifndef OLIME_DICTS_TRIE_H_
#define OLIME_DICTS_TRIE_H_

#include <map>
#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary-builder.h"

namespace dicts {


struct Item {
    std::u32string word;
    uint32_t cls;
    uint32_t cost;
};


class Trie {

private:
    dawgdic::Dictionary *dic_; // 词典文件

public:
    // 构造函数
    Trie();

    // 析构函数
    ~Trie();

    // 建立词典 (可以选择是否要建立引导)
    // 参数:
    //      is: 输入文本,  必须使用 UTF-8 编码, 格式为
    //          key [\tab] value
    //          其中, key 必须是字符串, value 必须是 uint32_t 能 cover 的正整数
    //          key 必须要有序!!!
    //          代码中不会进行数据监测, 请事先监测
    // 返回:
    //      返回是否建立成功
    bool Build(std::istream& utf8_is);

    // 导出词典和引导到文件中
    // 参数:
    //      dic_file: 词典文件的文件名
    // 返回:
    //      返回是否导出成功
    bool Dump(const std::string& dic_file);

    // 读取词典和引导到文件中
    // 参数:
    //      dic_file: 词典文件的文件名
    // 返回:
    //      返回是否读取成功
    bool Load(const std::string& dic_file);

    // 查找对应 key 的 value
    // 参数:
    //      key: Unicode 字符串
    //      value: key 对应的值
    // 返回:
    //      返回是否找到了结果
    bool ExactLookup(const std::u32string& key, uint32_t& value);

    // 前缀查找. 例如:
    //      完整 key 为 abcde, 会查找: a, ab, abc, abcd, abcde 下包含的 value
    //      返回的结果则是 {"a": 1, "ab": 2, ...}
    // 参数:
    //      key: Unicode 的完整 key
    //      values: 找到的 key-value 对
    // 返回:
    //      返回是否找到了结果
    bool PrefixLookup(const std::u32string& key,
            std::map<std::u32string, uint32_t>& values);
};


} // end of namespace dicts

#endif
