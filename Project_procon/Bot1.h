#ifndef BOT1_H
#define BOT1_H
#include "Piece.h"

class Bot1 {
public:

    Bot1(int numPieces,Piece** allPieces);
    void preprocess();
    bool tryConcatTwoPieces(Piece *&piece1, Piece *piece2);
    void removePiece(int index);
    int getNumPieces() {
        return numPieces;
    }
private:

    Piece **allPieces;
    int numPieces;
};
#endif