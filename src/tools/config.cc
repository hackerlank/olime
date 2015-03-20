#include <fstream>
#include "config.h"

namespace tools {
namespace config {


std::string Parser::Trim(const std::string& src_str, const std::string& invalid_chars) {
    size_t beg_index = src_str.find_first_not_of(invalid_chars);
    size_t end_index = src_str.find_last_not_of(invalid_chars);
    if (std::string::npos == beg_index || std::string::npos == end_index) {
        return std::string("");
    } else {
        return src_str.substr(beg_index, end_index - beg_index + 1);
    }
}


bool Parser::Load(const std::string& ini_filename) {
    std::fstream f_ini(ini_filename.c_str());
    if (! f_ini) {
        return false;
    }

    std::string line;
    std::string section("unknown_section");
    while (getline(f_ini, line)) {
        // 去除注释
        size_t sharp_index = line.find_first_of("#");
        if (std::string::npos != sharp_index) {
            line = line.substr(0, sharp_index);
        }

        line = Trim(line);

        if (line[0] == '#') { // 注释
            continue;
        } else if ('[' == line[0] && ']' == line[line.length() - 1] &&
                line.length() - 1 == line.find_first_of("[]", 1)) { // section
            section = line.substr(1, line.length() - 2);
            section = Trim(section);
            if (section_dict_.end() == section_dict_.find(section)) {
                section_dict_[section] = std::map<std::string, std::string>();
            }
        } else {
            size_t equal_index = line.find('=');
            if (std::string::npos == equal_index) { // 无效输入
                continue;
            }
            std::string key = line.substr(0, equal_index);
            std::string value = line.substr(equal_index + 1, line.length() - equal_index - 1);
            key = Trim(key);
            value = Trim(value);
            section_dict_[section][key] = value;
        }

    }

    f_ini.close();
    return true;
}


std::map<std::string, std::string> Parser::operator[](const std::string& section_name) {
    return section_dict_[section_name];
}


} // end of the namespace config
} // end of the namespace tools
