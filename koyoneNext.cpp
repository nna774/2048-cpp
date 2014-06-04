#include "koyoneNext.hpp"

#include <map>

Dir KoyoneNext::decideDir() const{
    using std::make_pair;
    Koyone::GridMap top =  Koyone::nurseryTime(grid) ? iterarion(nextPossibleWorld(grid), 4) : nextPossibleWorld10(grid);
    if(top.size() < DANGER){
        Koyone::uniq(top);
        int const ITERATION = 5;
        top = iterarion(std::move(top), ITERATION);
    }
    return (std::max_element(std::begin(top), std::end(top), Koyone::CompStatic()))->second;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld(Board::Grid grid){
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

Koyone::GridMap KoyoneNext::nextPossibleWorld2(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld3(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld2(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld4(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld3(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld5(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld4(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld6(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld5(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld7(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld6(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld8(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld7(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld9(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld8(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
}

Koyone::GridMap KoyoneNext::nextPossibleWorld10(Board::Grid grid){
    static std::map<Board::Grid, Koyone::GridMap> map;
    if(map.find(grid) != std::end(map)) return map[grid];
    Koyone::GridMap npws;
    for(auto const& e: nextPossibleWorld(grid)){
        for(auto const& e2: nextPossibleWorld9(e.first))
            npws.emplace_back(e2.first, e.second);
    }
    return map[grid] = npws;
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

