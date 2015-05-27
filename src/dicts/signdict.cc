#include <fstream>
#include "signdict.h"


dicts::SignDict::SignDict() {
    items_num_ = 0;
    prefix_bytes_ = 0;
    sign_bytes_ = 0;
    value_bytes_ = 0;
    prefix_buff_ = NULL;
    suffix_buff_ = NULL;
    value_buff_ = NULL;
    index_prefix_ = 0;
    index_suffix_ = 0;
    magic_num_ = 82901661;
}

dicts::SignDict::SignDict(uint64_t items_num, uint32_t value_bytes,
        uint32_t sign_bytes, uint32_t prefix_bytes) {

    items_num_ = items_num;
    prefix_bytes_ = prefix_bytes;
    sign_bytes_ = sign_bytes;
    value_bytes_ = value_bytes;
    prefix_buff_ = NULL;
    suffix_buff_ = NULL;
    value_buff_ = NULL;
    index_prefix_ = 0;
    index_suffix_ = 0;
    magic_num_ = 82901661;
}

dicts::SignDict::~SignDict() {
    DELETE(prefix_buff_)
    DELETE(suffix_buff_)
    DELETE(value_buff_)
}

inline uint64_t dicts::SignDict::calcPrefixBytes() const {
    if (items_num_ < 261000u) {
        return 1u;
    } else if (items_num_ < 67000000u) {
        return 2u;
    } else {
        return 3u;
    }
}

inline uint32_t dicts::SignDict::calcPrefixValueBytes() const {
    if (items_num_ <= UINT8_MAX) {
        return 1u;
    } else if (items_num_ <= UINT16_MAX) {
        return 2u;
    } else if (items_num_ <= UINT32_MAX) {
        return 4u;
    } else {
        return 8u;
    }
}

ErrCode dicts::SignDict::Create() {
    if (4 <= prefix_bytes_ || sign_bytes_ <= prefix_bytes_) {
        return ErrInvalidParams;
    }

    // 如果用户没有指定前缀字节数，那么就计算一个。
    if (0 == prefix_bytes_) {
        prefix_bytes_ = calcPrefixBytes();
    }

    // 计算各种参数
    prefix_size_ = (1 << (8 * prefix_bytes_)) + 1;
    suffix_bytes_ = sign_bytes_  - prefix_bytes_;
    prefix_value_bytes_ = calcPrefixValueBytes();

    // 申请空间
    prefix_buff_ = new VarBuff(prefix_size_, prefix_value_bytes_);
    suffix_buff_ = new VarBuff(items_num_, suffix_bytes_);
    value_buff_ = new VarBuff(items_num_, value_bytes_);

    return ErrSuccess;
}

ErrCode dicts::SignDict::binSearch(const uint8_t* sign_suffix, uint64_t beg_index,
        uint64_t end_index, uint64_t &target_index) {

    // 找到 beg_index == end_index 的点
    while (beg_index < end_index) {
        uint64_t mid_index = beg_index + (end_index - beg_index) / 2;
        int result = suffix_buff_->Cmp(mid_index, sign_suffix);
        if (result < 0) {
            beg_index = mid_index + 1;
        } else {
            end_index = mid_index;
        }
    }

    // beg_index == end_index
    int result = suffix_buff_->Cmp(beg_index, sign_suffix);
    if (0 == result) {
        target_index = beg_index;
        return ErrSuccess;
    } else {
        return ErrFailure;
    }
}

inline void dicts::SignDict::getPrefixAndSuffix(const uint8_t* sign,
        uint64_t &prefix, uint8_t* suffix) {

    prefix = 0;
    uint32_t i;
    for (i = 0; i < prefix_bytes_; ++i) {
        prefix = prefix << 8;
        prefix |= 0x000000FF & (uint32_t)sign[i];
    }
    for (uint32_t j = 0; i < sign_bytes_ && j < suffix_bytes_; ++i, ++j) {
        suffix[j] = sign[i];
    }
}

