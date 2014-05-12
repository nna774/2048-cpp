#CXX = clang++
CXX = g++
#CXX = /opt/clang-head/bin/clang++

all: main.o board.o
	$(CXX) main.o board.o

.cpp.o:
	$(CXX) --std=c++11 -Wall -Wextra -O3 -c $<

main.o: board.hpp
board.cpp: board.hpp

clean:
	rm -f *.o
