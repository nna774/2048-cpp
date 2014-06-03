#pragma once

#include <functional>
#include <random>
#include <vector>

#include "board.hpp"

class Kihime{
public:
    Kihime(Board::Grid grid) : grid(grid) { }
    Dir decideDir();
    static std::mt19937 mtInit();
private:
    Board::Grid grid;
    static std::random_device rnd;
    static std::mt19937 mt;
    static int const constexpr ITERATION = 5000;
    static int const constexpr DANGER = 10;
    static int toDead(Board::Grid, int);
};

