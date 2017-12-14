#include "Piece.h"
#include "helperPiece.h"
#include <cassert>
#include <vector>

using std::vector;
using std::abs;

#include <iostream>

using std::cout;
using std::endl;

#define EPSILON 1e-2


Grid *Piece::grid = new Grid(70, 110, 10);

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
        if (!this->isRationalRatio(index, NEXT_EDGE, otherPiece, indexOfOtherPiece, PREV_EDGE)
            || !this->isRationalRatio(index, PREV_EDGE, otherPiece, indexOfOtherPiece, NEXT_EDGE))
            return NOT_FILL;
        else styleFill = EQUAL;
    }
    else { //Trường hợp góc nhỏ hơn. Thì chỉ cần một cạnh tỷ lệ là được

        //Nếu cả 2 đều không tỷ lệ thì không lắp được.
        bool ratioWithNextEdge = this->isRationalRatio(index, NEXT_EDGE, otherPiece, 
                                        indexOfOtherPiece, PREV_EDGE);
        bool ratioWithPrevEdge = this->isRationalRatio(index, PREV_EDGE, otherPiece, 
                                        indexOfOtherPiece, NEXT_EDGE);
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
        // cout << "Smaller_and_both " << endl;
        isDifferentBoth = true;
        bool isChoosedOk = otherPiece->transition(this, index, indexOfOtherPiece, 
                                                                styleFill, newCoordOfOtherPiece);
        // cout << styleFill << endl;
        // vector<Frame*> vect; vect.push_back(this); vect.push_back(newCoordOfOtherPiece);
        // Piece::grid->showPieceSameTime(vect);
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
                        SMALLER_AND_NEXT, newCoordOfOtherPieceForBoth);
        
        if (!isChoosedOkForNextOfBoth && !isChoosedOkForPrevOfBoth) { //cả 2 đều không lắp được.
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
        if (this->isOverlap(newCoordOfOtherPiece)) {
            delete newCoordOfOtherPiece;
            delete newCoordOfOtherPieceForBoth;
            return NOT_FILL;
        }
    }
    else { // Xét trường hợp overlap cho Both

        if (styleFill == SMALLER_AND_NEXT  // đã bị thay đổi khi đi qua bước 3.
                || styleFill == SMALLER_AND_PREV ) {
            if(this->isOverlap(newCoordOfOtherPiece)) {
                delete newCoordOfOtherPiece;
                delete newCoordOfOtherPieceForBoth;
                return NOT_FILL;
            }
        }
        else { // BOTH
            bool isOverlapPrev = this->isOverlap(newCoordOfOtherPiece);
            bool isOverlapNext = this->isOverlap(newCoordOfOtherPieceForBoth);

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

// To-do
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
    int tempIndex = index;
    int tempOtherIndex = indexOfOtherPiece;

    switch (styleOfThis) {
        case NEXT_EDGE: myEdge = this->squareEdge[index]; break;
        case PREV_EDGE: 
            tempIndex = (index - 1 + this->numVertices) % this->numVertices;
            myEdge = this->squareEdge[(index - 1 + this->numVertices) % this->numVertices]; 
            break;
    }

    switch (styleOfOther) {
        case NEXT_EDGE: otherEdge = otherPiece->squareEdge[indexOfOtherPiece]; break;
        case PREV_EDGE: 
            tempOtherIndex = (indexOfOtherPiece - 1 + otherPiece->numVertices) % otherPiece->numVertices;
            otherEdge = otherPiece->squareEdge[(indexOfOtherPiece - 1 + otherPiece->numVertices) % otherPiece->numVertices]; 
            break;
    }

    int ucln = UCLN(myEdge, otherEdge);
    // cout << "Me : "; this->showEdge(); cout << *this;
    // cout << "Other: "; otherPiece->showEdge(); cout << *otherPiece;
    // cout << "Index : " << tempIndex  << " in " << this->numVertices
    //      << ". " << "OtherIndex: " << tempOtherIndex << " in " << otherPiece->numVertices << endl;
    // cout << "MyEdge : " << myEdge << ". " << "OtherEdge : " << otherEdge << endl;

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
    assert(indexOfMainPiece >= 0 && myIndex >= 0);
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
        {
            myOldMilestone2 = this->vertices[(myIndex + 1)%this->numVertices];
            int tmp1 = (indexOfMainPiece - 1 + mainPiece->numVertices)%mainPiece->numVertices;
            mainMilestone2 = mainPiece->vertices[tmp1];
            // int tmp = (indexOfMainPiece + 1)%mainPiece->numVertices;
            // mainMilestone2 = mainPiece->vertices[tmp];
        }
        break;
        case SMALLER_AND_NEXT:
        {        
            myOldMilestone2 = this->vertices[(myIndex - 1 + this->numVertices)%this->numVertices];
            int tmp = (indexOfMainPiece + 1)%mainPiece->numVertices;
            mainMilestone2 = mainPiece->vertices[tmp];
            // int tmp1 = (indexOfMainPiece - 1 + mainPiece->numVertices)%mainPiece->numVertices;
            // mainMilestone2 = mainPiece->vertices[tmp1];
        }
        break;
    }
    // Lấy được 2 mốc cũ của mỗi Piece
    // Bây giờ chuyển 2 mốc của piece tọa độ về tọa độ mới. (một cái tính dễ vì trùng đỉnh)
    vector<Dot*> vectDotA;
    vectDotA.push_back(myOldMilestone1); vectDotA.push_back(myNewMilestone1); 
    vector<Dot*> vectDotB;
    vectDotB.push_back(myOldMilestone2); vectDotB.push_back(myNewMilestone2);
    // cout << "MAin Piece " << *mainPiece;
    // cout << "Old stone 1 " << myOldMilestone1->x << "; " << myOldMilestone1->y << endl;
    // cout << "Old stone 2 " << myOldMilestone2->x << "; " << myOldMilestone2->y << endl;
    // cout << "Main Milestone 1 " << mainMilestone1->x << "; " << mainMilestone1->y << endl;
    // cout << "Main Milestone 2 " << mainMilestone2->x << "; " << mainMilestone2->y << endl;
    int fitPoint = fitPointToEdge(vectDotA, vectDotB, mainMilestone1, mainMilestone2);
    // cout << "VCL " << fitPoint << endl;
    // cout << "NEw stone 1 " << myNewMilestone1->x << "; " << myNewMilestone1->y << endl;
    // cout << "NEw stone 2 " << myNewMilestone2->x << "; " << myNewMilestone2->y << endl;
    // if (fitPoint < 0) { // Không có tọa độ nguyên cho milestone2
    //     //cout << "Vo Ly FitPoint."; // Vì ở trước đó đã xét điều kiện về cạnh rồi
    //     delete myNewMilestone1;
    //     delete myNewMilestone2;
    //     return false;
    // }
    
    // đã tìm được 2 mốc.
    int indexMilestone2;

    //cout << "OOOOO" << endl;
    switch(styleFill) {
        case EQUAL:
        case SMALLER_AND_PREV:
            indexMilestone2 = (myIndex + 1)%this->numVertices;
        break;
        case SMALLER_AND_NEXT:
            indexMilestone2 = (myIndex - 1 + this->numVertices)%this->numVertices;
        break;
    }
    bool chooseOk = chooseNewTranformedPiece(myIndex, indexMilestone2, 
                myNewMilestone1, myNewMilestone2, result);
    
    delete myNewMilestone1;
    delete myNewMilestone2;
    return chooseOk;
}

