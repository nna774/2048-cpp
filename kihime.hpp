#pragma once

#include <functional>
#include <random>
#include <vector>

#include "board.hpp"

class Kihime{
public:
    Kihime(Board::Grid grid) : grid(grid){ }
    Dir decideDir();
private:
    Board::Grid grid;
    static int const constexpr ITERATION = 1000;
};

Dir Kihime::decideDir(){
    std::random_device rnd;
    std::vector<std::uint_least32_t> v(10);
    for(auto& e:v) e = rnd();
    std::seed_seq seq(begin(v), end(v));
    std::mt19937 mt(seq);

    return allDirs[mt()%4];
}
