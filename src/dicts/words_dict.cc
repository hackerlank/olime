#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <cstring>
#include "msgpack.hpp"
#include "words_dict.h"
#include "define.h"
#include "charset.h"


dicts::WordsDict::WordsDict() {
    trie_ = NULL;
    words_ = NULL;
    ids_list_ = NULL;
}


dicts::WordsDict::~WordsDict() {
    DELETE(trie_);
    DELETE(words_);
    DELETE(ids_list_);
}


bool dicts::WordsDict::Build(std::istream& is) {
    trie_ = new dawgdic::Dictionary;
    words_ = new std::vector<Word>;
    ids_list_ = new std::vector< std::vector<uint32_t> >;

    dawgdic::DawgBuilder builder;
    std::string line;
    // 词的 id 标志
    size_t word_id = 0;
    // 发音映射到 (多个) 词的 ids 的新的偏移量
    size_t new_offset = 0;
    // {发音: (多个) 词的 ids 的列表}
    std::map<std::u32string, uint32_t> pron_offset_dict;
    // 整个流程是这样的:
    // 依据发音到 trie_ 中查找到存储词的 ids 的列表的偏移量
    // 利用该偏移量到 ids_list_ 中查找到所有的 id
    // 通过 id 到 words_ 中 获取词的信息
    while (std::getline(is, line)) {
        std::string pron, word;
        std::u32string u_pron, u_word;
        uint32_t cls, cost;
        // 添加资源到 trie 树, 每行只有4个元素
        char* pch = strtok((char*)line.c_str(), "\t");
        // pron
        pron.assign(pch);
        pch = strtok(NULL, "\t");
        // word
        word.assign(pch);
        // cls
        std::stringstream ss;
        pch = strtok(NULL, "\t");
        ss << pch;
        ss >> cls;
        // cost
        ss.clear();
        pch = strtok(NULL, "\t");
        ss << pch;
        ss >> cost;

        tools::charset::Utf8ToUnicode(pron, u_pron);
        tools::charset::Utf8ToUnicode(word, u_word);

        // 本次插入的 offset
        uint32_t inserted_offset;
        // 发音不存在, 插入一个新的
        if (pron_offset_dict.find(u_pron) == pron_offset_dict.end()) {
            inserted_offset = new_offset++;
            pron_offset_dict[u_pron] = inserted_offset;
            std::vector<uint32_t> ids;
            ids.push_back(word_id);
            ids_list_->push_back(ids);
        } else { // 发音存在
            inserted_offset = pron_offset_dict[u_pron];
            (*ids_list_)[inserted_offset].push_back(word_id);
        }
        if (! builder.Insert(u_pron.c_str(), inserted_offset)) {
            return false;
        }

        // 添加资源到 words
        Word word_obj(u_word, cls, cost);
        words_->push_back(word_obj);
        ++word_id;
    }
    dawgdic::Dawg dawg;
    builder.Finish(&dawg);

    dawgdic::DictionaryBuilder::Build(dawg, trie_);

    return true;
}


bool dicts::WordsDict::ExactGetIds(const std::u32string& pron,
        std::vector<uint32_t>& ids) {

    uint32_t ids_offset;
    bool found = trie_->Find(pron.c_str(), &ids_offset);
    ids = (*ids_list_)[ids_offset];
    return found;
}


bool dicts::WordsDict::GetWord(uint32_t id, Word& word) {
    if (id >= words_->size()) {
        return false;
    }
    word = (*words_)[id];
    return true;
}


bool dicts::WordsDict::Dump(const std::string& trie_file,
        const std::string& words_file, const std::string& ids_file) {

    if ((! trie_) || (! words_) || (! ids_list_)
            || trie_file.length() <= 0 || words_file.length() <= 0
            || ids_file.length() <= 0) {
        return false;
    }

    // trie 文件
    std::fstream trie_fs;
    trie_fs.open(trie_file, std::fstream::binary | std::fstream::out);
    trie_->Write(&trie_fs);
    trie_fs.close();

    // words 文件
    std::fstream words_fs;
    words_fs.open(words_file, std::fstream::binary | std::fstream::out);
    // 创建一个打包流对象
    msgpack::sbuffer words_buffer;
    msgpack::packer<msgpack::sbuffer> words_packer(&words_buffer);
    // 数据格式化为 msgpack 支持的类型
    for (std::vector<Word>::iterator it = words_->begin();
            it != words_->end(); ++it) {
        std::string u8word;
        if (ErrSuccess != tools::charset::UnicodeToUtf8(it->word_, u8word)) {
            return false;
        }
        msgpack::type::tuple<std::string, uint32_t, uint32_t> item(
            u8word, it->cls_, it->cost_);
        words_packer.pack(item);
    }
    // 写入文件
    words_fs.write(words_buffer.data(), words_buffer.size());
    words_fs.close();

    // ids 文件
    std::fstream ids_fs;
    ids_fs.open(ids_file, std::fstream::binary | std::fstream::out);
    msgpack::sbuffer ids_buffer;
    msgpack::packer<msgpack::sbuffer> ids_packer(&ids_buffer);
    for (std::vector< std::vector<uint32_t> >::iterator it = ids_list_->begin();
            it != ids_list_->end(); ++it) {
        ids_packer.pack(*it);
    }
    ids_fs.write(ids_buffer.data(), ids_buffer.size());
    ids_fs.close();

    return true;
}


