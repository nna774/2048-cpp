#pragma once

#include <random>

#include "board.hpp"

class Koyone{
public:
    Koyone(Board::Grid grid) : grid(grid){ }
    Dir decideDir() const;

    using Grid = Board::Grid;
    static int staticEval(Grid);
    static bool nurseryTime(Grid);
    static int const constexpr MATURED = 1024;
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
    Grid grid;
};

void Koyone::uniq(Koyone::GridMap& list) {
    auto newEnd = std::unique(std::begin(list), std::end(list), CompEqual());
    list.erase(newEnd, std::end(list));
}

Dir Koyone::decideDir() const{
    using std::make_pair;
    // std::mt19937 mt;
    // std::uniform_int_distribution<int> rand4(0,3);
    // std::uniform_int_distribution<int> rand10(0,9);
    // static int cnt(0);
    // return allDirs[cnt++%4];
    auto npw = nextPossibleWorld(grid);
    auto top = npw;
    decltype(npw) npw2, npw3, npw4, npw5, npw6;
    // if(npw.empty()) std::cout << "!!!!!!!!!!!!!!!!" << std::endl;
    // for(auto e: npw) {
    //     std::cout << "!!----------------!!" << std::endl;
    //     Board::show(e.first);
    //     std::cout << "!!----------------!!" << std::endl;
    // }
    std::cout << "size of 1: " << npw.size() << std::endl;
    uniq(npw);
    npw2.reserve(1024 * 12);
    for(auto e: npw){
        for(auto e2: nextPossibleWorld(e.first))
            npw2.push_back(make_pair(e2.first, e.second));
    }
    if(npw2.empty()) {
        top = std::move(npw);
        goto empty;
    }
    std::cout << "size of 2: " << npw2.size() << std::endl;
    uniq(npw2);
    std::cout << "size of 2: " << npw2.size() << std::endl;
    // npw3.reserve(1024 * 20);
    for(auto e: npw2){
        for(auto e2: nextPossibleWorld(e.first))
            npw3.push_back(make_pair(e2.first, e.second));
    }
    if(npw3.empty()) {
        top = std::move(npw);
        goto empty;
    }
    top = npw3;
    std::cout << "size of 3: " << npw3.size() << std::endl;
    uniq(npw3);
    std::cout << "size of 3: " << npw3.size() << std::endl;
    if(! nurseryTime(grid)) {
        npw4.reserve(1024 * 512);
        for(auto const& e: npw3){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw4.push_back(make_pair(e2.first, e.second));
        }
        if(npw4.empty()) {
            top = std::move(npw);
            goto empty;
        }
        top = npw4;
        uniq(npw4);
        for(auto const& e: npw4){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw5.push_back(make_pair(e2.first, e.second));
        }
        if(npw5.empty()) goto empty;
        top = npw5;
        for(auto const& e: npw5){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw6.push_back(make_pair(e2.first, e.second));
        }
        if(npw6.empty()) goto empty;
        top = npw6;

    }
        // std::cout << "before uniq of 4: " << npw4.size() << std::endl;
        // std::sort(std::begin(npw4), std::end(npw4));
        // auto newEnd = std::unique(std::begin(npw4), std::end(npw4), [](std::pair<Board::Grid, Dir> const& a, std::pair<Board::Grid, Dir> const& b) -> bool { return a.first == b.first; });
        // npw4.erase(newEnd, std::end(npw4));
        // std::cout << "after uniq of 4: " << npw4.size() << std::endl;
        // npw5.reserve(1024 * 12);
    //     // もう一回
    //     // std::cout << "before uniq of 5: " << npw5.size() << std::endl;
    //     // std::sort(std::begin(npw5), std::end(npw5));
    //     // newEnd = std::unique(std::begin(npw5), std::end(npw5), [](std::pair<Board::Grid, Dir> const& a, std::pair<Board::Grid, Dir> const& b) -> bool { return a.first == b.first; });
    //     // npw5.erase(newEnd, std::end(npw5));
    //     // std::cout << "after uniq of 5: " << npw5.size() << std::endl;
    //     // npw6.reserve(1024 * 120);
    //     for(auto const& e: npw5){
    //         for(auto const& e2: nextPossibleWorld(e.first))
    //             npw6.insert(make_pair(e2.first, e.second));
    //     }
    //     if(npw6.empty()) goto empty;
    //     top = npw6;
    // }
    empty: ;
    return (std::max_element(std::begin(top), std::end(top), CompStatic()))->second;
}

int Koyone::staticEval(Board::Grid grid){
    int sum(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            sum += Board::get(grid, i, j)
                ? Board::get(grid, i, j) * log2(Board::get(grid, i, j))
                : SPACE_WEIGHT;
    return sum;
}

bool Koyone::nurseryTime(Board::Grid grid){
    for(int i(0); i < 4;++i)
        for(int j(0); j < 4;++j)
            if(Board::get(grid, i, j) >= MATURED) return false;
    return true;
}

Koyone::GridMap Koyone::nextPossibleWorld(Board::Grid grid){
    GridMap map;
    GridList lefts = nextPossibleWorldLeft(grid);
    for(auto e: lefts) map.push_back(std::make_pair(e, Dir::Left));
    GridList right = nextPossibleWorldLeft(Board::gridMirror(grid));
    for(auto e: right) map.push_back(std::make_pair(Board::gridMirror(e), Dir::Right));
    GridList down = nextPossibleWorldLeft(Board::gridMirror(Board::transpose(grid)));
    for(auto e: down) map.push_back(std::make_pair(Board::transpose(Board::gridMirror(e)), Dir::Down));
    GridList up = nextPossibleWorldLeft(Board::transpose(grid));
    for(auto e: up) map.push_back(std::make_pair(Board::transpose(e), Dir::Up));
//    vec.reserve(128);
    // for(auto dir: allDirs){
    //     GridList tmp = nextPossibleWorldLeft(Board::rotate(Board::rotate(grid, dir), Dir::Right));
    //     for(auto e: tmp) map.insert(std::make_pair(Board::rotate(Board::rotate(e, Dir::Left), mirror(dir)), dir));
    // }
//    std::cout << "size: " << vec.size() << std::endl;
    return map;
}

Koyone::GridList Koyone::nextPossibleWorldLeft(Board::Grid grid){
    auto left = Board::moveLeft(grid);
    if(left == grid) return {};
    int zeros(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0) ++zeros;
    GridList tmps(zeros * 2, left);
    int cnt(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0){
                tmps[cnt]   = Board::set(left, i, j, 1);
                tmps[cnt+1] = Board::set(left, i, j, 2);
                cnt +=2;
            }
    return tmps;
}
