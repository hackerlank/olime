#include <string>
#include <sstream>
#include <fstream>
#include "define.h"
#include "trie.h"
#include "charset.h"
#include <iostream>


dicts::Trie::Trie() {
    dic_ = NULL;
}


dicts::Trie::~Trie() {
    DELETE(dic_);
}


bool dicts::Trie::Build(std::istream& utf8_is) {
    dic_ = new dawgdic::Dictionary;

    dawgdic::DawgBuilder builder;
    std::string line;
    while (std::getline(utf8_is, line)) {
        std::string u8key;
        std::u32string u32key;
        uint32_t value;

        std::stringstream ss(line);
        ss >> u8key >> value;
        ::tools::charset::Utf8ToUnicode(u8key, u32key);
        builder.Insert(u32key.c_str(), value);
    }
    dawgdic::Dawg dawg;
    builder.Finish(&dawg);

    dawgdic::DictionaryBuilder::Build(dawg, dic_);

    return true;
}


bool dicts::Trie::Dump(const std::string& dic_file) {
    if (dic_ && dic_file.length() <= 0) {
        return false;
    }

    std::fstream dic_fs;
    dic_fs.open(dic_file, std::fstream::binary | std::fstream::out);
    dic_->Write(&dic_fs);
    dic_fs.close();

    return true;
}


bool dicts::Trie::Load(const std::string& dic_file) {
    if (dic_file.length() <= 0) {
        return false;
    }

    DELETE(dic_);
    dic_ = new dawgdic::Dictionary;
    std::fstream dic_fs;
    dic_fs.open(dic_file, std::fstream::binary | std::fstream::in);
    dic_->Read(&dic_fs);
    dic_fs.close();

    return true;
}


bool dicts::Trie::ExactLookup(const std::u32string& key, uint32_t& value) {
    return dic_->Find(key.c_str(), &value);
}


bool dicts::Trie::PrefixLookup(const std::u32string& key,
        std::map<std::u32string, uint32_t>& values) {

    size_t beg_index = 0;
    dawgdic::BaseType index = dic_->root();
    for (size_t end_index = 1; end_index <= key.length(); ++end_index) {
        std::u32string part = key.substr(beg_index, end_index - beg_index);
        dic_->Follow(part.c_str(), &index);
        if (dic_->has_value(index)) {
            std::string u8;
            tools::charset::UnicodeToUtf8(key.substr(beg_index, end_index - beg_index), u8);
            std::cout << "[" << beg_index << ":" << end_index << "] " << u8 << "\t" << dic_->value(index) << std::endl;

            beg_index = end_index;
        }
    }
    return true;
}
