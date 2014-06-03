#pragma once

#include <random>

#include "board.hpp"

class Koyone{
public:
    Koyone(Board::Grid grid) : grid(grid){ }
    Dir decideDir() const;

    using Grid = Board::Grid;
    static int const constexpr MATURED = 10;
    static int const constexpr SPACE_WEIGHT = 500;
    static int staticEval(Board::Grid grid){
        int sum(0);
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j){
                auto gotten = Board::get(grid, i, j);
                sum += gotten
                    ? Board::pow2(gotten) * gotten
                    : SPACE_WEIGHT;
            }
        return sum;
    }
    static bool nurseryTime(Grid);
    struct CompStatic{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return staticEval(a.first) < staticEval(b.first);
        }
        // bool operator()(Grid a, Grid b){
        //     return staticEval(a) < staticEval(b);
        // }
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
    Grid grid;
};

