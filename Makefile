CXX = g++

all: main.o board.o
	$(CXX) main.o board.o

.cpp.o:
	$(CXX) --std=c++11 -Wall -Wextra -O3 -c $<

main.o: board.hpp
board.cpp: board.hpp koyone.hpp kihime.hpp

clean:
	rm -f *.o

test:
	echo do nothing
