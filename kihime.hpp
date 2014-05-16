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
};
