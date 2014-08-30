#pragma once

#include "board.hpp"

class UCT{
public:
    UCT(Board::Grid grid) : grid(grid) { }
    Dir decideDir();
    static std::mt19937 mtInit();
private:
    Board::Grid grid;
    static std::random_device rnd;
    static std::mt19937 mt;
    double staticEval(Board::Grid);
    Board::Grid playout(Board::Grid, int);
    static int const constexpr ITERATION = 5000;
    static int const constexpr ITERATION_BOOT = 100;
    static int const constexpr PLAYOUT_DEPTH = 500;
};
