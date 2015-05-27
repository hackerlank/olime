include makefile.d

all: mkdir $(MODULES)


mkdir:
	@if [ ! -d $(OBJECTS_DIR) ]; then mkdir $(OBJECTS_DIR); fi
	@if [ ! -d $(OUTPUT_DIR) ]; then mkdir $(OUTPUT_DIR); fi

$(MODULES): objs
	@# 显示进入哪个目录了
	@echo ">>> $@"
	@$(MAKE) -f makefile.$@ -C $@

test: objs
	@echo ">>> test"
	@$(MAKE) -f makefile.$@ -C $@

clean:
	@-rm -rf $(OBJECTS_DIR) $(TEST_RELEASE_DIR) $(OUTPUT_DIR)

.PHONY:
	objs clean all
