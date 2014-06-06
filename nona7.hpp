#pragma once

#include <random>

#include "board.hpp"

class Nona7{
public:
    Nona7(Board::Grid grid) : grid(grid){ }
    Dir decideDir() const;

    using Grid = Board::Grid;
    static int staticEval(Grid);
    static bool nurseryTime(Grid);
    static int const constexpr MATURED = 10;
    static int const constexpr SPACE_WEIGHT = 500;
    struct CompStatic{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return staticEval(a.first) < staticEval(b.first);
        }
        bool operator()(Grid a, Grid b){
            return staticEval(a) < staticEval(b);
        }
    };
    struct CompGrid{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return a.first < b.first;
        }
    };
    struct CompEqual{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return a.first == b.first;
        }
    };
    using GridList = std::vector<Grid>;
    using GridMap = std::vector<std::pair<Grid, Dir>>;
    static GridMap nextPossibleWorld(Grid);
    static GridList nextPossibleWorldLeft(Grid);
    static void uniq(GridMap&);
private:
    static int sqrt(int); 
    Grid grid;
};

