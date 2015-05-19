include makefile.d

all: objs $(MODULES) olime

olime: $(MODULES)
	@echo "olime"

$(MODULES): objs
	@# 显示进入哪个目录了
	@echo ">>> $@"
	@$(MAKE) -f makefile.$@ -C $@

objs:
	@if [ ! -d $(OBJECTS_DIR) ]; then mkdir $(OBJECTS_DIR); fi

test: objs
	@echo ">>> test"
	@$(MAKE) -f makefile.$@ -C $@

clean:
	@-rm -rf $(OBJECTS_DIR) $(TEST_RELEASE_DIR)

.PHONY:
	objs clean
