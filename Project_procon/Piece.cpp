#include "Piece.h"
#include "helperPiece.h"
#include <cassert>
#include <vector>

using std::vector;

#define EPSILON 1e-2

Piece::Piece()
{
}

Piece::Piece(int numVertices, int id) : Frame(numVertices, id)
{
}

// DONE
/*
 Lắp được khi : 
    1 Cần góc tại đỉnh của piece <= góc ngoài của lời giải hiện tại 
    2 nếu nhỏ hơn ít nhất cạnh chung cần tỷ lệ (thương là hữu tỷ).  
        Nếu góc bằng thì 2 cạnh đều phải tỷ lệ.
    3 lắp vào -> tịnh tiến thì các tọa độ của piece mới sẽ nguyên
    4 không đè hình sau khi lắp vào
 + Sau khi check là ghép được thì phải xác định xép ghép được thì là 
    ghép theo STYLE_FILL nào.
*/

STYLE_FILL Piece::isPossibleToFill(int index, Piece *otherPiece, int indexOfOtherPiece) {

    float outerAngle = 360 - this->angles[index];
    float angleOfOtherPiece = otherPiece->angles[indexOfOtherPiece]; 
    STYLE_FILL styleFill; // xác định kiểu lắp khi lắp otherPiece vào piece hiện tại. 
                            // xác định ở bước 2 (Xét điều kiện tỷ lệ cạnh)
    //1. Không thoả mãn điều kiện 1 (về góc)
    if (angleOfOtherPiece > outerAngle) return NOT_FILL; 

    //2. Xét không thỏa mãn về điều kiện tỷ lệ cạnh.
    if (abs(outerAngle - angleOfOtherPiece) <= EPSILON) { // trường hợp góc bằng. thì 2 cạnh đều tỷ lệ

        // Đây là trường hợp không flip nên xét tỷ lệ cạnh phải tương ứng 
        // NEXT_EDGE vs NEXT_EDGE Và PREV_EDGE vs PREV_EDGE
        if (!this->isRationalRatio(index, NEXT_EDGE, otherPiece, indexOfOtherPiece, NEXT_EDGE)
            || !this->isRationalRatio(index, PREV_EDGE, otherPiece, indexOfOtherPiece, PREV_EDGE))
            return NOT_FILL;
        else styleFill = EQUAL;
    }
    else { //Trường hợp góc nhỏ hơn. Thì chỉ cần một cạnh tỷ lệ là được

        //Nếu cả 2 đều không tỷ lệ thì không lắp được.
        bool ratioWithNextEdge = this->isRationalRatio(index, NEXT_EDGE, otherPiece, 
                                        indexOfOtherPiece, NEXT_EDGE);
        bool ratioWithPrevEdge = this->isRationalRatio(index, PREV_EDGE, otherPiece, 
                                        indexOfOtherPiece, PREV_EDGE);
        if (!ratioWithNextEdge
        && !ratioWithPrevEdge)
        return NOT_FILL;
        // Có thể lắp vào cả 2 phía tại một định
        if (ratioWithNextEdge && ratioWithPrevEdge) styleFill = SMALLER_AND_BOTH;  
        else if (ratioWithNextEdge) styleFill = SMALLER_AND_NEXT;// chỉ lắp được cạnh kế tiếp
        else styleFill = SMALLER_AND_PREV; // chỉ lắp được ở cạnh trước
    }

    //3.Xét xem lắp vào thì tọa độ có nguyên không
    // -> Cần lấy được tọa độ sau khi chuyển để phục vụ cho điều kiện 4.
    // + Riêng trường hợp styleFill = SMALLER_AND_BOTH thì phải xét cả 2 cách transition. 
    //      Nếu 1 trong 2 cách không được thì phải thay đổi styleFill 
    Piece *newCoordOfOtherPiece = new Piece(*otherPiece);
    Piece *newCoordOfOtherPieceForBoth = new Piece(*otherPiece);
    bool isDifferentBoth = false;
    bool isChoosedOkForPrevOfBoth = false;
    bool isChoosedOkForNextOfBoth = false;

    if (styleFill != SMALLER_AND_BOTH) {
        isDifferentBoth = true;
        bool isChoosedOk = otherPiece->transition(this, index, indexOfOtherPiece, 
                                                                styleFill, newCoordOfOtherPiece);
        if (!isChoosedOk) {
            delete newCoordOfOtherPiece;
            delete newCoordOfOtherPieceForBoth;
            return NOT_FILL;//  Không có tọa độ nguyên khi chuyển.
        }
        
    }
    else {
        // thử 2 trường hợp
        isChoosedOkForPrevOfBoth = otherPiece->transition(this, index, indexOfOtherPiece,
                        SMALLER_AND_PREV, newCoordOfOtherPiece);
        isChoosedOkForNextOfBoth = otherPiece->transition(this, index, indexOfOtherPiece,
                        SMALLER_AND_PREV, newCoordOfOtherPieceForBoth);
        
        if (!isChoosedOkForNextOfBoth && !isChoosedOkForPrevOfBoth) {
            delete newCoordOfOtherPiece;
            delete newCoordOfOtherPieceForBoth;
            return NOT_FILL;//  Không có tọa độ nguyên khi chuyển.
        }
        if (isChoosedOkForNextOfBoth && !isChoosedOkForPrevOfBoth) { 
            // chỉ ghép được vào bên NEXT 
            styleFill = SMALLER_AND_NEXT;
        }
        
        if (isChoosedOkForPrevOfBoth && !isChoosedOkForNextOfBoth) { 
            // Lúc đầu đoán là BOTH nhưng giờ chỉ điền zô được PREV
            styleFill = SMALLER_AND_PREV;
        }
    }
    //4. Xét xem có bị đè hình khi lắp vào không
    // Qua hàm isOverLap cũng có thể thay đổi được styleFill từ Both thành SMALLER_AND_...
    //       Hoặc từ không phải NOT_FILL thành NOT_FILL;
    if (isDifferentBoth) {
        // styleFill lúc đầu khác với SMALLER_BOTH
        if (this->isOverlap(index, newCoordOfOtherPiece, indexOfOtherPiece, styleFill)) {
            delete newCoordOfOtherPiece;
            delete newCoordOfOtherPieceForBoth;
            return NOT_FILL;
        }
    }
    else { // Xét trường hợp overlap cho Both

        if (styleFill == SMALLER_AND_NEXT || styleFill == SMALLER_AND_PREV ) {
            if(this->isOverlap(index, newCoordOfOtherPiece, indexOfOtherPiece, styleFill)) {
                delete newCoordOfOtherPiece;
                delete newCoordOfOtherPieceForBoth;
                return NOT_FILL;
            }
        }
        else { // BOTH
            bool isOverlapPrev = this->isOverlap(index, newCoordOfOtherPiece, indexOfOtherPiece, 
                                            SMALLER_AND_PREV);
            bool isOverlapNext = this->isOverlap(index, newCoordOfOtherPiece, indexOfOtherPiece, 
                                            SMALLER_AND_NEXT);
            if (isOverlapPrev && isOverlapNext) {
                delete newCoordOfOtherPiece;
                delete newCoordOfOtherPieceForBoth;
                return NOT_FILL;
            }
            
            if (isOverlapPrev && !isOverlapNext) {
                styleFill = SMALLER_AND_NEXT;
            }

            if (!isOverlapPrev && isOverlapNext) {
                styleFill = SMALLER_AND_PREV;
            }

        }
    }

    
    // Sau khi biết là lắp được vào rồi thì xem xem lắp vào theo STYLE_FILL nào để trả về.
    delete newCoordOfOtherPiece;
    delete newCoordOfOtherPieceForBoth;
    return styleFill;
}