#include <boost/assign/std/vector.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/dsv/write.hpp>

#include <boost/foreach.hpp>

using namespace boost::geometry;
using namespace boost::assign;
using boost::geometry::model::polygon;

#include <deque>
// DONE
// True nếu bị overlap, false nếu không bị overlap.
// Xét Piece gọi đến hàm và otherPiece.
bool Piece::isOverlap(Piece *otherPiece) {

    typedef boost::geometry::model::d2::point_xy<int> point_xy;
    vector<point_xy> pointsThis, pointsOther; // Tạo 2 vector points ứng với 2 Piece.

    // tạo polygon 1 từ piece this.
    for (int i = 0; i < this->numVertices; i++) {
        pointsThis += point_xy(this->vertices[i]->x, this->vertices[i]->y);
    }
    pointsThis += point_xy(this->vertices[0]->x, this->vertices[0]->y);
    boost::geometry::model::polygon<point_xy> polygonThis;
    boost::geometry::assign_points(polygonThis, pointsThis);
    float area1 = boost::geometry::area(polygonThis);
    boost::geometry::correct(polygonThis);
    //cout << "AreaThis " <<  boost::geometry::area(polygonThis) << std::endl;

    // tạo polygon 2 từ piece otherPiece 
    for (int i = 0; i < otherPiece->numVertices; i++) {

        pointsOther += point_xy(otherPiece->vertices[i]->x, otherPiece->vertices[i]->y);
    }
    pointsOther += point_xy(otherPiece->vertices[0]->x, otherPiece->vertices[0]->y);
    boost::geometry::model::polygon<point_xy> polygonOther;
    boost::geometry::assign_points(polygonOther, pointsOther);
    float area2 = boost::geometry::area(polygonOther);
    boost::geometry::correct(polygonOther);
    
    if (!boost::geometry::overlaps(polygonThis, polygonOther)
        && (!boost::geometry::within(polygonThis, polygonOther)
            && !boost::geometry::within(polygonOther, polygonThis) )) return false;
    return true;
}   

