GTEST_DIR = gtest-1.7.0
USER_DIR = ..
TESTSRC_DIR = .
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

all: test

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

TESTS = board_unittest

MINE = $(USER_DIR)/board.cpp $(USER_DIR)/board.hpp $(USER_DIR)/kihime.cpp $(USER_DIR)/kihime.hpp $(USER_DIR)/koyone.cpp $(USER_DIR)/koyone.hpp $(USER_DIR)/koyoneNext.cpp $(USER_DIR)/koyoneNext.hpp $(USER_DIR)/nona7.cpp $(USER_DIR)/nona7.hpp
MINE_O = board.o koyone.o koyoneNext.o kihime.o nona7.o

board.o : $(MINE) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/board.cpp

koyone.o : $(MINE) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/koyone.cpp

koyoneNext.o : $(MINE) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/koyoneNext.cpp

kihime.o : $(MINE) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/kihime.cpp

nona7.o : $(MINE) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/nona7.cpp

board_unittest.o : $(TESTSRC_DIR)/board_unittest.cpp $(USER_DIR)/board.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TESTSRC_DIR)/board_unittest.cpp

board_unittest : $(MINE_O) board_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

test: $(TESTS)
	for p in "$(TESTS)"; do ./$$p ; done

clean:
	rm -f *.o *.a $(TESTS)
