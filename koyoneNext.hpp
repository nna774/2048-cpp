#pragma once

#include "koyone.hpp"

class KoyoneNext {
public:
    KoyoneNext(Board::Grid grid) : grid(grid) { }
    Dir decideDir() const;
private:
    Board::Grid grid;
    static int const constexpr DANGER = 30000;
    static Koyone::GridMap nextPossibleWorld(Board::Grid);
    static Koyone::GridList nextPossibleWorldLeft(Board::Grid);
};

