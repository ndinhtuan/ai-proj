#include "Grid.h"
#include "helper.h"

#include <iostream>


using std::cout;
using std::endl;

#define INPUT_DATA "example/data1.txt"

int main(int argc, char** argv) {

    Frame *frame;
    Piece **pieces;
    int numPieces;
    cout << "Reading data..";
    parseInput(INPUT_DATA, frame, pieces, numPieces);
    cout << " Done!" << endl;

    Grid grid(70, 110, 10);

    for (int i = 0; i < numPieces; i++) {
        grid.showPiece(pieces[i]);
    }

    grid.showPiece(frame);
    return 0;
}
