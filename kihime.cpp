#include "kihime.hpp"

std::random_device Kihime::rnd;
std::mt19937 Kihime::mt = Kihime::mtInit();

std::mt19937 Kihime::mtInit(){
    std::vector<std::uint_least32_t> v(10);
    for(auto& e:v) e = rnd();
    std::seed_seq seq(begin(v), end(v));
    std::mt19937 mt(seq);
    return mt;
}

Dir Kihime::decideDir(){
    std::array<int, 4> cnt, depths;
    cnt.fill(0);
    depths.fill(0);
    std::array<double, 4> aves;
    aves.fill(0);

    for(int i(0); i < ITERATION; ++i){
        Dir dir = allDirs[mt()%4];
        while(! Board::movable(grid, dir)) dir = allDirs[mt()%4];
        auto moved = Board::moved(grid, dir);
        int depth = toDead(moved, 0);
        depths[dirToInt(dir)] += depth;
        ++cnt[dirToInt(dir)];
    }
    for(int i(0); i < 4; ++i){
        if(cnt[i] == 0){
            aves[i] = 0;
            continue;
        }
        aves[i] = depths[i] / (cnt[i] * 1.0);
    }
    Dir max = Dir::Up;
    double maxAve = 0.0;
    for(int i(0); i < 4; ++i){
        if(aves[i] > maxAve){
            max = allDirs[i];
            maxAve = aves[i];
        }
    }
    return max;
}

int Kihime::toDead(Board::Grid grid, int depth) {
    if(Board::alive(grid)){
        auto dir = allDirs[mt()%4];
        while(! Board::movable(grid, dir)) dir = allDirs[mt()%4];
        auto moved = moveAndBirth(grid, dir);
        return toDead(moved, depth + 1);
    }
    return depth;
}

inline Board::Grid Kihime::moveAndBirth(Board::Grid grid, Dir dir){
    auto moved = Board::moved(grid, dir);
    int zeros = Board::countZeroGrid(grid);
    if(zeros > 0){
        int point = mt() % zeros;
        int birth = "1111111112"[mt() % 10] - '0'; // 10% で4、 のこりが2
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
                if(Board::get(moved, i, j) == 0)
                    if(point-- == 0) moved = Board::set(moved, i, j, birth);
    }else{
        // 動けなかった時だけだと思う
    }
    return moved;
}
