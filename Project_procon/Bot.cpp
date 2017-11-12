#include "Bot.h"
#include <iostream>
#include <vector>

using std::vector;

#include <cassert>

// Định nghĩa Epsilon để so sánh 2 góc bằng nhau "không thế so sánh 2 float trực tiếp."
#define EPSILON 1e-3
// Để FlIP_MODE là false nếu đề bài cho Piece KHÔNG flip
#define FLIP_MODE true

Bot::Bot(int numPieces, Frame *frame, Piece **allPieces)
{

    this->frame = frame;
    this->allPieces = allPieces;
    this->currAnswer = NULL;
    this->numPieces = numPieces;

    for (int i = 0; i < numPieces; i++) {

        currentDomainPiece.push_back(allPieces[i]);
    }
}

/*
+ Chọn piece đầu tiên cho lời giải, để làm init.
+ chọn piece có số góc ngoái < 180 (góc trong > 180) và khác 90 ít nhất 
    - nếu có nhiều piece có nhiều góc bằng nhau thì chọn piece ít đỉnh hơn.
+Nếu không tìm được Piece nào thỏa mãn thì trả về 1 piece random.
*/
Piece *Bot::chooseInitPiece()
{

    // chứa các Piece có số góc ngoài < 180 và khác 90 ít nhất
    vector<Piece *> angleConstraintPieces;
    int minSpecialAngles = 100;

    for (int i = 0; i < numPieces; i++)
    {

        int tmp = allPieces[i]->getNumSpecialAngles();

        if (minSpecialAngles > tmp)
        {

            minSpecialAngles = tmp;
            angleConstraintPieces.clear();
            angleConstraintPieces.push_back(allPieces[i]);
        }
        else if (minSpecialAngles == tmp)
        {
            angleConstraintPieces.push_back(allPieces[i]);
        }
    }

    int tmp = angleConstraintPieces.size();

    if (tmp == 0) 
        return allPieces[0];

    int index = 0;
    int minVertices = angleConstraintPieces[0]->getNumVertices();
    for (int i = 1; i < tmp; i++)
    {

        if (minVertices > angleConstraintPieces[i]->getNumVertices())
        {

            index = i;
            minVertices = angleConstraintPieces[i]->getNumVertices();
        }
    }

    return allPieces[index];
}

/*
+ Ưu tiên cái đỉnh có ít mảnh mà ghép vào nó được hơn, để cho ít nhánh .
    Trả về 1 int biểu diễn số thứ tự của góc trong currentPiece.    
+ Thứ tụ ưu tiên : có góc trong > 180 (#270) (góc càng lớn càng tốt), 270, < 180
+ Note: Trước mỗi lần chọn đỉnh thì currentPiece đã được lắp thêm.
*/
int Bot::chooseNextVariabel() {

    int max = 0;
    int index = -1; // index của góc đặc biệt (>180 và khác 270)
    int index270 = -1; // index của góc 270. Dùng khi không có góc đặc biệt.

    const float *angles = this->currAnswer->getAngles();

    for (int i = 0; i < this->currAnswer->getNumVertices(); i++) {

        if (angles[i] > 180+EPSILON && abs(angles[i]-270) > EPSILON) { // Special angle

            if (max < angles[i]) { // chọn góc đặc biệt lớn nhất.
                max = angles[i];
                index = i;
            }
        }

        if (index270 == -1 && abs(angles[i] - 270) < EPSILON) { // Góc 270

            index270 = i;
        }
    }

    // Trả về index của góc 

    if (index != -1) { // Có góc đặc biệt

        return index;
    }
    else if (index270 != -1) { // Có góc 270
        return index270;
    }
    else { // Không có góc đặc biệt & 270
        return 0;
    }
}