bool Piece::checkIsClockWise() {

    typedef boost::geometry::model::d2::point_xy<int> point_xy;
    vector<point_xy> pointsThis, pointsOther; // Tạo 2 vector points ứng với 2 Piece.

    // tạo polygon 1 từ piece this.
    for (int i = 0; i < this->numVertices; i++) {
        pointsThis += point_xy(this->vertices[i]->x, this->vertices[i]->y);
    }
    pointsThis += point_xy(this->vertices[0]->x, this->vertices[0]->y);
    boost::geometry::model::polygon<point_xy> polygonThis;
    boost::geometry::assign_points(polygonThis, pointsThis);
    float area1 = boost::geometry::area(polygonThis);
    if (area1 < 0) return false;
    return true;
}

/*
+ Xoay quanh center, center thuộc Piece đó. Vì vậy nên move trước rồi mời rotate.
+ newPiece đã được cấp phát ở ngoài.
*/
// void Piece::rotate90ClockWise(Dot *center, Piece *newPiece) { // Test Oke.

//     assert(center != NULL & newPiece != NULL);
//     assert(newPiece->numVertices == this->numVertices);

//     // Move center về (0, 0) sau đó xoay rồi lại move (0, 0) về center.
//     // công thức : x = y và y = -x;
//     for (int i = 0; i < newPiece->numVertices; i++) {
//         newPiece->vertices[i]->x = this->vertices[i]->y - center->y + center->x;
//         newPiece->vertices[i]->y = -(this->vertices[i]->x - center->x) + center->y;
//     }
// }

