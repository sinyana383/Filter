BUILD_DIR = build/project
CLANG_TIDY_CMD = clang-format -style=google -n
TMP = Testing/ html/ latex/

.PHONY: all install uninstall tests lint dist dvi clean

all: install
	./$(BUILD_DIR)/photolab

install: $(BUILD_DIR)
	cmake -S . -B build && cmake --build build

$(BUILD_DIR):
	mkdir -p $@

tests:
	./build/test/tests

uninstall:
	rm -rf build

lint:
	$(CLANG_TIDY_CMD) ./project/view/*.cpp ./project/view/*.h
	$(CLANG_TIDY_CMD) ./project/controller/*.cpp ./project/controller/*.hpp
	$(CLANG_TIDY_CMD) ./project/model/*.cpp ./project/model/*.hpp

dist:
	zip -r $(BUILD_DIR)/photolab.zip $(BUILD_DIR)/photolab

dvi:
	doxygen Doxyfile && open ./html/index.html

clean: uninstall
	rm -rf $(TMP)
