#include "ValueIndex.h"

ValueIndex::ValueIndex(Piece *piece, int index, bool flip) {

    this->piece = piece;
    this->indexPiece = index;
    this->flip = flip;
    this->score = 0; 
}