bool dicts::WordsDict::Load(const std::string& trie_file,
        const std::string& words_file, const std::string& ids_file) {

    if (trie_file.length() <= 0 || words_file.length() <= 0
            || ids_file.length() <= 0) {
        return false;
    }

    // 创建新的对象
    DELETE(trie_);
    DELETE(words_);
    DELETE(ids_list_);
    trie_ = new dawgdic::Dictionary;
    words_ = new std::vector<Word>;
    ids_list_ = new std::vector< std::vector<uint32_t> >;

    // 读取 trie 树
    std::fstream trie_fs;
    trie_fs.open(trie_file, std::fstream::binary | std::fstream::in);
    trie_->Read(&trie_fs);
    trie_fs.close();

    // 读物 words
    std::fstream words_fs;
    words_fs.open(words_file, std::fstream::binary | std::fstream::in);
    // 获取文件大小
    words_fs.seekp(0, words_fs.end);
    size_t words_file_size = words_fs.tellg();
    words_fs.seekp(0, words_fs.beg);
    // 创建 unpacker 对象
    msgpack::unpacker words_unpacker;
    words_unpacker.reserve_buffer(words_file_size);
    // 读取文件至 unpacker, 解包
    words_fs.read(words_unpacker.buffer(), words_file_size);
    words_unpacker.buffer_consumed(words_file_size);
    // 生成 words_
    msgpack::unpacked temp_obj;
    while (words_unpacker.next(&temp_obj)) {
        auto temp_data = temp_obj.get();
        msgpack::type::tuple<std::string, uint32_t, uint32_t> item;
        temp_data.convert(&item);
        // 获取元素
        std::string u8word = msgpack::type::get<0>(item);
        uint32_t cls = msgpack::type::get<1>(item);
        uint32_t cost = msgpack::type::get<2>(item);
        // 转码
        std::u32string word;
        tools::charset::Utf8ToUnicode(u8word, word);
        words_->push_back(Word(word, cls, cost));
    }
    words_fs.close();

    // 生成 ids_list_
    std::fstream ids_fs;
    ids_fs.open(ids_file, std::fstream::binary | std::fstream::in);
    ids_fs.seekp(0, ids_fs.end);
    size_t ids_file_size = ids_fs.tellg();
    ids_fs.seekp(0, ids_fs.beg);
    // 创建 unpacker 对象
    msgpack::unpacker ids_unpacker;
    ids_unpacker.reserve_buffer(ids_file_size);
    // 读取文件至 unpacker, 解包
    ids_fs.read(ids_unpacker.buffer(), ids_file_size);
    ids_unpacker.buffer_consumed(ids_file_size);
    while (ids_unpacker.next(&temp_obj)) {
        auto temp_data = temp_obj.get();
        std::vector<uint32_t> item;
        temp_data.convert(&item);
        ids_list_->push_back(item);
    }
    ids_fs.close();

    return true;
}

bool dicts::WordsDict::PrefixGetIds(const std::u32string& pron,
        std::map<uint32_t, uint32_t>& ids) {

    dawgdic::BaseType index = trie_->root();
    uint32_t beg_index = 0;
    for (uint32_t end_index = 1; end_index <= pron.length(); ++end_index) {
        // 本次新增的字符
        std::u32string part = pron.substr(beg_index, end_index - beg_index);
        trie_->Follow(part.c_str(), &index);
        if (trie_->has_value(index)) {
            // 获取该发音下的所有 id
            uint32_t offset = trie_->value(index);
            std::vector<uint32_t>& cur_ids = (*ids_list_)[offset];
            // 保存至 ids 中
            for (std::vector<uint32_t>::iterator it = cur_ids.begin();
                    it != cur_ids.end(); ++it) {
                ids[*it] = end_index;
            }
            // 下一轮
            beg_index = end_index;
        }
    }
    return true;
}
