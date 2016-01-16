# OLIME (Online Input Method) 文档

本文档旨在帮助接手该代码的同学, 同时也可以让你写出能让自己满意也让后续接你代码的人满意的代码.


## 写在最前面

写在最前面表示该部分 **十分十分重要**, 请严格遵守如下几条, 你可以慢一点写代码 (也可以选择加班加点, 只是我不推荐), 但是请不要在如下几条上有 **任何妥协**.
**如果你对于其中任何一点有疑问, 请不要动手! 谢谢配合.**

1. 请先看一看[Google C++ 代码规范](http://zh-google-styleguide.readthedocs.org/en/latest/contents/), 这大概需要花费你不少的时间, 相信我这绝对值得!
2. 所有的文档请**不要**使用`Tab`制表符, 请使用**4个空格代替**.
3. 所有文档请都使用`UTF-8`编码..
4. 所有的类型请都写在`include/define.h`中, 这样看代码的人来找**自定义类型**和**数据结构定义**的时候不会找遍整个文件目录.
5. 因为输入法中存在 CJK 字符, 所以请搞清楚 Unicode, UTF-32, UTF-16, UTF-8 的关系以后再开始动手.
6. 对于UTF-32, UTF-16, UTF-8 这三种编码方式的字符串, 请使用对应的`stringXX_t`.
7. 从 golang 上学习到一个函数命名方法, 可外部调用的函数首字母大写, 不可外部调用的函数首字母小写.


## 引用的第三方库

本代码引用了部分第三方库, 其中包括:

### 1. [glog](https://github.com/google/glog)

Google 出品, 我是脑残粉. 直接用吧, 请记得安装.

### 2. [dawgdic](https://code.google.com/p/dawgdic/)

dawgdic 也包含一个 Python 的[封装](https://github.com/kmike/DAWG), 代码中对
dawgdic 进行了封装, 将我们会用的功能提取出来. 有兴趣可以仔细看看 dawgdic 的代码,
封装的代码叫做 trie.cc / trie.h

### 3. [msgpack](http://msgpack.org/)

msgpack 是一个序列化工具, 可以序列话各种类型的对象, 包括你自己设计的.


## 字符编码规范

由于我们之前使用的字符集采用了 GB18030 这种国标, 而不是使用 Unicode
这种国际化的规范标准, 在使用的时候出现了很多不好处理的情况, 例如: debug
的时候需要将 terminal 改成 GB18030 编码; 和 URL 一同处理的时候存在 UTF-8
转换 GB18030 的需求.

由于我们采用 C++ 11, 所以直接使用新支持的 char16_t, char32_t, u16string,
u32string. 所以请使用新的类型和 char, string 来分别处理16bit, 32bit, 8bit
的字符串.


## 文档细节

### 1. 数据结构

1. [signdict](/docs/signdict.md)一种快速简单的 key-value 存储/查询方式.
2. [trie](/docs/trie.md)字典树, 对 dawgdic 的封装.
