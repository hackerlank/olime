#ifndef OLIME_TOOLS_CONFIG_H_
#define OLIME_TOOLS_CONFIG_H_
// 用来处理 ini 文件
// ini 文件仅支持如下 3 种词条样式:
// 1. 第一个字符是 #
// 2. 以方括号包住的字符串 (字符串中不能再含有方括号)
// 3. 以 key = value 形式存在的 pair, 以 *第一个等于号* 作为分隔符
// P.S. 行首尾的空格, Tab, 回车都会被去掉
//
// !! 注意 !!
// 1. 因为没做代码层面的配置容错, 所以不存在的配置项不会报警告知.
//    请在写配置文件的时候写清楚


#include <string>
#include <map>


namespace tools {
namespace config {


class Parser {

private:

    // 用来储存数据的, 所有的数据都是字符串
    // 格式为:
    //  section_dict_ = {
    //      section1: {key1: value1, key2: value2, ...},
    //      section2: {key1: value1, key2: value2, ...},
    //      ...
    //  }
    std::map< std::string, std::map<std::string, std::string> > section_dict_;

    // 处理字符串的首尾无效字符
    // param[in] src_str: 源字符串
    // param[in] invalid_chars: 定义为无效的字符
    // return: 返回已经去除首尾无效字符的字符串
    std::string Trim(const std::string& src_str, const std::string& invalid_chars="\t\n ");

public:

    // 获取对应 section 的结果
    // param[in] section: section 的名称
    // return: 返回一个 section 的结果
    //         如果 section 不存在也不会报错, 只会返回一个空 map
    std::map<std::string, std::string> operator[](const std::string& section);

    // 读取配置文件, 同时会生成 section_dict_
    // param[in] filename: 文件名称 (包含路径, 仅支持 ini 文件格式)
    // return: 是否读取成功
    bool Load(const std::string &filename);
};


} // end of the namespace config
} // end of the namespace tools


#endif // OLIME_TOOLS_CONFIG_H_
