#include <fstream>
#include <iostream>
#include <map>
#include "words_dict.h"
#include "charset.h"


int main() {
    // 待 build 的文件
    std::fstream fs;
    fs.open("words_dict.in", std::fstream::binary | std::fstream::in);

    // build
    dicts::WordsDict d1;
    d1.Build(fs);
    fs.close();

    // get ids
    std::u32string pron;
    tools::charset::Utf8ToUnicode("あさひし", pron);
    std::vector<uint32_t> ids;
    d1.ExactGetIds(pron, ids);
    for (size_t i = 0; i < ids.size(); ++i) {
        std::cout << "id: " << ids[i] << std::endl;
    }

    // get word
    dicts::Word word;
    d1.GetWord(ids[0], word);
    std::string u8word;
    tools::charset::UnicodeToUtf8(word.word_, u8word);
    std::cout << "word: " << u8word << " cls: " << word.cls_ << " cost: " << word.cost_ << std::endl;

    // dump
    d1.Dump("trie.dat", "words.dat", "ids.dat");

    // load
    dicts::WordsDict d2;
    d2.Load("trie.dat", "words.dat", "ids.dat");

    // get ids
    pron.clear();
    tools::charset::Utf8ToUnicode("ふくい", pron);
    ids.clear();
    d2.ExactGetIds(pron, ids);
    for (size_t i = 0; i < ids.size(); ++i) {
        std::cout << "id: " << ids[i] << std::endl;
    }

    d2.GetWord(3, word);
    tools::charset::UnicodeToUtf8(word.word_, u8word);
    std::cout << "word: " << u8word << " cls: " << word.cls_ << " cost: " << word.cost_ << std::endl;

    // prefix get ids
    std::map<uint32_t, uint32_t> prefix_ids;
    d2.PrefixGetIds(pron, prefix_ids);
    std::cout << "size: " << prefix_ids.size() << std::endl;
    for (std::map<uint32_t, uint32_t>::iterator it = prefix_ids.begin();
            it != prefix_ids.end(); ++it) {
        uint32_t id = it->first;
        uint32_t end_pos = it->second;
        d2.GetWord(id, word);
        tools::charset::UnicodeToUtf8(word.word_, u8word);
        std::cout << id << ", " << end_pos << " word: " << u8word << " cls: " << word.cls_ << " cost: " << word.cost_ << std::endl;
    }




    return 0;
}
