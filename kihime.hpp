#pragma once

#include <functional>
#include <random>
#include <vector>

#include "board.hpp"

class Kihime{
public:
    Kihime(Board::Grid grid) : grid(grid) { }
    Dir decideDir();
private:
    Board::Grid grid;
    static std::random_device rnd;
    static std::mt19937 mt;
    static std::mt19937 mtInit();
    static int const constexpr ITERATION = 1000;
    static int toDead(Board::Grid, int);
    static Board::Grid moveAndBirth(Board::Grid, Dir);
};

