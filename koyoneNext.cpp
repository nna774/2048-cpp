#include "koyoneNext.hpp"

Koyone::GridMap KoyoneNext::iterarion(Koyone::GridMap&& map, int ITERATION){
    Koyone::GridMap npws[ITERATION];
    npws[0] = map;
    for(int i(0); i < ITERATION - 1; ++i){
        npws[i+1].reserve(npws[i].size());
        for(auto const& e: npws[i]){
            for(auto const& e2: nextPossibleWorld(e.first))
                npws[i+1].emplace_back(e2.first, e.second);
        }
        if(npws[i+1].empty()) return npws[i];
    }
    return npws[ITERATION - 1];
}

Dir KoyoneNext::decideDir() const{
    using std::make_pair;
    int const ITERATION = Koyone::nurseryTime(grid) ? 4 : 10;
    Koyone::GridMap top = iterarion(nextPossibleWorld(grid), ITERATION);
    if(top.size() < DANGER){
        Koyone::uniq(top);
        int const ITERATION = 5;
        top = iterarion(std::move(top), ITERATION);;
    }
    return (std::max_element(std::begin(top), std::end(top), Koyone::CompStatic()))->second;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld(Board::Grid grid){
    Koyone::GridMap map;
    Koyone::GridList lefts = nextPossibleWorldLeft(grid);
    for(auto e: lefts) map.push_back(std::make_pair(e, Dir::Left));
    Koyone::GridList right = nextPossibleWorldLeft(Board::gridMirror(grid));
    for(auto e: right) map.push_back(std::make_pair(Board::gridMirror(e), Dir::Right));
    Koyone::GridList down = nextPossibleWorldLeft(Board::gridMirror(Board::transpose(grid)));
    for(auto e: down) map.push_back(std::make_pair(Board::transpose(Board::gridMirror(e)), Dir::Down));
    Koyone::GridList up = nextPossibleWorldLeft(Board::transpose(grid));
    for(auto e: up) map.push_back(std::make_pair(Board::transpose(e), Dir::Up));
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

