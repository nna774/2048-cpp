#include "board.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include "picojson/picojson.h"

#include "koyone.hpp"
#include "kihime.hpp"

Board::Board(){
    Board("http", "2048.semantics3.com", "80");
}

Board::Board(std::string const, std::string const endpoint, std::string const port) : endpoint(endpoint) {
    struct addrinfo addr;
    addr.ai_family = AF_UNSPEC;
    addr.ai_socktype = SOCK_STREAM;
    addr.ai_flags = 0;
    addr.ai_protocol = 0;
    struct addrinfo *result, *rp;
    int s, sfd;

    s = getaddrinfo(endpoint.c_str(), port.c_str(), &addr, &result);

    if (s != 0) {
        std::fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        std::exit(-1);
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == nullptr) exit(-1);
    freeaddrinfo(result);
    fd = sfd;
    
    std::string req = "GET /hi/start/json HTTP/1.1\nHost: " + endpoint + "\n\n";
    write(sfd, req.c_str(), req.size());
    char buf[512];
    read(sfd, buf, 255);

    std::istringstream is(buf);
    std::string tmp, URI;
    std::getline (is, tmp);
    std::getline (is, tmp);
    std::getline (is, tmp);
    std::istringstream is02(tmp);

    is02 >> tmp >> URI;
    sessionID = URI.substr(10, 40 );
    req = "GET " + URI + "\nHost: " + endpoint + "\nConnection: Keep-Alive\n\n";
    write(sfd, req.c_str(), req.size());
    read(sfd, buf, 512);
    std::istringstream is03(buf);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp); // JSON は8行目にある 酷い……
    picojson::value v;
    is03 >> v;
    // std::cout << v;
    grid = UINT64_C(0);
    auto g = v.get("grid");
    picojson::array arr = g.get<picojson::array>();
    for(int i(0); i < 4; ++i){
        picojson::array a = arr[i].get<picojson::array>();
        for(int j(0); j < 4; ++j)
            // grid[i][j] =  a[j].get<double>()
            grid = set(grid, i, j, log2(a[j].get<double>()));
    }
    // for(auto e: grid){
    //     for(auto f: e)
    //         std::cout << f;
    //     std::cout << std::endl;
    // }
}

bool Board::move(Dir dir){
    std::ostringstream req;
    req << "GET /hi/state/" << sessionID << "/move/" << dirToInt(dir) << "/json\nHost: " << endpoint << "\nConnection: Keep-Alive\n\n";
    write(fd, req.str().c_str(), req.str().size());
    char buf[512];
    read(fd, buf, 512);

    std::string tmp;
    // std::cout << buf << '\n';
    std::istringstream is03(buf);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp);
    std::getline (is03, tmp); // JSON は8行目にある 酷い……
    picojson::value v;
    is03 >> v;

    grid = UINT64_C(0);
    // std::cout << v << std::endl;
    auto g = v.get("grid");
    picojson::array arr = g.get<picojson::array>();
    for(int i(0); i < 4; ++i){
        picojson::array a = arr[i].get<picojson::array>();
        for(int j(0); j < 4; ++j)
            // grid[i][j] =  a[j].get<double>()
            grid = set(grid, i, j, log2(a[j].get<double>()));
    }
    // for(auto e: grid){
    //     for(auto f: e)
    //         std::cout << f;
    //     std::cout << std::endl;
    // }

    if(v.get("over").evaluate_as_boolean()){
        throw v.serialize();
    }
//    std::cout << v.get("moved") ;//== "true" << std::endl;
    return v.get("moved").evaluate_as_boolean();
}

void Board::show() const {
    std::cout << "+----+----+----+----+\n";
    // for(auto e: grid){
    //     std::cout << '|' << std::setfill(' ') << std::setw(4) << std::right << int(e[0])
    //               << '|' << std::setfill(' ') << std::setw(4) << std::right << int(e[1])
    //               << '|' << std::setfill(' ') << std::setw(4) << std::right << int(e[2])
    //               << '|' << std::setfill(' ') << std::setw(4) << std::right << int(e[3])
    //               << '|' << '\n';
    // }
    for(int i(0); i < 4; ++i)
        std::cout << '|' << std::setfill(' ') << std::setw(4) << std::right << pow2(get(grid, i, 0))
                  << '|' << std::setfill(' ') << std::setw(4) << std::right << pow2(get(grid, i, 1))
                  << '|' << std::setfill(' ') << std::setw(4) << std::right << pow2(get(grid, i, 2))
                  << '|' << std::setfill(' ') << std::setw(4) << std::right << pow2(get(grid, i, 3))
                  << '|' << '\n';
    std::cout << "+----+----+----+----+" << std::endl;
}

