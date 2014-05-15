#pragma once

#include <functional>
#include <random>

#include "board.hpp"

class Kihime{
public:
    Kihime(Board::Grid const& grid) : grid(grid){ }
    Dir decideDir() const;
private:
    Board::Grid grid;
    static int const constexpr ITERATION = 1000;
};

Dir Kihime::decideDir() const{
    // std::random_device rnd;
    // std::vector< std::uint_least32_t> v(10);
    // std::generate(begin(v), end(v), ref(rnd));
    // std::mt19937 mt(std::seed_seq(begin(v), end(v)));

    // return allDirs[mt()%4];
}
