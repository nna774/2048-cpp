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
    sums.fill(0);
    for(auto dir: allDirs){
        for(int i(0); i < ITERATION_BOOT; ++i){
            sums[dirToInt(dir)] += staticEval(playout(Board::moved(grid, dir), PLAYOUT_DEPTH));
            ++counts[dirToInt(dir)];
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
    // std::array<std::array<double, 4>, 4> weight = {{
    //         {{0.3, 0.2, 0.1, 0.0}},
    //         {{0.4, 0.3, 0.2, 0.1}},
    //         {{0.6, 0.5, 0.3, 0.2}},
    //         {{1.0, 0.6, 0.4, 0.3}}
    //     }};

    // [[80,40,40,40],[30,10,10,10],[3,1,1,3],[1,1,1,1]];
    std::array<std::array<double, 4>, 4> weight = {{
            {{0.01, 0.01, 0.01, 0.01}},
            {{0.03, 0.01, 0.01, 0.03}},
            {{0.30, 0.10, 0.10, 0.10}},
            {{0.80, 0.40, 0.40, 0.40}}
        }};
    double val = 0.0;
    for(auto dir: allDirs){
        double est = product(weight, Board::rotate(grid, dir));
        val = std::max(est, val);
    }
    return std::min(val/(8192 * 1.0), 1.0);
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

double UCT::product(std::array<std::array<double, 4>, 4> const& weight, Board::Grid grid){
    double sum(0);
    for(int i(0); i < 4; ++i){
        for(int j(0); j < 4; ++j){
            sum += weight[i][j] * Board::pow2(Board::get(grid, i, j));
        }
    }
    return sum;
}
