#include "koyoneNext.hpp"

Dir KoyoneNext::decideDir() const{
    using std::make_pair;
    int const ITERATION = Koyone::nurseryTime(grid) ? 4 : 10;
    Koyone::GridMap top = iterarion(nextPossibleWorld(grid), ITERATION);
    if(top.size() < DANGER){
        Koyone::uniq(top);
        int const ITERATION = 5;
        top = iterarion(std::move(top), ITERATION);
    }
    return (std::max_element(std::begin(top), std::end(top), Koyone::CompStatic()))->second;
}

inline Koyone::GridMap KoyoneNext::nextPossibleWorld(Board::Grid grid){
    Koyone::GridMap map;
    Koyone::GridList lefts = nextPossibleWorldLeft(grid);
    for(auto e: lefts) map.emplace_back(e, Dir::Left);
    Koyone::GridList right = nextPossibleWorldLeft(Board::gridMirror(grid));
    for(auto e: right) map.emplace_back(Board::gridMirror(e), Dir::Right);
    Koyone::GridList down = nextPossibleWorldLeft(Board::gridMirror(Board::transpose(grid)));
    for(auto e: down) map.emplace_back(Board::transpose(Board::gridMirror(e)), Dir::Down);
    Koyone::GridList up = nextPossibleWorldLeft(Board::transpose(grid));
    for(auto e: up) map.emplace_back(Board::transpose(e), Dir::Up);
    return map;
}

inline Koyone::GridList KoyoneNext::nextPossibleWorldLeft(Board::Grid grid){
    auto left = Board::moveLeft(grid);
    if(left == grid) return {};
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0)
                left = Board::set(left, i, j, 1);
    return { left };
}

