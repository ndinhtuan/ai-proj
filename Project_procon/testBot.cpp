#include "Grid.h"
#include "helper.h"
#include "Bot.h"
#include <iostream>
#include <deque> 


using std::cout;
using std::endl;

#include <cstring>

using std::string;

#define INPUT_DATA "example/data1.txt"

/*
Thêm phần check overlap và ghép hình.
*/

int main(int argc, char** argv) {

    Frame *frame;
    Piece **pieces;
    int numPieces;
    cout << "Reading data..";
    parseInput(INPUT_DATA, frame, pieces, numPieces);
    cout << " Done!" << endl;

    Grid grid(70, 110, 10);
    Bot *bot = new Bot(numPieces, frame, pieces);
    bot->run();
    return 0;
}