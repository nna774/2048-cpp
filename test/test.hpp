#include <random>
#include <vector>

#include "../board.hpp"

namespace test{
    std::mt19937 mtInit(){
        static std::random_device rnd;
        std::vector<std::uint_least32_t> v(10);
        for(auto& e: v) e = rnd();
        std::seed_seq seq(begin(v), end(v));
        std::mt19937 mt(seq);
        return mt;
    }
    Board::Grid make0to15(){
	Board::Grid grid = 0;
	for(int i(0); i < 16; ++i){
	    grid = Board::set(grid, i / 4, i % 4, i);
	}
	return grid;
    }
    Board::Grid toGrid(std::array<int, 16> xs){
	Board::Grid grid = 0;
	for(int i(0); i < 16; ++i){
	    grid = Board::set(grid, i / 4, i % 4, xs[i]);
	}
	return grid;
    }
};

