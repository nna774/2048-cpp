#include "kihimeNext.hpp"
#include "kihime.hpp"

std::random_device KihimeNext::rnd;
std::mt19937 KihimeNext::mt = Kihime::mtInit();

Dir KihimeNext::decideDir(){
    std::array<unsigned int, 4> cnt, scores;
    cnt.fill(0);
    scores.fill(0);
    std::array<double, 4> aves;
    aves.fill(0);
    Dir max = Dir::Up;
    double maxAve = 0.0;
    bool flg = true;

label:;
    for(int i(0); i < ITERATION; ++i){
        Dir dir = allDirs[mt()%4];
        while(! Board::movable(grid, dir)) dir = allDirs[mt()%4];
        auto moved = Board::moved(grid, dir);
        unsigned score = toDead(moved, 0);
        scores[dirToInt(dir)] += score;
        ++cnt[dirToInt(dir)];
    }
    for(int i(0); i < 4; ++i){
        if(cnt[i] == 0){
            aves[i] = 0;
            continue;
        }
        aves[i] = scores[i] / (cnt[i] * 1.0);
    }
    for(int i(0); i < 4; ++i){
        if(aves[i] > maxAve){
            max = allDirs[i];
            maxAve = aves[i];
        }
    }
    if(flg && maxAve < DANGER){
        flg = false;
        maxAve = 0;
        goto label;
    }
    return max;
}

unsigned KihimeNext::toDead(Board::Grid grid, unsigned score) {
    if(! Board::alive(grid)) return score;

    Dir dir = allDirs[mt()%4];
    while(! Board::movable(grid, dir)) dir = allDirs[mt()%4];
    Board::Grid moved = Board::moveAndBirth(grid, dir).second;
    return toDead(moved, score + Board::getScore(grid, dir));
}