bool Piece::transition2(Piece* mainPiece, int indexOfMainPiece, int myIndex, STYLE_FILL styleFill, Piece* &result){

    Dot* moveVec = new Dot(mainPiece->vertices[indexOfMainPiece]->x - this->vertices[myIndex]->x, mainPiece->vertices[indexOfMainPiece]->y - this->vertices[myIndex]->y);
 
    this->move(moveVec);
    //this->show();

    //cout<<"main piece"<<endl;
   // mainPiece->show();

    //cout<<"transition testing ............... 3"<<endl;

    int preMainIndex =  indexOfMainPiece - 1;
    int nextMainIndex = indexOfMainPiece + 1;
    int preIndex = myIndex - 1;
    int nextIndex = myIndex + 1;

    if(indexOfMainPiece == 0){
        preMainIndex = mainPiece->numVertices - 1;
    }

    if(indexOfMainPiece == mainPiece->numVertices - 1){
        nextMainIndex = 0;
    }

    if(myIndex == 0){
        preIndex = this->numVertices - 1;
    }

    if(myIndex == this->numVertices - 1){
        nextIndex = 0;
    }

    int i ;
    for(i = 0; i < 4; i ++){
        this->rotate90ClockWise(this->vertices[myIndex]);
        //cout<<"rotate "<<endl;
        //this->show();
        if(styleFill == SMALLER_AND_PREV && 
            is3DotInLine(mainPiece->vertices[indexOfMainPiece], mainPiece->vertices[preMainIndex], this->vertices[nextIndex])&&
            !this->isOverlap(mainPiece))
        {
            break;
        }

        if(styleFill == SMALLER_AND_NEXT &&
            is3DotInLine(mainPiece->vertices[indexOfMainPiece],mainPiece->vertices[nextMainIndex], this->vertices[preIndex]) &&
            !this->isOverlap(mainPiece))
        {
            break;
        }

        if(styleFill == EQUAL &&
            is3DotInLine(mainPiece->vertices[indexOfMainPiece], mainPiece->vertices[preMainIndex], this->vertices[nextIndex])&&
            is3DotInLine(mainPiece->vertices[indexOfMainPiece],mainPiece->vertices[nextMainIndex], this->vertices[preIndex]) &&
            !this->isOverlap(mainPiece))
        {
          //  cout<<"EQULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"<<endl;
            break;
        }

        //this->rotate90ClockWise(this->vertices[myIndex]);


    }

    if(i == 4){
        return false;
    }

    this->unionOtherPiece(mainPiece, result);
    // cout << "Inside " << *result << endl;
    //cout<<"after result  numVertices = "<<result->numVertices<<endl;
    return true;
}

// tinh tien vector a
void Piece::move(Dot* a){
    assert(a != NULL);
    for(int i = 0; i < this->numVertices; i ++){
        this->vertices[i]->x += a->x;
        this->vertices[i]->y += a->y;
    }
}

void Piece::rotate90ClockWise(Dot *center, Piece *newPiece) { // Test Oke.

    assert(center != NULL & newPiece != NULL);
    assert(newPiece->numVertices == this->numVertices);

    // Move center về (0, 0) sau đó xoay rồi lại move (0, 0) về center.
    // công thức : x = y và y = -x;
    for (int i = 0; i < newPiece->numVertices; i++) {
        int temp = newPiece->vertices[i]->x ;
        newPiece->vertices[i]->x = this->vertices[i]->y - center->y + center->x;
        newPiece->vertices[i]->y = -(temp - center->x) + center->y;
    }
}

