#include <iostream>
#include "signdict.h"

using namespace std;

int main() {
    const uint64_t items_num = 5; // less than 64
    const uint32_t value_bytes = 5;
    const uint32_t sign_bytes = 4;

    dicts::SignDict sd1 = dicts::SignDict(items_num, value_bytes, sign_bytes);
    sd1.Create();

    for (size_t i = 0; i < items_num; ++i) {
        uint8_t sign[sign_bytes];
        uint8_t value[value_bytes];

        for (uint32_t j = 0; j < sign_bytes; ++j) {
            sign[j] = i + 5;
        }

        for (uint32_t j = 0; j < value_bytes; ++j) {
            value[j] = i + j + 1;
        }
        value[value_bytes - 1] = i + 5;
        ErrCode ret = sd1.Insert(sign, value);
        cout << "Ret: " << ret << endl;

    }
    sd1.InsertFinish();

    uint8_t sign[sign_bytes];
    // i = 1
    uint8_t i = 2;
    for (uint32_t j = 0; j < sign_bytes; ++j) {
        sign[j] = i + 5;
    }
    uint8_t value[value_bytes];
    ErrCode result = sd1.Search(sign, value);
    cout << result << "(needs 3 4 5 6 7):" << endl;
    for (uint32_t i = 0; i < value_bytes; ++i) {
        cout << hex << (uint32_t)value[i] << " ";
    }
    cout << endl;

    sd1.Dumps("sd.dat");

    dicts::SignDict sd2 = dicts::SignDict();
    sd2.Loads("sd.dat");
    memset(value, 0, value_bytes);
    result = sd2.Search(sign, value);
    cout << result << "(needs 3 4 5 6 7):" << endl;
    for (uint32_t i = 0; i < value_bytes; ++i) {
        cout << hex << (uint32_t)value[i] << " ";
    }
    cout << endl;
    return 0;

}
