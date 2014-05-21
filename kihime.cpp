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
        auto moved = Board::moved(grid, dir);
        int depth = toDead(moved, 0);
        depths[dirToInt(dir)] += depth;
        ++cnt[dirToInt(dir)];
    }
    for(int i(0); i < 4; ++i){
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
        auto moved = moveAndBirth(grid, dir);
        return toDead(moved, depth + 1);
    }
    return depth;
}

Board::Grid Kihime::moveAndBirth(Board::Grid grid, Dir dir){
    auto moved = Board::moved(grid, dir);
    int zeros(0);
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(moved, i, j) == 0) ++zeros;
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

    // int const MAX_ITERATION = 4000;
    // std::array<unsigned long,4> sums, counts, aves;
    // sums.fill(0);
    // counts.fill(0);
    // aves.fill(0);
    // for(int i(0); i < MAX_ITERATION; ++i){
    //     int dirI = rand4(mt);
    //     // std::cout << dirI;
    //     Dir dir = allDirs[dirI];
    //     auto movedGrid = movedAndBirth(grid, dir);
    //     // std::cout << "a";
    //     sums[dirToInt(dir)] += toDead(movedGrid);
    //     ++counts[dirToInt(dir)];
    // }
    // for(int i(0); i < 4; ++i) aves[i] = sums[i]/counts[i];
    // Dir maxDir = Dir::Down;
    // int maxAve = 0;
    // for(int i(0); i < 4; ++i){
    //     // std::cout << aves[i] << std::endl;
    //     if(aves[i] > maxAve){
    //         maxDir = allDirs[i];
    //         maxAve = aves[i];
    //     }
    // }
    // // std::cout << dirToInt(maxDir);
    // auto m = moved(grid, maxDir);
    // for(int i(0); i < 4; ++i)
    //     for(int j(0); j < 4; ++j)
    //         if(m[i][j] != grid[i][j]) {
    //             // std::cout << "!!!!!" << std::endl;
    //             // std::cout << i << ',' << j << ',' << m[i][j] << ',' << grid[i][j];
    //             return maxDir;
    //         }
    // // std::cout << "xxxxx";
    // return allDirs[rand4(mt)]; // つらい
    // return maxDir;

// std::pair<bool,Board::Grid> Board::movedAndBirth(Board::Grid const& grid, Dir dir) { // 動けばfirst はtrue
//     std::random_device rnd;
//     std::vector< std::uint_least32_t> v(10);
//     std::generate(begin(v), end(v), ref(rnd));
//     std::mt19937 mt(std::seed_seq(begin(v), end(v)));
    
//     auto m = moved(grid, dir);
//     if(m == grid) return std::make_pair(false, m);
//     int birth = "2222222224"[rand10(mt)] - '0';
//     int zeros(0);
//     for(int i(0); i < 4; ++i)
//         for(int j(0); j < 4; ++j)
//             if(m[i][j] == 0) ++zeros;
//     std::uniform_int_distribution<int> dice(0,zeros-1);
//     int pos = dice(mt);
//     for(int i(0); i < 4; ++i)
//         for(int j(0); j < 4; ++j)
//             if(m[i][j] == 0)
//                 if(pos-- == 0) m[i][j] = birth;
//     return std::make_pair(true, m);
// }

