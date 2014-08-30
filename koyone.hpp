#pragma once

#include <random>
#include <array>

#include "board.hpp"

class Koyone{
public:
    Koyone(Board::Grid grid) : grid(grid){ }
    Dir decideDir() const;

    using Grid = Board::Grid;
    static int const constexpr MATURED = 10;
    static int const constexpr SPACE_WEIGHT = 500;
    static std::array<int, 1 << 16> makeTable();
    static double product(std::array<std::array<double, 4>, 4> const& weight, Board::Grid grid){
        double sum(0);
        for(int i(0); i < 4; ++i){
            for(int j(0); j < 4; ++j){
                sum += weight[i][j] * Board::pow2(Board::get(grid, i, j));
            }
        }
        return sum;
    }
    static double staticEval(Board::Grid grid){
        static std::array<std::array<double, 4>, 4> const weight = {{
                {{0.01, 0.01, 0.01, 0.01}},
                {{0.03, 0.01, 0.01, 0.03}},
                {{0.30, 0.10, 0.10, 0.10}},
                {{0.80, 0.40, 0.40, 0.40}}
            }};
        static std::array<std::array<double, 4>, 4> const weight2 = {{
                {{0.01, 0.01, 0.01, 0.01}},
                {{0.03, 0.01, 0.01, 0.03}},
                {{0.30, 0.20, 0.10, 0.10}},
                {{0.80, 0.30, 0.40, 0.40}}
            }};
        bool isPhase12(false);
        for(int i(0); i < 4;++i)
            for(int j(0); j < 4;++j)
                if(Board::get(grid, i, j) >= 12) isPhase12 = true;
        
        double val = 0.0;
            for(auto dir: allDirs){
                double est = product(isPhase12 ? weight2 : weight, Board::rotate(grid, dir));
                val = std::max(est, val);
            }
            return std::min(val/(8192 * 1.0), 1.0);
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

