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
    if(write(sfd, req.c_str(), req.size()) < 0) std::cerr << "something Wrong..." << std::endl;
    char buf[512];
    if(read(sfd, buf, 255) < 0) std::cerr << "something Wrong..." << std::endl;

    std::istringstream is(buf);
    std::string tmp, URI;

    while(tmp != "Location:") is >> tmp;
    is >> URI;
    sessionID = URI.substr(10, 40 );
    req = "GET " + URI + "\nHost: " + endpoint + "\nConnection: Keep-Alive\n\n";
    if(write(sfd, req.c_str(), req.size()) < 0) std::cerr << "something Wrong..." << std::endl;
    if(read(sfd, buf, 512) < 0) std::cerr << "something Wrong..." << std::endl;
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
    if(write(fd, req.str().c_str(), req.str().size()) < 0) std::cerr << "something Wrong..." << std::endl;
    char buf[512];
    if(read(fd, buf, 512) < 0) std::cerr << "something Wrong..." << std::endl;

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

std::pair<std::array<uint16_t, 1 << 16>, std::array<int, 1 << 16>> Board::makeTable(){
    std::array<uint16_t, 1 << 16> table;
    std::array<int, 1 << 16> scoreTable;
    for(int orig(0); orig < 1 << 16; ++orig){
        auto grid = orig;
        bool joined = false;
        bool hit = false;
        int score = 0;
        for(int i(0); i < 4; ++i){
            if(Board::get(grid, 0, i) == 0) continue;
            hit = false;
            for(int j(i-1); j >= 0;--j){
                if(Board::get(grid, 0, j) == 0) continue;
                if(Board::get(grid, 0, j) == Board::get(grid, 0, i) && !joined){
                    auto const gotten = Board::get(grid, 0, j);
                    grid = Board::set(grid, 0, j, gotten == 0b1111 ? gotten : gotten + 1);
                    grid = Board::set(grid, 0, i, 0);
                    score += pow2(gotten + 1);
                    joined = true;
                }else{
                    if(j + 1 != i){
                        grid = Board::set(grid, 0, j+1, Board::get(grid, 0, i));
                        grid = Board::set(grid, 0, i, 0);
                    }
                    joined = false;
                }
                hit = true;
                break;
            }
            if(i != 0 && ! hit){
                grid = Board::set(grid, 0, 0, Board::get(grid, 0, i));
                grid = Board::set(grid, 0, i, 0);
            }
        }
        table[orig] = grid;
        scoreTable[orig] = score;
    }
    return {table, scoreTable};
}

std::array<bool, 1 << 16> Board::makeMovableTable(){
    static std::array<uint16_t, 1 << 16> const table = Board::makeTable().first;
    std::array<bool, 1 << 16> mTable;
    for(int i(0); i< 1<<16; ++i)
        mTable[i] = table[i] != i;
    return mTable;
}

template Dir Board::decideDir<Kihime>();
template Dir Board::decideDir<Koyone>();
template Dir Board::decideDir<KoyoneNext>();
template Dir Board::decideDir<Nona7>();

