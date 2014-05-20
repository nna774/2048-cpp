#pragma once

#include "koyone.hpp"

class KoyoneNext {
public:
    KoyoneNext(Board::Grid grid) : grid(grid) { }
    Dir decideDir() const;
private:
    Board::Grid grid;
    static Koyone::GridMap nextPossibleWorld(Board::Grid);
    static Koyone::GridList nextPossibleWorldLeft(Board::Grid);
};

