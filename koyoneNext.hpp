#pragma once

#include "koyone.hpp"

class KoyoneNext {
public:
    KoyoneNext(Board::Grid grid) : grid(grid) { }
    Dir decideDir() const;
private:
    Board::Grid grid;
    static Koyone::GridMap nextPossibleWorld(Board::Grid);
    static Koyone::GridList nextPossibleWorldLeft(Board::Grid);
};

Dir KoyoneNext::decideDir() const{
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
    Koyone::uniq(npw);
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
    Koyone::uniq(npw2);
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
    Koyone::uniq(npw3);
    std::cout << "size of 3: " << npw3.size() << std::endl;
    if(! Koyone::nurseryTime(grid)) {
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
        Koyone::uniq(npw4);
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
    printf("aaaaa!!!!!###########\n");
    int zeros(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0)
                left = Board::set(left, i, j, 1);
    return { left };
}