int Board::get(Board::Grid grid, int i, int j){
    int pos = i * 16 + j * 4;
    return (grid >> pos) & 0b1111;
}
Board::Grid Board::set(Board::Grid const grid, int i, int j, int v){
    int pos = i * 16 + j * 4;
    return (grid & ~(UINT64_C(0b1111) << pos)) | ((v & (UINT64_C(0b1111))) << pos); // あふれてるかも
}

// int Board::toDead(std::pair<bool,Grid> const& grid) {
//     std::pair<bool,Grid> movedGrid = grid;
//     while(alive(movedGrid.second)){ // 死ぬまで
//         auto oldGrid = movedGrid;
//         int flg(0);
//         int toI = rand4(mt);
//         flg = 1 << (toI+1);
//         auto to = allDirs[toI];
//         movedGrid = movedAndBirth(oldGrid.second, to);
//         if(movedGrid.first == false){
//             while(flg != (1 << 4) + (1 << 3) + (1 << 2) + (1 << 1)){
//                 int tmp = rand4(mt);
//                 if(1 << tmp & flg) continue; // 既に見た
//                 // あった
//                 movedGrid = movedAndBirth(oldGrid.second, allDirs[tmp]);
//                 flg = 1 << (tmp+1);
//                 if(movedGrid.first) goto label; // goto!!
//             }
//             movedGrid = oldGrid;
//             break; // 全部ダメだった
//         }
//     label: ;
//     }
//     return staticEval(movedGrid.second);
// }

Dir Board::decideDir() {
    static int cnt = 0;
    return allDirs[cnt++%4];
    Kihime kihime(grid);
    Koyone koyone(grid);
    return koyone.decideDir();
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
}

Board::Grid Board::rotate(Board::Grid grid, Dir dir){
    if(dir == Dir::Up) return grid;
    if(dir == Dir::Right)
        return // {{{{grid[0][3], grid[1][3], grid[2][3], grid[3][3]}},
               //  {{ grid[0][2], grid[1][2], grid[2][2], grid[3][2]}},
               //  {{ grid[0][1], grid[1][1], grid[2][1], grid[3][1]}},
               //  {{ grid[0][0], grid[1][0], grid[2][0], grid[3][0]}} }}
    0;
    if(dir == Dir::Down)
        return // {{{{grid[3][3], grid[3][2], grid[3][1], grid[3][0]}},
               //  {{ grid[2][3], grid[2][2], grid[2][1], grid[2][0]}},
               //  {{ grid[1][3], grid[1][2], grid[1][1], grid[1][0]}},
               //  {{ grid[0][3], grid[0][2], grid[0][1], grid[0][0]}} }}
    0;
    if(dir == Dir::Left)
        return // {{{{grid[3][0], grid[2][0], grid[1][0], grid[0][0]}},
               //  {{ grid[3][1], grid[2][1], grid[1][1], grid[0][1]}},
               //  {{ grid[3][2], grid[2][2], grid[1][2], grid[0][2]}},
               //  {{ grid[3][3], grid[2][3], grid[1][3], grid[0][3]}} }}
    0;
    return 0; // never come
}

Board::Grid Board::transpose(Board::Grid grid){
    uint64_t
        cdgh = UINT64_C(0x00FF00FF00000000),
        ijmn = UINT64_C(0x00000000FF00FF00),
        bdjl = UINT64_C(0x0F0F00000F0F0000),
        egmo = UINT64_C(0x0000F0F00000F0F0);
    grid = (grid & ~cdgh & ~ijmn) | ((grid & cdgh) >> 24) | ((grid & ijmn) << 24);
    return (grid & ~bdjl & ~egmo) | ((grid & bdjl) >> 12) | ((grid & egmo) << 12);
}

Board::Grid Board::gridMirror(Board::Grid grid){
    uint64_t
        aeim = UINT64_C(0xF000F000F000F000),
        bfin = UINT64_C(0x0F000F000F000F00),
        cgko = UINT64_C(0x00F000F000F000F0),
        dhlp = UINT64_C(0x000F000F000F000F);
    return
        ((grid & aeim) >> 12)|
        ((grid & bfin) >> 4) |
        ((grid & cgko) << 4) |
        ((grid & dhlp) << 12);
}


