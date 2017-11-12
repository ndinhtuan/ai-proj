/*
- class Piece inherit from Frame. Describe Piece
- TO-DO : rotate piece ------> Kiên
*/

#ifndef PIECE_H
#define PIECE_H
#include "Frame.h"
#include "Grid.h"

/*
+ Style Fill mô tả giá trị trả về cho hàm isPossibleFill. Dựa vào 
    enum này ta có thể biết được cách để transfrom hình.
    - NOT_FILL : Không điền được
    - EQUAL : Góc bằng nhau và tỷ lệ cả 2 cạnh
    - SMALLER_AND_PREV : Góc nhỏ hơn nhưng chỉ ghép được vào cạnh trước của đỉnh
                         Vì cạnh chỉ tỉ lệ với cạnh đằng trước.
    - SMALLER_AND_NEXT : Góc nhỏ hơn nhưng chỉ ghép được vào cạnh sau của đỉnh
                            Vì cạnh chỉ tỷ lệ với cạnh đằng sau.
    - SMALLER_AND_BOTH : Góc nhỏ hơn nhưng có thể điền vào cả 2.
*/
enum STYLE_FILL {
    NOT_FILL, // Không điền vào được 
    EQUAL,
    SMALLER_AND_PREV,
    SMALLER_AND_NEXT,
    SMALLER_AND_BOTH
};

class Piece: public Frame{
public:
    Piece();
    Piece(int numVertices, int id);
    //~Piece();
    /*
     - Hàm này kiểm tra xem otherPiece có khả năng lắp vào được pieie không
        tại index của piece và indexOfOthePiece của otherPiece
    */
    STYLE_FILL isPossibleToFill(int index, Piece *otherPiece, int indexOfOtherPiece);
    /*
    - Giống hàm isPossibleToFill nhưng cho otherPiece flip đi.
    */
    STYLE_FILL isPossibleToFillFlip(int index, Piece *otherPiece, int indexOfOtherPiece);
    /*
    Xét xem 2 cạnh ứng với 2 đỉnh của 2 hình có tỷ lệ bình phương không.
    @Return True nếu tỷ lệ, ngược lại return false.
    */
    bool isRationalRatio(int, ADJACENT_EDGE, Piece *, int, ADJACENT_EDGE);
    /*
    Trả về tọa độ mới khi chuyển thử đến vị trí indexOfMainPiece của mainPiece theo 
        Style là styleFill 
    @Return false nếu chuyển không thành cong (Piece mới không có tọa độ nguyên)
    */
    bool transition(Piece *mainPiece, int indexOfMainPiece, int myIndex, STYLE_FILL styleFill
                ,Piece *result);
    /*
     Thử xem có bị overlap khi lắp hình hiện tại với hình otherPiece tại index của 
        hình hiện tại và indexOfOtherPiece của otherPiece theo style styleFill.
    */
    bool isOverlap(Piece *otherPiece);

    // center là tâm để xoay là một đỉnh của hình
    // newPiece là tọa độ mới sau khi xoay
    void rotate90ClockWise(Dot *center, Piece *newPiece);

    // chọn hình biết oldMilestone1 được move đến newMilestone1 
    // quay Piece hiện tại đến khi milestone2 có predictedMilestone bằng với newMilestone
    // Piece result chứa kết quả trả về.
    // trả về false nếu không tìm thấy transformed Piece phù hợp
    bool chooseNewTranformedPiece(int indexMilestone1, int indexMilestone2, 
                Dot *newMilestone1, Dot *newMilestone2, Piece *result);

    /*
    Clone bộ nhớ của ob vào bộ nhớ của mình.
    */
    void cloneSamePieceToMyMemory(Piece *ob);
    // Gán piece hiện tại cho thằng khác (ob)
    // nó sẽ delete piece hiện tại và tạo bộ nhớ mới.
    void assign(const Piece *ob);

    // Phải transition trước khi nối.
    void unionOtherPiece(Piece *otherPiece, Piece* &result);
    // Loại đỉnh giữa trong 3 điểm thằng hàng ra.
    void eliminate3DotInLine();
    bool transition2(Piece* mainPiece, int indexOfMainPiece, int myIndex, STYLE_FILL styleFill, Piece* &result);
    void move(Dot* a);
    void rotate90ClockWise(Dot* center);
    bool checkIsClockWise();

    /*
    + Theo thứ tự 3 đỉnh ABC (prev, đỉnh, next)
    + thuật : xBA / xCB = yBA / yCB
    */
    static bool is3DotInLine(Dot *dotA, Dot *dotB, Dot *dotC);
    static Grid *grid;
};
#endif
