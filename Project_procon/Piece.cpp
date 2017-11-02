#include "Piece.h"

Piece::Piece()
{
}

Piece::Piece(int numVertices, int id) : Frame(numVertices, id)
{
}

// TO-DO
/*
 Lắp được khi : 
    - Cần góc tại đỉnh của piece <= góc ngoài của lời giải hiện tại 
    - ít nhất cạnh chung cần tỷ lệ (thương là hữu tỷ)
    - lắp vào -> tịnh tiến thì các tọa độ của piece mới sẽ nguyên
    - không đè hình sau khi lắp vào
*/
bool Piece::isPossibleToFill(int index, Piece *otherPiece, int indexOfOtherPiece) {

   return false;
}

bool isPossibleToFillFlip(int index, Piece *otherPiece, int indexOfOtherPiece) {

    return false;
}