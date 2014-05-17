CXX = g++

all: main.o board.o boardTable.o
	$(CXX) main.o board.o boardTable.o

.cpp.o:
	$(CXX) --std=c++11 -Wall -Wextra -O3 -c $<

main.o: board.hpp
board.cpp: board.hpp koyone.hpp kihime.hpp
boardTable.o: boardTable.cpp

clean:
	rm -f *.o

test:
	echo do nothing
