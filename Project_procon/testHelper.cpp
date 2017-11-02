#include "helper.h"
#include "Frame.h"
#include "Piece.h"
#include <iostream>


using std::cout;
using std::endl;

#define INPUT_DATA "example/data1.txt"

int main(int argc, char **argv) {

    Frame *frame;
    Piece **pieces;
    int numPieces;
    cout << "Reading data..";
    parseInput(INPUT_DATA, frame, pieces, numPieces);
    cout << "Done!" << endl;
    cout << (*frame);
    return 0;
}