STYLE_FILL Piece::isPossibleToFillFlip(int index, Piece *otherPiece, int indexOfOtherPiece) {

    return NOT_FILL;
}

// Done.
/*
    + Xét tại 2 đỉnh index của Piece this với styleOfthis và đỉnh indexOfOtherPiece 
        của otherPiece với style cạnh là styleOfOther
*/
bool Piece::isRationalRatio(int index, ADJACENT_EDGE styleOfThis, 
                        Piece *otherPiece, int indexOfOtherPiece, ADJACENT_EDGE styleOfOther) {

    int myEdge; // cạnh cần xét của Piece hiện tại.
    int otherEdge; // cạnh cần xét của otherPiece.

    switch (styleOfThis) {
        case NEXT_EDGE: myEdge = this->squareEdge[index]; break;
        case PREV_EDGE: 
            myEdge = this->squareEdge[(index - 1 + this->numVertices) % this->numVertices]; break;
    }

    switch (styleOfOther) {
        case NEXT_EDGE: otherEdge = otherPiece->squareEdge[indexOfOtherPiece]; break;
        case PREV_EDGE: 
            myEdge = otherPiece->squareEdge[(index - 1 + otherPiece->numVertices) % otherPiece->numVertices]; break;
    }

    int ucln = UCLN(myEdge, otherEdge);

    for (int i = 1; i <= ucln; i++) {
        
        if (myEdge % i == 0 && otherEdge % i == 0) {
        
            int tmp1 = myEdge / i;
            int tmp2 = otherEdge / i;

            if (isSquareNumber(tmp1) && isSquareNumber(tmp2)) return true;
        }
    }

    return false;
}

