#include <utility>
#include <cmath>
#include <iostream>

#include "uct.hpp"
#include "kihime.hpp"

std::random_device UCT::rnd;
std::mt19937 UCT::mt = Kihime::mtInit();

Dir UCT::decideDir(){
    std::array<int, 4> counts;
    std::array<double, 4> sums;
    int count(4 * ITERATION_BOOT);
    counts.fill(ITERATION_BOOT);
    sums.fill(0);
    for(auto dir: allDirs){
        for(int i(0); i < ITERATION_BOOT; ++i){
            sums[dirToInt(dir)] += staticEval(playout(Board::moved(grid, dir), PLAYOUT_DEPTH));
        }
    }
    for(int i(0); i < ITERATION; ++i){
        std::array<double, 4> ucb1s;
        for(auto dir: allDirs){
            ucb1s[dirToInt(dir)] = (sums[dirToInt(dir)] / counts[dirToInt(dir)]) + std::sqrt(2 * std::log2(count) / counts[dirToInt(dir)]);
        }
        Dir bestDir = Dir::Up;
        double maxUcb1 = 0.0;
        for(auto dir: allDirs){
            if(maxUcb1 < ucb1s[dirToInt(dir)]){
                bestDir = dir;
                maxUcb1 = ucb1s[dirToInt(dir)];
            }
        }
        sums[dirToInt(bestDir)] += staticEval(playout(Board::moved(grid, bestDir), PLAYOUT_DEPTH));
    }
    Dir bestDir = Dir::Up;
    double maxAve = 0.0;
    for(auto dir: allDirs){
        if(maxAve < sums[dirToInt(dir)] / counts[dirToInt(dir)]){
            bestDir = dir;
            maxAve = sums[dirToInt(dir)] / counts[dirToInt(dir)];
        }
    }
    std::cout << maxAve << std::endl;
    return bestDir;
}

double UCT::staticEval(Board::Grid grid){
    int sum = Board::sum(grid);
    return std::min(sum/(8192 * 1.0), 1.0);
}

Board::Grid UCT::playout(Board::Grid grid, int depth){
    if(depth == 0) return grid;
    if(! Board::alive(grid)) return grid;
    Dir dir = allDirs[mt()%4];
    grid = Board::moveAndBirth(grid, dir).second;
    // for(int i(0); i < 4; ++i){
    //     for(int j(0); j < 4; ++j){
    //         grid = Board::set(grid, i, j, 1);
    //     }
    // }
    return playout(grid, depth - 1);
}