ErrCode dicts::SignDict::Search(const uint8_t* sign, uint8_t* value) {
    if (NULL == sign || NULL == value) {
        return ErrInvalidParams;
    }

    uint64_t prefix;
    uint8_t* suffix = new uint8_t[suffix_bytes_];
    getPrefixAndSuffix(sign, prefix, suffix);
    uint64_t beg_index, end_index;
    if (ErrSuccess != prefix_buff_->GetUint64(prefix, beg_index) ||
            ErrSuccess != prefix_buff_->GetUint64(prefix + 1, end_index)) {

        DELETE(suffix)
        return ErrFailure;
    }

    if (beg_index == end_index) {
        DELETE(suffix)
        return ErrFailure;
    }

    uint64_t target_index;
    ErrCode found = binSearch(suffix, beg_index, end_index, target_index);
    if (ErrSuccess == found) {
        value_buff_->Get(target_index, value);
    }

    DELETE(suffix)
    return found;
}

ErrCode dicts::SignDict::Insert(const uint8_t* sign, const uint8_t* value) {
    if (NULL == sign || NULL == value) {
        return ErrInvalidParams;
    }

    uint64_t prefix;
    uint8_t* suffix = new uint8_t[suffix_bytes_];
    getPrefixAndSuffix(sign, prefix, suffix);
    if (index_suffix_ >= items_num_) {
        DELETE(suffix)
        return ErrSignDictFull;
    }

    // 插入前缀数据
    if (prefix >= index_prefix_) { // 合法插入
        while (index_prefix_ <= prefix) {
            ErrCode ret = prefix_buff_->SetUint64(index_prefix_, index_suffix_);
            if (ErrSuccess != ret) {
                DELETE(suffix)
                return ret;
            }
            ++index_prefix_;
        }
    } else if (prefix == index_prefix_ - 1) { // 和上次相同的前缀
        if (0 == suffix_buff_->Cmp(index_suffix_ - 1, suffix)) { // 后缀和上一次相同
            DELETE(suffix)
            return ErrSignDictDuplicateKey;
        }
    } else { // 本次插入的位置小于上次插入的位置, 不合法
        DELETE(suffix)
        return ErrSignDictDisorderedKey;
    }

    // 插入后缀数据
    suffix_buff_->Set(index_suffix_, suffix);
    value_buff_->Set(index_suffix_, value);
    ++index_suffix_;

    DELETE(suffix)
    return ErrSuccess;
}

void dicts::SignDict::InsertFinish() {
    for (uint64_t i = index_prefix_; i < items_num_; ++i) {
        prefix_buff_->SetUint64(i, index_suffix_);
    }
    for (uint64_t i = index_suffix_; i < items_num_; ++i) {
        suffix_buff_->SetUint64(i, 0);
    }
}

void dicts::SignDict::Dumps(const std::string& filename) {
    std::ofstream outfile(filename, std::ofstream::binary);
    SignDictHeader header = {
        magic_num_, items_num_, value_bytes_,
        sign_bytes_, prefix_bytes_,
    };
    outfile.write((char*)&header, sizeof(header));

    prefix_buff_->Dumps(outfile);
    suffix_buff_->Dumps(outfile);
    value_buff_->Dumps(outfile);
    outfile.close();
}

ErrCode dicts::SignDict::Loads(const std::string& filename) {
    std::ifstream infile(filename, std::ifstream::binary);
    SignDictHeader header;
    infile.read((char*)&header, sizeof(header));
    if (header.magic_num != magic_num_) {
        return ErrSignDictInvalidFile;
    }

    items_num_ = header.items_num;
    prefix_bytes_ = header.prefix_bytes;
    sign_bytes_ = header.sign_bytes;
    value_bytes_ = header.value_bytes;

    ErrCode ret = Create();
    if (ErrSuccess != ret) {
        return ret;
    }
    prefix_buff_->Loads(infile);
    suffix_buff_->Loads(infile);
    value_buff_->Loads(infile);
    infile.close();
    return ErrSuccess;
}
