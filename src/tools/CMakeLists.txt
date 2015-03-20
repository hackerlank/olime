# 添加 include 目录
include_directories(../include)

# 将当前目录下所有源文件保存到变量中
aux_source_directory(. DIR_SRCS)

# 生成链接库
add_library(tools STATIC ${DIR_SRCS})