// int Board::moveUpImp(int tmp){
//     bool joined = false;
//     bool hit = false;
//     for(int i(0); i < 4;++i){
//         joined = false;
//         if((tmp >> (i * 4) & 0b1111) == 0) continue;
//         hit = false;
//         for(int j(i-1); j >= 0;--j){
//             if((tmp >> (j * 4) & 0b1111) == 0) continue;
//             if((tmp >> (j * 4) & 0b1111) == (tmp >> (i * 4) & 0b1111) && !joined){
//                 //tmp[j] *= 2;
//                 tmp += 1 << (j * 4);
//                 tmp |= ~(0b1111 << i);
//                 joined = true;
//             }else{
//                 if(j + 1 != i){
//                     // tmp[j+1] = tmp >> (i * 4) & 0b1111;
//                     tmp |= (tmp & ((~0b1111) << (j + 1) * 4 )) | (tmp >> (i * 4) & 0b1111) << (j + 1) * 4;
//                     tmp |= ~(0b1111 << i);
//                 }
//                 joined = false;
//             }
//             hit = true;
//             break;
//         }
//         if(i != 0 && ! hit){
//             // tmp[0] = tmp >> (i * 4) & 0b1111;
//             tmp |= (tmp & (~0b1111)) | (tmp >> (i * 4) & 0b1111);
//             tmp |= ~(0b1111 << i);
//             joined = false;
//         }
//     }
//     return tmp;
// }

Board::Grid Board::moveLeft(Board::Grid grid){
    for(int k(0); k < 4;++k){
        bool joined = false;
        bool hit = false;
        for(int i(0); i < 4;++i){
            joined = false;
            if(get(grid, k, i) == 0) continue;
            hit = false;
            for(int j(i-1); j >= 0;--j){
                if(get(grid, k, j) == 0) continue;
                if(get(grid, k, j) == get(grid, k, i) && !joined){
                    grid = set(grid, k, j, get(grid, k, j) + 1);
                    grid = set(grid, k, i, 0);
                    joined = true;
                }else{
                    if(j + 1 != i){
                        grid = set(grid, k, j+1, get(grid, k, i));
                        grid = set(grid, k, i, 0);
                    }
                    joined = false;
                }
                hit = true;
                break;
            }
            if(i != 0 && ! hit){
                grid = set(grid, k, 0, get(grid, k, i));
                grid = set(grid, k, i, 0);
                joined = false;
            }
        }
    }
    return grid;
}
Board::Grid Board::moved(Board::Grid grid, Dir dir){
    // if(dir != Dir::Up) return rotate((moveUp (rotate(grid, mirror(dir)))), dir);

    return moveLeft(grid);
}

bool Board::movable(Board::Grid grid, Dir dir){
    // auto m = moved(grid, dir);
    // for(int i(0); i < 4; ++i)
    //     for(int j(0); j < 4; ++j)
    //         if(m[i][j] != grid[i][j]) return true;
    // return false; 
    return moved(grid, dir) != grid;
}

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

bool Board::alive(Board::Grid grid) {
    for(int i(0); i < 4;++i) if(movable(grid, allDirs[i])) return true;
    return false;
}

int Board::log2(int i){
    switch(i){
    case 0:
        return 0;
    case 2:
        return 1;
    case 4:
        return 2;
    case 8:
        return 3;
    case 16:
        return 4;
    case 32:
        return 5;
    case 64:
        return 6;
    case 128:
        return 7;
    case 256:
        return 8;
    case 512:
        return 9;
    case 1024:
        return 10;
    case 2048:
        return 11;
    case 4096:
        return 12;
    case 8192:
        return 13;
    case 16384:
        return 14;
    default:
        return 15;
    }
}

int Board::pow2(int i){
    static int const constexpr table[] = {0,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
    return table[i];
}

Board::GridList_t<std::pair<Board::Grid, Dir>> Board::nextPossibleWorld(Board::Grid grid){
    GridList_t<std::pair<Grid, Dir>> vec;
    vec.reserve(128);
    for(auto dir: allDirs){
        GridList tmp = nextPossibleWorldUp(rotate(grid, dir));
        for(auto const& e: tmp) vec.push_back(std::make_pair(rotate(e, mirror(dir)), dir));
    }
//    std::cout << "size: " << vec.size() << std::endl;
    return vec;
}

Board::GridList Board::nextPossibleWorldUp(Board::Grid grid){
    auto left = moveLeft(grid);
    if(left == grid) return {};
    // int zeros(0);
    // for(int i(0); i < 4; ++i)
    //     for(int j(0); j < 4; ++j)
    //         if(up[i][j] == 0) ++zeros;
    // GridList tmps(zeros * 2, up);
    // int cnt(0);
    // for(int i(0); i < 4; ++i)
    //     for(int j(0); j < 4; ++j)
    //         if(up[j][i] == 0){
    //             tmps[cnt][j][i] = 2;
    //             tmps[cnt+1][j][i] = 4;
    //         }
    // return tmps;
}

