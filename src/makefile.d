CC := g++
#MODULES := tools dicts
MODULES := tools

TOP_DIR := $(PWD)
INCLUDES_DIR := $(TOP_DIR)/includes
TEST_DIR := $(TOP_DIR)/test
TEST_RELEASE_DIR := $(TEST_DIR)/release
OBJECTS_DIR := $(TOP_DIR)/objs
LIBS_DIR := $(TOP_DIR)/../libs
OUTPUT_DIR := $(TOP_DIR)/output

# 请务必使用 C++ 11
CPPFLAGS := -std=c++11
INCLUDES := -I$(INCLUDES_DIR)
OBJECTS := -L$(OBJECTS_DIR)
LIBS := -l glog -l pthread


# 定义一个规则用于所有 .o 文件的生成
%.o: %.cc $(wildcard $(INCLUDES_DIR)/*.h)
	@$(CC) -c $(CPPFLAGS) $(INCLUDES) $< -o $(OBJECTS_DIR)/$@
