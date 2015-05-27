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
7. 从 golang 上学习到一个函数命名方法, 可外部调用的函数首字母大写, 不可外部调用的函数首字母小写, 均采用驼峰命名法.
8. 各种返回类型请标注在`include/define.h`中的`ErrCode`枚举类型中.


## 引用的第三方库

本代码引用了部分第三方库, 其中包括:

### 1. [easylogging++](https://github.com/easylogging/easyloggingpp)

本来是使用[glog](https://code.google.com/p/google-glog/)的, 但是由于glog过于庞大, 而我们对于日志的需求并没有想象中那么强烈,
所以改用 easylogging++. easylogging++ 的名字真是拗口, 但是人家就叫这个也只能这样了. easylogging++需要使用 C++ 11.
