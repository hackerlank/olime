#include <fstream>
#include <iostream>
#include <map>
#include "trie.h"
#include "charset.h"


int main() {
    std::fstream fs;
    fs.open("trie.in", std::fstream::binary | std::fstream::in);
    dicts::Trie t1;
    t1.Build(fs);
    t1.Dump("dic1");
    uint32_t value=0;
    std::u32string u32key;
    ::tools::charset::Utf8ToUnicode("b", u32key);
    bool ret = t1.ExactLookup(u32key, value);
    std::cout << int(ret) << " " << value << std::endl;
    fs.close();

    dicts::Trie t2;
    ret = t2.Load("dic1");
    std::cout << int(ret) << std::endl;
    value = 0;

    ::tools::charset::Utf8ToUnicode("ab", u32key);
    ret = t2.ExactLookup(u32key, value);
    std::cout << int(ret) << " " << value << std::endl;

    ::tools::charset::Utf8ToUnicode("abc", u32key);
    std::map<std::u32string, uint32_t> values;
    ret = t2.PrefixLookup(u32key, values);
    std::cout << int(ret) << " " << value << std::endl;

    return 0;
}
