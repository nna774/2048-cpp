#include <iostream>
#include "board.hpp"


static std::string const protocol = "http";
static std::string const endpoint = "ring"; // #### TODO: 適当にargv 読むようにする ####
static std::string const port = "2048";

int main(int, char**){
    Board b(protocol, endpoint, port);
    b.show();
    // b.grid = b.moved(b.grid, Dir::Up);
    // b.show();

    try{
        while(1){
            // auto tmp = b.grid;
            auto dir = b.decideDir();
            // auto moved = Board::moved(tmp, dir);
            b.move(dir);
            b.show();
            // auto npw = Koyone::nextPossibleWorld(tmp);
            // auto a = 0;
            // if((a = __builtin_popcountll(b.grid ^ moved)) > 1){
            //     std::cout << a << std::endl;
            //     b.show();
            //     b.grid = moved;
            //     b.show();
            //     throw;
            // }
            // std::cout << a << std::endl;
            // auto tmp = b.grid;
            // b.show();
            // b.grid = Board::moveLeft(tmp);
            // b.show();
            // throw "na-";
        }
    }
    catch (std::string v){
        std::cerr << v << std::endl;
        b.show();
    }
}
