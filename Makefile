all: test
clean: testclean

CXX = clang++

# Tests

TEST_CXXFLAGS = -I $(GTEST_DIR)/include
TESTSOURCES = $(wildcard tests/testsrc/*.cpp)
TESTOBJECTS = $(TESTSOURCES:tests/testsrc/%.cpp=tests/%.o)

test: tests/test.out
	tests/test.out

tests/%.o: tests/testsrc/%.cpp
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

tests/test.out: $(TESTOBJECTS) tests/gtest_main.a
	$(CXX) $(TEST_CXXFLAGS) -lpthread $^ -o $@

testclean:
	rm -f tests/*.a tests/*.o tests/*.out

# Google Test Lib

GTEST_DIR = tests/googletest/googletest
GTEST_CPPFLAGS += -isystem $(GTEST_DIR)/include
GTEST_CXXFLAGS += -g -Wall -Wextra -pthread
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

tests/gtest_main.a: tests/gtest-all.o tests/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

tests/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_DIR) $(GTEST_CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

tests/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_DIR) $(GTEST_CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

.PHONY: all clean test testclean
