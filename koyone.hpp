#pragma once

#include <random>
#include <map>

#include "board.hpp"

class Koyone{
public:
    Koyone(Board::Grid grid) : grid(grid){ }
    Dir decideDir() const;
private:
    using Grid = Board::Grid;
    Grid grid;
    static int staticEval(Grid);
    static bool nurseryTime(Grid);
    static int const constexpr MATURED = 1024;
    struct Comp{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return staticEval(a.first) < staticEval(b.first);
        }
        bool operator()(Grid a, Grid b){
            return staticEval(a) < staticEval(b);
        }
    };
    using GridList = std::vector<Grid>;
    using GridMap = std::map<Grid, Dir>;
    static GridMap nextPossibleWorld(Grid);
    static GridList nextPossibleWorldUp(Grid);
};

Dir Koyone::decideDir() const{
    using std::make_pair;
    std::mt19937 mt;
    std::uniform_int_distribution<int> rand4(0,3);
    std::uniform_int_distribution<int> rand10(0,9);

    // int const constexpr MIN_LENGTH = 100;
    auto npw = nextPossibleWorld(grid);
    auto top = npw;
    decltype(npw) npw2, npw3, npw4, npw5, npw6;
    // std::cout << "size of 1: " << npw.size() << std::endl;
    // npw2.reserve(1024 * 8);
    for(auto const& e: npw){
        for(auto const& e2: nextPossibleWorld(e.first))
            npw2.insert(make_pair(e2.first, e.second));
    }
    if(npw2.empty()) {
        top = std::move(npw);
        goto empty;
    }
    // std::cout << "size of 2: " << npw2.size() << std::endl;
    // npw3.reserve(1024 * 20);
    for(auto const& e: npw2){
        for(auto const& e2: nextPossibleWorld(e.first))
            npw3.insert(make_pair(e2.first, e.second));
    }
    if(npw3.empty()) {
        top = std::move(npw);
        goto empty;
    }
    // std::cout << "before uniq of 3: " << npw3.size() << std::endl;
    // std::sort(std::begin(npw3), std::end(npw3));
    
    // npw3.erase(std::unique(std::begin(npw3), std::end(npw3), [](std::pair<Board::Grid, Dir> const& a, std::pair<Board::Grid, Dir> const& b) -> bool { return a.first == b.first; })
    //            , std::end(npw3));
    // std::cout << "after uniq of 3: " << npw3.size() << std::endl;

    // npw4.reserve(1024 * 512);
    for(auto const& e: npw3){
        for(auto const& e2: nextPossibleWorld(e.first))
            npw4.insert(make_pair(e2.first, e.second));
    }
    if(npw4.empty()) {
        top = std::move(npw);
        goto empty;
    }
    top = npw4;
    if(! nurseryTime(grid)) {
        // std::cout << "before uniq of 4: " << npw4.size() << std::endl;
        // std::sort(std::begin(npw4), std::end(npw4));
        // auto newEnd = std::unique(std::begin(npw4), std::end(npw4), [](std::pair<Board::Grid, Dir> const& a, std::pair<Board::Grid, Dir> const& b) -> bool { return a.first == b.first; });
        // npw4.erase(newEnd, std::end(npw4));
        // std::cout << "after uniq of 4: " << npw4.size() << std::endl;
        // npw5.reserve(1024 * 12);
        for(auto const& e: npw4){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw5.insert(make_pair(e2.first, e.second));
        }
        if(npw5.empty()) goto empty;
        top = npw5;
        // もう一回
        // std::cout << "before uniq of 5: " << npw5.size() << std::endl;
        // std::sort(std::begin(npw5), std::end(npw5));
        // newEnd = std::unique(std::begin(npw5), std::end(npw5), [](std::pair<Board::Grid, Dir> const& a, std::pair<Board::Grid, Dir> const& b) -> bool { return a.first == b.first; });
        // npw5.erase(newEnd, std::end(npw5));
        // std::cout << "after uniq of 5: " << npw5.size() << std::endl;
        // npw6.reserve(1024 * 120);
        for(auto const& e: npw5){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw6.insert(make_pair(e2.first, e.second));
        }
        if(npw6.empty()) goto empty;
        top = npw6;
    }
    empty: ;
    // for(auto const& e:npw3) std::cout << staticEval(e.first) << ", " << dirToInt(e.second) << std::endl;
    auto max = *std::max_element(std::begin(top), std::end(top), Comp());
    // std::cout << "Max: " << staticEval(max.first) << ", " << dirToInt(max.second) << std::endl;
    return max.second;
}

int Koyone::staticEval(Board::Grid grid){
    int const constexpr SPACE_WEIGHT = 500;
    int sum(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            sum += Board::get(grid, i, j)
                ? Board::get(grid, i, j) * log2(Board::get(grid, i, j))
                : SPACE_WEIGHT;
    return sum;
}

bool Koyone::nurseryTime(Board::Grid grid){
    int const constexpr MATURED = 1024;
    for(int i(0); i < 4;++i)
        for(int j(0); j < 4;++j)
            if(Board::get(grid, i, j) >= MATURED) return false;
    return true;
}

Koyone::GridMap Koyone::nextPossibleWorld(Board::Grid grid){
    GridMap map;
//    vec.reserve(128);
    for(auto dir: allDirs){
        GridList tmp = nextPossibleWorldUp(Board::rotate(grid, dir));
        for(auto const& e: tmp) map.insert(std::make_pair(Board::rotate(e, mirror(dir)), dir));
    }
//    std::cout << "size: " << vec.size() << std::endl;
    return map;
}

Koyone::GridList Koyone::nextPossibleWorldUp(Board::Grid grid){
    auto left = Board::moveLeft(grid);
    if(left == grid) return {};
    int zeros(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j)) ++zeros;
    GridList tmps(zeros * 2, left);
    int cnt(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0){
                // tmps[cnt][j][i] = 2;
                // tmps[cnt+1][j][i] = 4;
                tmps[cnt]   = Board::set(tmps[cnt], i, j, 1);
                tmps[cnt+1] = Board::set(tmps[cnt+1], i, j, 2);
            }
    return tmps;
}
