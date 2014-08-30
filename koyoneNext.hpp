#pragma once

#include "koyone.hpp"

class KoyoneNext {
public:
    KoyoneNext(Board::Grid grid) : grid(grid) { }
    Dir decideDir() const;
    static Koyone::GridMap iterarion(Koyone::GridMap&& map, int ITERATION){
        Koyone::GridMap *npws = new Koyone::GridMap[ITERATION];
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
    static Koyone::GridMap nextPossibleWorld(Board::Grid);
private:
    Board::Grid grid;
    static unsigned int const constexpr DANGER = 30000;
    static Koyone::GridList nextPossibleWorldLeft(Board::Grid);
};