/*
+ Lấy hết tất cả khả năng của các hình có thể lắp vào đỉnh index của currentPieces
    Giá trị là list các value có ưu tiên . Điều kiện:
    - Cần góc tại đỉnh của piece <= góc ngoài của lời giải hiện tại 
    - ít nhất cạnh chung cần tỷ lệ (thương là hữu tỷ)
    - lắp vào -> tịnh tiến thì các tọa độ của piece mới sẽ nguyên
    - không đè hình sau khi lắp vào
+ Ưu tiên : 
    - khít góc (đều chung 2 cạnh) +3
        - cạnh chung bằng nhau +1
        - cạnh không bằng nhau +0
    - chỉ chung 1 cạnh +1
        - cạnh chung bằng nhau +1
        - cạnh chung không bằng nhau.+0
*/
vector<ValueIndex*> Bot::getListValues(int index) {

    int sizeOfDomain = currentDomainPiece.size();
    assert(sizeOfDomain > 0);

    vector<ValueIndex*> listValues;
    vector<ValueIndex*> listNotPriValues; // list value không có ưu tiên

    // Lấy thông tin của lời giải hiện tại
    float *anglesOfCurrAnswer = this->currAnswer->getAngles();
    Dot **verticesOfCurrAnswer = this->currAnswer->getVertices();

    for (int i = 0; i < sizeOfDomain; i++) { // Tìm trong tất cả các Piece chưa ghép

        // lấy thông tin của piece đang xét.
        Piece *currPiece = currentDomainPiece[i];
        float *anglesOfCurrPiece = currPiece->getAngles();
        Dot **verticesOfCurrPiece = currPiece->getVertices();

        for (int j = 0; j < currPiece->getNumVertices(); j++) { // Tìm trong tất cả các đỉnh của Piece

            if (this->currAnswer->isPossibleToFill(index, currPiece, j)) { 
                // Có thể lắp được ở cạnh LEFT hoặc RIGHT
                // -> Xem 2 trường hợp cho góc < góc Frame. Để update cho styleShared
                // -> Có thể tạo 2 khả năng
                // Nếu lắp được tại index (không flip)

                // styleShared và score để tính riêng.
                ValueIndex *value = new ValueIndex(currPiece, j, false);
                this->updateStyleSharedAndScore(index, value);
                this->pushPriorityQueue(listValues, value);
            }

            if (this->currAnswer->isPossibleToFillFlip(index, currPiece, j) && FLIP_MODE) { 
                // Nếu lắp được tại index mà có flip

                ValueIndex *value = new ValueIndex(currPiece, j, true);
                this->updateStyleSharedAndScore(index, value);
                this->pushPriorityQueue(listValues, value);
            }
        }
    }

    return listValues;
}

/*
+ Thêm 1 phần tử vào priority Queue
    Phần tử nào có score lớn hơn thì nổi lên đầu.
*/
void pushPriorityQueue(vector<ValueIndex*> &listValues, ValueIndex *value) {

    assert(value != NULL);
    listValues.push_back(value);

    int index = listValues.size() - 1;

    while (index > 0 && listValues[index]->score < listValues[index - 1]->score) {

        ValueIndex *tmp = listValues[index];
        listValues[index] = listValues[index - 1];
        listValues[index - 1] = tmp;
        index--;
    }
}

// TO-DO

/*
+ Tính điểm dựa trên 
  Ưu tiên : 
    - khít góc (đều chung 2 cạnh) +3
        - cạnh chung bằng nhau +1
        - cạnh không bằng nhau +0
    - chỉ chung 1 cạnh +1
        - cạnh chung bằng nhau +1
        - cạnh chung không bằng nhau.+0
*/
void Bot::updateStyleSharedAndScore(int index, ValueIndex *value) {

    assert(value->piece != NULL); // Đã có lời giải
}

/*
+ Chạy backtrack cho Bot
*/
SIGNAL_BACKTRACK Bot::backtrack() {

    if (isComplete()) return SUCCESS;
    
    //Lấy đỉnh tiếp theo để ghép hình
    int nextIndex = this->chooseNextVariabel();
    // Lấy tập hình có thể ghép vào.
    vector<ValueIndex *> valueDomain= this->getListValues(nextIndex);

    for (int i = 0; i < valueDomain.size(); i++) {
        ValueIndex *value = valueDomain[i];
        // Ghép thử vào lời giải
        this->addValueToCurrAnswer(nextIndex, value);
        //NOTE: Có thể dùng thêm phép suy luận của Minh óc chó để loại luôn nhánh 

        // Lấy kết quả của bước ghép hình tiếp
        SIGNAL_BACKTRACK result = this->backtrack();
        if (result != FAILURE) return SUCCESS;

        // Do không ghép được nên gỡ cái mảnh vừa ghép thử vào ra.
        this->undo();
    }

    // Không lắp được Tại đỉnh index.
    return FAILURE;
}

//TO_DO
/*
- Check xem đã fit vào Frame hay chưa. Tìm hình chữ nhật bao quanh currentPiece xem nó có chiều dài 
    và chiều rộng bằng Frame hay không. 
- Nếu chưa lắp hết (Miền Piece vẫn còn) thì trả về false
*/
bool Bot::isComplete() {
    return false;
}

// TO-DO
/*
- Hàm này ghép thử value vào vị trí index 
*/
void Bot::addValueToCurrAnswer(int index, ValueIndex *value) {


}

// TO-DO 
/*
- Hàm này đối ngược với hàm addValueToCurAnswer (), nó gỡ mảnh ghép vào nhưng không thành công ra.
*/
void Bot::undo() {

}