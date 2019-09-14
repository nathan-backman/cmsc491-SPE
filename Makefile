CXX=g++
CFLAGS=-std=c++17 -Isrc -g -fmax-errors=1

SRC_FILES = $(shell find src/ -name '*.cpp')
OBJ_FILES = $(SRC_FILES:.cpp=.o)
DEP_FILES = $(OBJ_FILES:.o=.d)
TEST_DIRS = $(sort $(dir $(wildcard tests/*/.)))
TEST_APPS = $(patsubst tests/%/, tests/%/driver.app, $(TEST_DIRS))
PROFILER_DIRS = $(sort $(dir $(wildcard profiler/*/.)))
PROFILER_APPS = $(patsubst profiler/%/, profiler/%/driver.app, $(PROFILER_DIRS))
SAMPLE_CPPS = $(shell ls apps/*.cpp)
SAMPLE_APPS = $(SAMPLE_CPPS:.cpp=.app)

all: $(SAMPLE_APPS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

%.d: %.cpp
	$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

-include $(DEP_FILES)

apps/%.app: apps/%.cpp $(OBJ_FILES)
	$(CXX) $(CFLAGS) -o $@ $^

tests/%/driver.app: tests/%/driver.cpp $(OBJ_FILES)
	$(CXX) $(CFLAGS) -o $@ $^

profiler/%/driver.app: profiler/%/driver.cpp $(OBJ_FILES)
	$(CXX) $(CFLAGS) -o $@ $^

run: apps/sample_driver.app
	$<


.PHONY: clean dbg lint format docs tests profiler
clean:
	@echo "Cleaning ..."
	rm -f $(OBJ_FILES)
	rm -f $(SAMPLE_APPS)
	rm -f $(DEP_FILES)
	rm -f $(TEST_APPS)
	rm -f $(PROFILER_APPS)

lint:
	find . -name "*.cpp" -or -name "*.h" | xargs python cpplint.py --root=src

format:
	find . -name "*.cpp" -or -name "*.h" | xargs clang-format -i

docs:
	doxygen docs/Doxyfile

tests: $(TEST_APPS)
	tests/run_tests.sh

profiler: $(PROFILER_APPS)
	profiler/run_tests.sh
