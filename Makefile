CXX = g++
O = -O3

all: main.o board.o boardTable.o koyone.o koyoneNext.o kihime.o nona7.o
	$(CXX) main.o board.o boardTable.o koyone.o koyoneNext.o kihime.o nona7.o

.cpp.o:
	$(CXX) $(O) --std=c++11 -Wall -Wextra -c $<

main.o: board.hpp
board.o: board.cpp board.hpp koyone.hpp koyoneNext.hpp kihime.hpp nona7.hpp

boardTable.o: boardTable.cpp
koyone.o: koyone.cpp koyone.hpp
koyoneNext.o : koyoneNext.cpp koyoneNext.hpp
kihime.o: kihime.cpp kihime.hpp
nona7.o: nona7.cpp nona7.hpp

clean:
	rm -f *.o a.out

test:
	echo do nothing