// Transform hình.
/*
+ Lắp mảnh đang xét vào mảnh mainPiece tại vị trí indexOfMainPiece của mainPiece và 
    myIndex của đỉnh đang muốn chuyến, theo StyleFill là styleFill.
+ Cần 2 đỉnh làm mốc (1 đỉnh là đỉnh chung, đỉnh kia do tính chất cạnh chung tạo nên)
+ 2 đỉnh kia do xoay hình và có 2 đỉnh mốc trùng với 2 đỉnh mốc trên thì lấy
+ Khi đã biết 2 đỉnh mới thì lấy hình đã xoay sang trạng thái mới đó.
+ Khi không transition được thì thay giá trị styleFill thành NOT_FILL

+ Sau khi tìm được 2 mốc thì move hình đến đỉnh chung. Rồi quay thử 90 độ 4 lần
    chọn hình có 2 mốc trùng với mốc ở trên
*/
// Result chứa kết quả transformation của Piece nên nó cần phải được cấp phát từ trước.
bool Piece::transition(Piece *mainPiece, int indexOfMainPiece, int myIndex, STYLE_FILL styleFill
                        ,Piece *result) {

    //Piece *newPiece = new Piece(this->numVertices, this->id); // tạo ra Piece mới để trả về.
    assert(result != NULL);
    assert(styleFill != NOT_FILL);
    // Cần tìm 2 milestones để chuyển được cả Piece về vị trí mới.
    Dot *myOldMilestone1 = this->vertices[myIndex];
    Dot *mainMilestone1 = mainPiece->vertices[indexOfMainPiece];
    Dot *myOldMilestone2 = NULL;
    Dot *mainMilestone2 = NULL;
    
    Dot *myNewMilestone1 = new Dot(mainMilestone1->x, mainMilestone1->y);
    Dot *myNewMilestone2 = new Dot();

    switch(styleFill) {
        case EQUAL:
        case SMALLER_AND_PREV:
            myOldMilestone2 = this->vertices[(myIndex - 1 + this->numVertices)%this->numVertices];
            int tmp = (indexOfMainPiece - 1 + mainPiece->numVertices)%mainPiece->numVertices;
            mainMilestone2 = mainPiece->vertices[tmp];
        break;
        case SMALLER_AND_NEXT:
            myOldMilestone2 = this->vertices[(myIndex + 1)%this->numVertices];
            int tmp = (indexOfMainPiece + 1)%mainPiece->numVertices;
            mainMilestone2 = mainPiece->vertices[tmp];
        break;
    }

    // Lấy được 2 mốc cũ của mỗi Piece
    // Bây giờ chuyển 2 mốc của piece tọa độ về tọa độ mới. (một cái tính dễ vì trùng đỉnh)
    vector<Dot*> vectDotA;
    vectDotA.push_back(myOldMilestone1); vectDotA.push_back(myNewMilestone1);
    vector<Dot*> vectDotB;
    vectDotB.push_back(myOldMilestone2); vectDotB.push_back(myNewMilestone2);
    int fitPoint = fitPointToEdge(vectDotA, vectDotB, mainMilestone1, mainMilestone2);
    
    if (fitPoint < 0) { // Không có tọa độ nguyên cho milestone2
        cout << "Vo Ly FitPoint."; // Vì ở trước đó đã xét điều kiện về cạnh rồi
        delete myNewMilestone1;
        delete myNewMilestone2;
        return false;
    }
    
    // đã tìm được 2 mốc.
    int indexMilestone2;

    switch(styleFill) {
        case EQUAL:
        case SMALLER_AND_PREV:
            indexMilestone2 = (myIndex - 1 + this->numVertices)%this->numVertices;
        break;
        case SMALLER_AND_NEXT:
            indexMilestone2 = (myIndex + 1)%this->numVertices;
        break;
    }
    bool chooseOk = chooseNewTranformedPiece(myIndex, indexMilestone2, 
                myNewMilestone1, myNewMilestone2, result);

    delete myNewMilestone1;
    delete myNewMilestone2;
    return chooseOk;
}

