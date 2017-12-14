#ifndef BOT1_H
#define BOT1_H
#include "Piece.h"
#include <vector>

using std::vector;

#include "ValueIndex.h"

enum SIGNAL {
    SUCCESS1,
    FAILURE1
};

class Bot1 {
public:

    Bot1(int numPieces,Piece** allPieces);
    Bot1(int numPieces,Piece** allPieces, Frame *frame);
    ~Bot1();
    void preprocess();
    bool tryConcatTwoPieces(Piece *&piece1, Piece *piece2);
    void removePiece(int index);
    int getNumPieces() {
        return numPieces;
    }
    SIGNAL backtrack(Piece *currAnswer);
    void sortPrioIndex(Piece *piece, vector<int> &queueIndex);
    void run();
    Piece* chooseFirstPiece();
    bool isComplete(Piece* p1, Frame* p2);
    float distance(Dot* d1, Dot* d2);
    void getListValues(Piece *currAnswer, int index, vector<ValueIndex*> &listValues);
    void pushToListValues(vector<ValueIndex*> &listValues, ValueIndex *newAns);
    void removePieceFromDomain(Piece *piece);
private:

    Piece **allPieces;
    Frame *frame;
    vector<Piece*> currDomain;
    vector<Frame *> currVect;
    int numPieces;
};
#endif