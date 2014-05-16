#include <iostream>
#include "board.hpp"


static std::string const protocol = "http";
static std::string const endpoint = "ring";
static std::string const port = "2048";

int main(int, char**){
    Board b(protocol, endpoint, port);
    b.show();
    // b.grid = b.moved(b.grid, Dir::Up);
    // b.show();

    try{
        while(1){
            auto dir = b.decideDir();
            b.move(dir);
            b.show();
        }
    }
    catch (std::string v){
        std::cout << v << std::endl;
        b.show();
    }
}



