// TO-DO
// True nếu bị overlap, false nếu không bị overlap.
bool Piece::isOverlap(int index, Piece *otherPiece, int indexOfOtherPiece, STYLE_FILL styleFill) {

    assert(styleFill != NOT_FILL);
}

/*
+ Xoay quanh center, center thuộc Piece đó. Vì vậy nên move trước rồi mời rotate.
+ newPiece đã được cấp phát ở ngoài.
*/
void Piece::rotate90ClockWise(Dot *center, Piece *newPiece) {

    assert(center != NULL & newPiece != NULL);

    // Move center về (0, 0) sau đó xoay rồi lại move (0, 0) về center.
    // công thức : x = y và y = -x;
    for (int i = 0; i < newPiece->numVertices; i++) {
        newPiece->vertices[i]->x = this->vertices[i]->y - center->y + center->y;
        newPiece->vertices[i]->y = -(this->vertices[i]->x - center->x) + center->x ;
    }
}

/*
Thuật toán là : cứ xoay Piece hiện tại 4 lần (dùng rotate90ClockWise) đến khi gặp 
    indexMilestone2 bằng với newMileStone
*/
bool Piece::chooseNewTranformedPiece(int indexMilestone1, int indexMilestone2, 
    Dot *newMilestone1, Dot *newMilestone2, Piece *result) {

    assert(result != NULL);
    // move hình từ vị trí ban đầu indexMilestone1 đến vị trí newMilestone1.
    Piece *tmpResult = new Piece(*this);
    // move tmpResult 
    int deltaX = newMilestone1->x - this->vertices[indexMilestone1]->x;
    int deltaY = newMilestone1->x - this->vertices[indexMilestone1]->y;
    
    for (int i = 0; i < tmpResult->numVertices; i++) {

        tmpResult->vertices[i]->x += deltaX;
        tmpResult->vertices[i]->y += deltaY;
    }

    // xoay hình xem hình xoay nào sẽ phù hợp
    Piece *newRotatedPiece = new Piece(*tmpResult);
    for (int i = 0; i < 4; i++) { // số lần quay tối đá là 4

        tmpResult->rotate90ClockWise(tmpResult->vertices[indexMilestone1], newRotatedPiece);
        if (newRotatedPiece->vertices[indexMilestone2]->x == newMilestone2->x
            && newRotatedPiece->vertices[indexMilestone2]->y == newMilestone2->y) {

                result->cloneSamePieceToMyMemory(newRotatedPiece);
                delete tmpResult;
                delete newRotatedPiece;
                return true;
            }
        
        tmpResult->cloneSamePieceToMyMemory(newRotatedPiece);
    }

    delete tmpResult;
    delete newRotatedPiece;
    return false; // xoay 4 lần mà chẳng có hình nào phù hợp
}

// Điều kiện cho clone là có cùng số đỉnh
void Piece::cloneSamePieceToMyMemory(Piece *ob) {

    assert(this->numVertices > 0);
    assert(this->numVertices == ob->numVertices);

    this->id = ob->id;
    for (int i = 0; i < this->numVertices; i++) {

        this->vertices[i]->x = ob->vertices[i]->x;
        this->vertices[i]->y = ob->vertices[i]->y;

        this->angles[i] = ob->angles[i];

        this->squareEdge[i] = ob->squareEdge[i];
    }
}