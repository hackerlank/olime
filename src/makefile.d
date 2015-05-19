CC := g++
MODULES := tools

TOP_DIR := $(PWD)
INCLUDES_DIR := $(TOP_DIR)/includes
TEST_DIR := $(TOP_DIR)/test
TEST_RELEASE_DIR := $(TEST_DIR)/release
OBJECTS_DIR := $(TOP_DIR)/objs

CPPFLAGS := 
INCLUDES := -I$(INCLUDES_DIR)
OBJECTS := -L$(OBJECTS_DIR)


# 定义一个规则用于所有 .o 文件的生成
%.o: %.cc $(wildcard $(INCLUDES_DIR)/*.h)
	@$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $(OBJECTS_DIR)/$@
