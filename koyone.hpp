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
    static int staticEval(Board::Grid grid){
        static std::array<int, 1 << 16> const table = makeTable();
        return
            table[(grid & UINT64_C(0xFFFF000000000000)) >> 48] +
            table[(grid & UINT64_C(0x0000FFFF00000000)) >> 32] +
            table[(grid & UINT64_C(0x00000000FFFF0000)) >> 16] +
            table[(grid & UINT64_C(0x000000000000FFFF)) >> 00] ;
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