void Piece::rotate90ClockWise(Dot* center){
    assert(center !=NULL);

    for(int i = 0; i < this->numVertices; i ++){
        int temp = this->vertices[i]->x;
        this->vertices[i]->x = this->vertices[i]->y - center->y + center->x;
        this->vertices[i]->y = - temp + center->y + center->x;
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
    int deltaY = newMilestone1->y - this->vertices[indexMilestone1]->y;
    
    for (int i = 0; i < tmpResult->numVertices; i++) {

        tmpResult->vertices[i]->x += deltaX;
        tmpResult->vertices[i]->y += deltaY;
    }
    
    // xoay hình xem hình xoay nào sẽ phù hợp
    Piece *newRotatedPiece = new Piece(*tmpResult);
    for (int i = 0; i < 4; i++) { // số lần quay tối đá là 4
        // Piece::grid->showPiece(newRotatedPiece);
        tmpResult->rotate90ClockWise(tmpResult->vertices[indexMilestone1], newRotatedPiece);
        // cout << "Xoay lan " << i << " : "  << *tmpResult;
        // cout << *newRotatedPiece << endl;
        if (newRotatedPiece->vertices[indexMilestone2]->x == newMilestone2->x
            && newRotatedPiece->vertices[indexMilestone2]->y == newMilestone2->y) {

                result->cloneSamePieceToMyMemory(newRotatedPiece);
                // cout << "Le ra phai chon : " << *newRotatedPiece;
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
void Piece::cloneSamePieceToMyMemory(Piece *ob) { // Test oke

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

void Piece::assign(const Piece *ob) {

    if (this->numVertices != 0) {  
        delete[] angles;
        delete[] squareEdge;
        for (int i = 0; i < this->numVertices; i++) {
            delete vertices[i];
        }
        delete[] vertices;
    }

    this->numVertices = ob->numVertices;
    this->id = ob->id; 
    this->angles = new float[this->numVertices];
    this->squareEdge = new int[this->numVertices];
    this->vertices = new Dot*[this->numVertices];

    for (int i = 0; i < this->numVertices; i++) {

        this->vertices[i] = new Dot(ob->vertices[i]->x, ob->vertices[i]->y);
        this->angles[i] = ob->angles[i];
        this->squareEdge[i] = ob->squareEdge[i];
    }
}
/*
   + thuật : xBA / xCB = yBA / yCB
   + hay xBA * yCB == yBA * xCB
*/
bool Piece::is3DotInLine(Dot *dotA, Dot* dotB, Dot *dotC) {
    assert(dotA != NULL && dotB != NULL && dotC != NULL);
    int xBA = dotB->x - dotA->x;
    int yBA = dotB->y - dotA->y;
    
    int xCB = dotC->x - dotB->x;
    int yCB = dotC->y - dotB->y;
    
    if (xBA * yCB == yBA * xCB) return true;
    return false;
}

/*
    + HÀm này loại những đỉnh mà 3 đỉnh trước, nó và sau tạo thành 1 đường thẳng
    + sau hàm này Piece có đỉnh khác, góc khác và squareEdge khác.
*/
void Piece::eliminate3DotInLine() {

    int newNumVertices = this->numVertices;
    
    for (int index = 0; index < this->numVertices; index++) {

        Dot *dotA = this->vertices[(index-1+this->numVertices)%this->numVertices];
        Dot *dotB = this->vertices[index];
        Dot *dotC = this->vertices[(index + 1)%this->numVertices];

        if (dotA != NULL && dotB != NULL && dotC != NULL && 
            Piece::is3DotInLine(dotA, dotB, dotC) ) {
                
            delete this->vertices[index];
            this->vertices[index] = NULL;
            newNumVertices--;
        }
    
    }
    
    Dot **newDot = new Dot*[newNumVertices];
    
    int i = 0;

    for (int index = 0; index < this->numVertices; index++) {

        if (this->vertices[index] != NULL) {
            
            newDot[i] = new Dot(this->vertices[index]->x, this->vertices[index]->y);
            i++;
        }
    }
    
    for (int i = 0; i < this->numVertices; i++) {

        if (this->vertices[i] != NULL) delete this->vertices[i];
    }
    delete[] this->vertices;

    this->numVertices = newNumVertices;
    this->vertices = new Dot*[newNumVertices];
    
    for (int i = 0; i < this->numVertices; i++) {
        this->vertices[i] = new Dot(newDot[i]->x, newDot[i]->y);
    }

    // release newDot 
    for (int i = 0; i < newNumVertices; i++)  {
        delete newDot[i];
    }
    delete[] newDot;

    this->calcAngle();
    Frame::calcSquareEdge(this);
}


#include <boost/foreach.hpp>

void Piece::unionOtherPiece(Piece *otherPiece, Piece* &result) {

    // Grid grid(100, 100, 10);
    // vector<Frame*> vect; vect.push_back(this); vect.push_back(otherPiece);
    // grid.showPieceSameTime(vect);

    typedef boost::geometry::model::d2::point_xy<int> point_xy;
    vector<point_xy> pointsThis, pointsOther; // Tạo 2 vector points ứng với 2 Piece.

    // tạo polygon 1 từ piece this.
    for (int i = 0; i < this->numVertices; i++) {
        pointsThis += point_xy(this->vertices[i]->x, this->vertices[i]->y);
    }
    pointsThis += point_xy(this->vertices[0]->x, this->vertices[0]->y);
    boost::geometry::model::polygon<point_xy> polygonThis;
    boost::geometry::assign_points(polygonThis, pointsThis);
    boost::geometry::correct(polygonThis);

    // tạo polygon 2 từ piece otherPiece 
    for (int i = 0; i < otherPiece->getNumVertices(); i++) {

        pointsOther += point_xy(otherPiece->getVertices()[i]->x, otherPiece->getVertices()[i]->y);
    }
    pointsOther += point_xy(otherPiece->getVertices()[0]->x, otherPiece->getVertices()[0]->y);
    boost::geometry::model::polygon<point_xy> polygonOther;
    boost::geometry::assign_points(polygonOther, pointsOther);
    boost::geometry::correct(polygonOther);
    
    std::vector<polygon<point_xy> > output;
    boost::geometry::union_(polygonThis, polygonOther, output);

    // int i = 0;
    // //std::cout << "green || blue:" << std::endl;
    
    
    //     int a = output[0].outer()[0].x();
    //     std::cout << a;
    //     std::cout << i++ << ": " << boost::geometry::dsv(output[0]) 
    //                 << endl << output[0].outer().size() << std::endl;
         
                          
    result = new Piece(output[0].outer().size() - 1, 0);
    
    for (int i = 0; i < output[0].outer().size() - 1; i++) {
        result->setCoord(i, output[0].outer()[i].x(), output[0].outer()[i].y());
    }
    result->calcAngle();
    result->calcSquareEdge();
    result->eliminate3DotInLine();
    
    //for ()
}

void Piece::moveIfOutMap() {

    int x = vertices[0]->x;
    int y = vertices[0]->y;
    Dot *minXVertice = vertices[0];
    Dot *minYVertice = vertices[0];

    for (int i = 1; i < numVertices; i++) {

        if (x > vertices[i]->x) {
            x = vertices[i]->x;
            minXVertice = vertices[i];
        }
        if (y > vertices[i]->y) {
            y = vertices[i]->y;
            minXVertice = vertices[i];
        }
    }

    if (x <= 0) {
        Dot *tmp = new Dot(10 - x, 2);
        this->move(tmp);
        delete tmp;
    }
    if (y <= 0) {
        Dot *tmp = new Dot(2, 10 - y);
        this->move(tmp);
        delete tmp;
    }
}

void Piece::getHeightWidth(Frame *frame, int &height, int &width) {

    int minX = frame->getVertices()[0]->x;
    int maxX = frame->getVertices()[0]->x;
    int minY = frame->getVertices()[0]->y;
    int maxY = frame->getVertices()[0]->y;

    for (int i = 1; i < frame->getNumVertices(); i++) {
        if (minX > frame->getVertices()[i]->x) minX = frame->getVertices()[i]->x;
        if (maxX < frame->getVertices()[i]->x) maxX = frame->getVertices()[i]->x;
        if (minY > frame->getVertices()[i]->y) minY = frame->getVertices()[i]->y;
        if (maxY < frame->getVertices()[i]->y) maxY = frame->getVertices()[i]->y;
    }

    height = abs(maxY - minY);
    width = abs(maxX - minX);
}

bool Piece::comfortReference(Frame *frame) {

    float threshold = 20;
    int sum = 0;
    int c = 0;
    
    for (int i = 0; i < numVertices; i++) {

        if ((squareEdge[i] - threshold) < 0) return false;
        if (abs(angles[i] - 270) <= EPSILON && 
            abs(angles[(i + 1) % this->numVertices] - 270) <= EPSILON) {
                sum += squareEdge[i];
                c++;
            }
            else {
                sum = 0; c = 0;
            }
        
        //if (c >= 2 && sum < 120) return false;
    }

    int minX = vertices[0]->x;
    int maxX = vertices[0]->x;
    int minY = vertices[0]->y;
    int maxY = vertices[0]->y;

    for (int i = 1; i < numVertices; i++) {
        if (minX > vertices[i]->x) minX = vertices[i]->x;
        if (maxX < vertices[i]->x) maxX = vertices[i]->x;
        if (minY > vertices[i]->y) minY = vertices[i]->y;
        if (maxY < vertices[i]->y) maxY = vertices[i]->y;
    }

    int h, w;
    this->getHeightWidth(frame, h, w);
    if (maxY - minY > h + 5 || maxX - minX > w) return false;

    return true;
}