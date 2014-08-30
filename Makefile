CXX = g++
O = -O3
OPT = -O3 -march=native -pipe
PROFILE = -fprofile-generate
RELEASE = -fprofile-use
SRC = main.o board.o koyone.o koyoneNext.o kihime.o kihimeNext.o nona7.o uct.o
OUTNAME = 2048-ai
ITERATION = 3

.PHONY: test all clean

all: $(SRC)
	$(CXX) $(OPT) $(SRC) -o $(OUTNAME)

.cpp.o:
	$(CXX) $(O) $(OPT) --std=c++11 -Wall -Wextra -c $<

main.o: board.hpp
board.o: board.cpp board.hpp koyone.hpp koyoneNext.hpp kihime.hpp kihimeNext.hpp nona7.hpp uct.hpp

koyone.o: koyone.cpp koyone.hpp
koyoneNext.o : koyoneNext.cpp koyoneNext.hpp
kihime.o: kihime.cpp kihime.hpp
kihimeNext.o: kihimeNext.cpp kihimeNext.hpp
nona7.o: nona7.cpp nona7.hpp
uct.o: uct.cpp uct.hpp

clean:
	rm -f *.o $(OUTNAME) *.gcda gmon.out
	cd test; make clean

release:
	rm -f *.gcda
	$(CXX) $(OPT) $(PROFILE) --std=c++11 -Wall -Wextra main.cpp board.cpp koyone.cpp koyoneNext.cpp kihime.cpp kihimeNext.cpp nona7.cpp -o $(OUTNAME)
	for i in `seq 1 $(ITERATION)`; do echo Stage $i; ./$(OUTNAME); done
	$(CXX) $(OPT) $(RELEASE) --std=c++11 -Wall -Wextra main.cpp board.cpp koyone.cpp koyoneNext.cpp kihime.cpp kihimeNext.cpp nona7.cpp -o $(OUTNAME)

test:
	cd test; make test
