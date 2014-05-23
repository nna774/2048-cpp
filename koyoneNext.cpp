#include "koyoneNext.hpp"

Dir KoyoneNext::decideDir() const{
    using std::make_pair;
    Koyone::GridMap top;
    if(Koyone::nurseryTime(grid)) {
        bool flg = true;
        auto npw = Koyone::nextPossibleWorld(grid);
        decltype(npw) npw2, npw3;
        npw2.reserve(1024 * 12);
        for(auto e: npw){
            for(auto e2: Koyone::nextPossibleWorld(e.first))
                npw2.push_back(make_pair(e2.first, e.second));
        }
        if(npw2.empty()) {
            top = std::move(npw);
            flg = false;
        }
        for(auto e: npw2){
            for(auto e2: Koyone::nextPossibleWorld(e.first))
                npw3.push_back(make_pair(e2.first, e.second));
        }
        if(flg && npw3.empty()) {
            top = std::move(npw2);
            flg = false;
        }
        top = npw3;
    }else{
        bool flg = true;
        static int const constexpr ITERATION = 10;
        Koyone::GridMap npws[ITERATION];
        npws[0] = nextPossibleWorld(grid);
        for(int i(0); i < ITERATION - 1; ++i){
            for(auto const& e: npws[i]){
                for(auto const& e2: nextPossibleWorld(e.first))
                    npws[i+1].push_back(make_pair(e2.first, e.second));
            }
            if(flg && npws[i+1].empty()) {
                top = std::move(npws[i]);
                flg = false;
            }
        }
        top = npws[ITERATION - 1];
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

Koyone::GridList KoyoneNext::nextPossibleWorldLeft(Board::Grid grid){
    auto left = Board::moveLeft(grid);
    if(left == grid) return {};
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0)
                left = Board::set(left, i, j, 1);
    return { left };
}

