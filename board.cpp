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
#include "koyoneNext.hpp"
#include "kihime.hpp"
#include "nona7.hpp"

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
    
    std::string req = "GET /hi/start/json HTTP/1.1\nHost: " + endpoint + "\n\n"; //// #### TODO: 共通部分の関数化 ####
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
    std::cout << v << std::endl;
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
    show(grid);
}

void Board::show(Board::Grid const grid){
    std::cout << "+----+----+----+----+\n";
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

// Dir Board::decideDir() {
//     Kihime kihime(grid);
//     return kihime.decideDir();
// }

template<class T>
Dir Board::decideDir() {
    T oracle(grid);
    return oracle.decideDir();
}

Board::Grid Board::rotate(Board::Grid grid, Dir dir){
    switch(dir){
    case Dir::Up:
        return grid;
    case Dir::Down:
        return 0;
    case Dir::Right:
        return 0;
    case Dir::Left:
    default :
        return 0;
    }
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
        bfjn = UINT64_C(0x0F000F000F000F00),
        cgko = UINT64_C(0x00F000F000F000F0),
        dhlp = UINT64_C(0x000F000F000F000F);
    return
        ((grid & aeim) >> 12)|
        ((grid & bfjn) >> 4) |
        ((grid & cgko) << 4) |
        ((grid & dhlp) << 12);
}

Board::Grid Board::moved(Board::Grid grid, Dir dir){
    if(dir == Dir::Up) return transpose(moveLeft(transpose(grid)));
    if(dir == Dir::Down) return transpose(gridMirror(moveLeft(gridMirror(transpose(grid)))));
    if(dir == Dir::Right) return gridMirror(moveLeft(gridMirror(grid)));
    return moveLeft(grid);
}

bool Board::movable(Board::Grid grid, Dir dir){
    return moved(grid, dir) != grid;
}

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

template Dir Board::decideDir<Kihime>();
template Dir Board::decideDir<Koyone>();
template Dir Board::decideDir<KoyoneNext>();
template Dir Board::decideDir<Nona7>();

