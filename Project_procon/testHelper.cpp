#include "helper.h"
#include "Frame.h"
#include "Piece.h"
#include "Bot.h"
#include "Bot1.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

#include "Grid.h"

#define INPUT_DATA "example/data1.txt"

int main(int argc, char **argv) {

    Frame *frame;
    Piece **pieces;
    Grid grid(110, 110, 10);
    int numPieces;
    cout << "Reading data..";
    parseInput(INPUT_DATA, frame, pieces, numPieces);
    cout << "Done! " << numPieces << " Pieces " << endl;

    for (int i = 0; i < numPieces; i++) {
        // cout << *pieces[i];
        while (!pieces[i]->checkIsClockWise()) {
            cout << "T" << endl;
            pieces[i]->reverseDot(1, pieces[i]->getNumVertices()-1);
        }
        // grid.showPiece(pieces[i]);

    }

    // vector<Frame *> vect; 
    // vect.push_back(pieces[3]);
    // vect.push_back(pieces[4]);
    // grid.showPieceSameTime(vect);

    // Piece *result = new Piece(*pieces[4]);
    // pieces[4]->transition(pieces[3], 5, 7, EQUAL, result);
    // vect.push_back(result);
    // grid.showPieceSameTime(vect);

    // Bot1 bot(numPieces, pieces);
    // bot.preprocess();
    Bot1 *bot = new Bot1(numPieces, pieces);
    bot->preprocess();
    cout << "Bot run completely." << endl;

    delete frame;
    for (int i = 0; i < bot->getNumPieces(); i++) {
        if(pieces[i] != NULL) delete pieces[i];
    }
    delete[] pieces;
    delete bot;
    return 0;
}
