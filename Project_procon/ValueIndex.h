/*
- Struct ValueIndex định nghĩa cấu trúc của 1 cái value có thể lắp vào được index i 
của frame.
@piece để lưu cái piece có thể lắp vào index i của frame 
@indexPiece trả về đỉnh của Piece mà lắp vào dược đỉnh của frame.
@flip để xác định xem cái piece có bị flip hay không.
*/
#ifndef VALUEINDEX_H
#define VALUEINDEX_H

#include "Piece.h"

/*
- Hiển thị xem 2 Piece lắp với nhau chung 1 đỉnh nhưng theo cạnh nào :
    + Theo cả 2 cạnh kề với đỉnh (khít)
    + Theo cạnh trước của đỉnh (i, i+1) -> chỉ hợp với 1 cạnh
    + Theo canh sau của đỉnh (i, i-1) -> chỉ hợp với 1 cạnh
*/
enum class SharedEdgeStyle {
    EQUAL_ANGLE, // 2 Piece khít nhau tại đỉnh đang xét, do có góc bằng nhau 
    BOTH_DIFF_ANGLE, // piece có thể lắp vào RIGHT hoặc LEFT
    NEXT, // cạnh trước của đỉnh chung (tạo ra bởi 2 đỉnh i và i + 1)
    PREV  // cạnh trước của đỉnh chung (tạo ra bởi 2 đỉnh i và i - 1)
};

struct ValueIndex {

    Piece *piece;
    int indexPiece;
    bool flip; // Piece có flip hay không khi lắp vào
    SharedEdgeStyle styleShared;

    /*
    score để đánh giá điểm của Value để đưa vào priorityQueue
    */
    int score;
    // public:
    ValueIndex(Piece *, int, bool);
};

#endif