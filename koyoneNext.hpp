#pragma once

#include <vector>

#include "koyone.hpp"

class KoyoneNext {
public:
    KoyoneNext(Board::Grid grid) : grid(grid) { }
    Dir decideDir() const;
    static Koyone::GridMap iterarion(Koyone::GridMap const& map, int iter){
        std::vector<Koyone::GridMap> npws(iter);
        npws[0] = map;
        for(int i(0); i < iter - 1; ++i){
            npws[i+1].reserve(npws[i].size());
            for(auto const& e: npws[i]){
                for(auto const& e2: nextPossibleWorld(e.first))
                    npws[i+1].emplace_back(e2.first, e.second);
            }
            if(npws[i+1].empty()) return npws[i];
        }
        return npws[iter - 1];
    }
    static Koyone::GridMap nextPossibleWorld(Board::Grid);
private:
    Board::Grid grid;
    static unsigned int const constexpr DANGER = 30000;
    static Koyone::GridList nextPossibleWorldLeft(Board::Grid);
};

