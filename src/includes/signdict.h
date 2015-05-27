#ifndef OLIME_DICTS_SIGNDICT_H_
#define OLIME_DICTS_SIGNDICT_H_

#include <cmath>
#include <fstream>
#include "define.h"

namespace dicts {


// 用于 dumps 到文件时的头信息
struct SignDictHeader {
    uint64_t magic_num;
    uint64_t items_num;
    uint32_t value_bytes;
    uint32_t sign_bytes;
    uint32_t prefix_bytes;
};

// 变长的缓存类, 定义了一些常用功能.
class VarBuff {

private:
    uint8_t* buff_; // 存储空间
    uint64_t items_num_; // 总条目数
    uint32_t item_bytes_; // 每个条目占用多少字节
    uint64_t max_value_; // 条目的最大值
    uint64_t magic_num_; // 用来判断文件的魔数

public:
    // 负责申请空间, 存储各种参数
    VarBuff(uint64_t items_num, uint32_t item_bytes) {
        items_num_ = items_num;
        item_bytes_ = item_bytes;
        max_value_ = pow(2, item_bytes_ * 8) - 1;
        buff_ = new uint8_t[items_num * item_bytes];
        memset(buff_, 0, items_num * item_bytes);
    }

    // 删除空间
    virtual ~VarBuff() {
        if (buff_) {
            delete buff_;
        }
    }

    // 从文件读取数据
    void Loads(std::ifstream &infile) {
        infile.read((char*)buff_, items_num_ * item_bytes_);
    }

    // 写入到文件
    void Dumps(std::ofstream &outfile) {
        outfile.write((char*)buff_, items_num_ * item_bytes_);
    }

    // 获取值
    ErrCode Get(uint64_t item_index, uint8_t* item_value) {
        if (item_index > items_num_ || NULL == item_value) {
            return ErrInvalidParams;
        }
        memcpy(item_value, buff_ + item_index * item_bytes_, item_bytes_);
        return ErrSuccess;
    }

    // 设定值
    ErrCode Set(uint64_t item_index, const uint8_t* item_value) {
        if (item_index > items_num_ || NULL == item_value) {
            return ErrInvalidParams;
        }
        memcpy(buff_ + item_index * item_bytes_, item_value, item_bytes_);
        return ErrSuccess;
    }

    // 直接比较, 省去了复制一份的性能消耗
    int Cmp(uint64_t item_index, const uint8_t* other_value) {
        return memcmp(buff_ + item_index * item_bytes_, other_value,
                item_bytes_);
    }

    // 直接返回一个 uint64_t 的数据
    ErrCode GetUint64(uint64_t item_index, uint64_t &item_value) {
        if (item_bytes_ > 8) {
            return ErrFailure;
        }

        if (item_index > items_num_) {
            return ErrInvalidParams;
        }

        item_value = 0;
        uint8_t* item_buff = buff_ + item_index * item_bytes_;
        for (size_t i = 0; i < item_bytes_; ++i) {
            item_value <<= 8;
            item_value |= item_buff[i];
        }
        return ErrSuccess;
    }

    // 直接插入一个 uint64_t 的数据
    // 数据将大头存储
    ErrCode SetUint64(uint64_t item_index, uint64_t item_value) {
        if (item_value > max_value_ || item_index >= items_num_) {
            return ErrInvalidParams;
        }

        uint8_t *item_buff = buff_ + item_index * item_bytes_;
        for (size_t i = 0; i < item_bytes_; ++i) {
            item_buff[i] = 0x00FF & item_value;
            item_value >>= 8;
        }
        return ErrSuccess;
    }

};

// SignDict 本体
class SignDict {

private:
    uint64_t items_num_; // signdict 的总条目数

    uint32_t prefix_bytes_; // 每个前缀的字节数
    uint32_t suffix_bytes_; // 每个后缀的字节数
    uint32_t value_bytes_; // 每个值的字节数
    uint32_t sign_bytes_; // 每个 sign 的字节数
    uint32_t prefix_value_bytes_; // 根据词条数目决定前缀缓存中每元素所占字节数
    uint64_t magic_num_; // 用来确认文件的魔数, 请不要修改

    VarBuff* prefix_buff_; // 前缀的缓存头指针
    VarBuff* suffix_buff_; // 后缀的缓存头指针
    VarBuff* value_buff_; // 值的缓存头指针

    uint64_t prefix_size_; // 前缀总长度

    uint64_t index_prefix_; // 用于插入操作中, 标识上一次插入的前缀位置
    uint64_t index_suffix_; // 用于插入操作中, 标志上一次插入的后缀位置

public:

    // 默认构造函数, 从文件构建时使用
    SignDict();

    // 初始化 SignDict, 参数中的数据必须给出, 其余克通过计算获得。
    // param[in] items_num: signdict 的总条目数
    // param[in] value_bytes: 每个值的字节数
    // param[in] sign_bytes: 每个 sign 的字节数
    // param[in] prefix_bytes: 每个前缀的字节数
    SignDict(uint64_t items_num, uint32_t value_bytes, uint32_t sign_bytes,
            uint32_t prefix_bytes=0);

    // 删除 SignDict 申请的空间
    virtual ~SignDict();

    // 创建一个 SignDict, 包括各种参数的计算与存储, 空间申请等.
    ErrCode Create();

    // 查找 sign 是否存在, 如果存在保存到 value_ret 中.
    // param[in] sign: 用于查找的 sign, 等效于 key.
    // param[out] value: 存储查找到的值
    ErrCode Search(const uint8_t* sign, uint8_t* value);

    // 插入 sign-value 到 SignDict 中. 所有的插入必须是对于 sign 有序的.
    // param[in] sign: 用于插入的 sign, 等效于 key.
    // param[in] value: 用于插入的 value 值.
    ErrCode Insert(const uint8_t* sign, const uint8_t* value);

    // 全部插入完成后需要把 prefix_buff_ 的最后一个元素添加一个值
    void InsertFinish();

    // 将数据保存到文件
    // param[in] filename: 文件名
    void Dumps(const std::string& filename);

    // 从文件中读取数据
    // param[in] filename: 文件名
    ErrCode Loads(const std::string& filename);

private:

    // 计算前缀所占的字节数, 改结果来源于经验值
    uint64_t calcPrefixBytes() const;

    // 折半查找
    // param[in] sign_suffix:  作为 key 的后缀 sign.
    // param[in] beg_index: suffix_buff_ 中待查找的起始位置
    // param[in] end_index: suffix_buff_ 中带查找的结束位置
    // param[out] target_index: 如果找到用来存放找到的位置
    ErrCode binSearch(const uint8_t* sign_suffix, uint64_t beg_index,
            uint64_t end_index, uint64_t &target_index);

    // 获取给定 sign 的前后缀
    // param[in] sign: 给定 sign
    // param[out] prefix: 前缀的值
    // param[out] suffix: 后缀的值
    void getPrefixAndSuffix(const uint8_t* sign, uint64_t &prefix,
            uint8_t* suffix);

    // 计算前缀缓存中每个条目的字节数
    // return: 返回字节数
    uint32_t calcPrefixValueBytes() const;
};


} // end of namespace dicts

#endif
