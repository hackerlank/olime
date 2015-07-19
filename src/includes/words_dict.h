#ifndef OLIME_DICTS_WORDDICT_H_
#define OLIME_DICTS_WORDDICT_H_

#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary-builder.h"

namespace dicts {


struct Word {
    // 用来存储 "词" 的结构体

    // 词条内容
    std::u32string word_;
    // 词性 id
    uint32_t cls_;
    // cost
    uint32_t cost_;

    // 默认构造函数
    Word(){};

    // 构造函数, 负责传入值
    // 参数:
    //      word: 词条
    //      cls: 词性 id
    //      cost: cost 数值
    Word(const std::u32string& word, uint32_t cls, uint32_t cost) :
        word_(word), cls_(cls), cost_(cost) {}

    // deep copy
    Word& operator=(const Word& other) {
        word_ = other.word_;
        cls_ = other.cls_;
        cost_ = other.cost_;
        return *this;
    }
};


class WordsDict {

private:
    // trie 树, 类似于 {发音: ids_list_ 偏移量}
    dawgdic::Dictionary *trie_;
    // 存放 Word 对象的数组
    std::vector<Word> *words_;
    // 每个 item 都是某发音下的词条 ids
    // [ [发音1对应的词的 ids], [发音2对应的词的 ids], ... ]
    std::vector< std::vector<uint32_t> > *ids_list_;

public:
    // 构造函数
    WordsDict();

    // 析构函数
    ~WordsDict();

    // 建立词典, 只使用了 trie 树部分, dawgdic 实际上还支持 complete 功能
    // 参数:
    //      is: 输入文本,  必须使用 UTF-8 编码, 格式为
    //          发音 [\tab] 词条 [\tab] 词性 id [\tab] cost
    //          词条会自动编号, 从 0 开始, 第1条就是 id 为 0
    //          其中, 发音, 词条必须是字符串,  词性 id, cost 必须是 uint32_t
    //          key 必须要有序!!!
    //          代码中不会进行数据监测, 请事先监测
    // 返回:
    //      返回是否建立成功
    bool Build(std::istream& is);

    // 通过发音准确获取 ids 的 vector
    // 参数:
    //      pron: 发音, Unicode 字符串
    //      ids: 获取到的 ids 列表
    // 返回:
    //      返回是否存在
    bool ExactGetIds(const std::u32string& pron, std::vector<uint32_t>& ids);

    // 通过 id 获取词条信息
    // 参数:
    //      id: id 号
    //      word: 词条内容, Word
    // 返回:
    //      返回找到与否
    bool GetWord(uint32_t id, Word& word);

    // 导出词典和引导到文件中
    // 参数:
    //      trie_file: trie 文件的文件名
    //      words_file: words 文件的文件名
    //      ids_file: ids 文件的文件名
    // 返回:
    //      返回是否导出成功
    bool Dump(const std::string& trie_file,
        const std::string& words_file, const std::string& ids_file);

    // 读取词典和引导到文件中
    // 参数:
    //      trie_file: trie 文件的文件名
    //      words_file: words 文件的文件名
    //      ids_file: ids 文件的文件名
    // 返回:
    //      返回是否读取成功
    bool Load(const std::string& trie_file,
        const std::string& words_file, const std::string& ids_file);

    // 前缀查找, 例如:
    //      完整发音为 abcde, 会查找: a, ab, abc, abcd, abcde 下包含的词
    //      返回的结果则是 {词1:  词1发音长度,  词2:  词2发音长度, ...}
    // 参数:
    //      pron: Unicode 的完整发音
    //      ids: 词-发音长度的 pair
    // 返回:
    //      返回是否找到了结果
    bool PrefixGetIds(const std::u32string& pron,
            std::map<uint32_t, uint32_t>& ids);
};


} // end of namespace dicts

#endif
