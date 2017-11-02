/*
- class Piece inherit from Frame. Describe Piece
- TO-DO : rotate piece ------> Kiên
*/

#ifndef PIECE_H
#define PIECE_H
#include "Frame.h"

class Piece: public Frame{
public:
    Piece();
    Piece(int numVertices, int id);
    /*
     - Hàm này kiểm tra xem otherPiece có khả năng lắp vào được pieie không
        tại index của piece và indexOfOthePiece của otherPiece
    */
    bool isPossibleToFill(int index, Piece *otherPiece, int indexOfOtherPiece);
    /*
    - Giống hàm isPossibleToFill nhưng cho otherPiece flip đi.
    */
    bool isPossibleToFillFlip(int index, Piece *otherPiece, int indexOfOtherPiece);
};

#endif
