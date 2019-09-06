CXX=g++
CFLAGS=-std=c++14 -Isrc -g -fmax-errors=1

SRC_FILES = $(shell find src/ -name '*.cpp')
OBJ_FILES = $(SRC_FILES:.cpp=.o)
DEP_FILES = $(OBJ_FILES:.o=.d)


apps/sample_driver.app: apps/sample_driver.cpp $(OBJ_FILES)
	$(CXX) $(CFLAGS) -o $@ $^


%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

%.d: %.cpp
	$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@


-include $(DEP_FILES)

run: apps/sample_driver.app
	$<


.PHONY: clean dbg lint format docs
clean:
	@echo "Cleaning ..."
	rm -f apps/sample_driver.app $(OBJ_FILES)
	rm -f $(DEP_FILES)

lint:
	find . -name "*.cpp" -or -name "*.h" | xargs python cpplint.py --root=src

format:
	find . -name "*.cpp" -or -name "*.h" | xargs clang-format -i

docs:
	doxygen docs/Doxyfile
