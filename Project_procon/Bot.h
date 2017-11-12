#ifndef BOT_H
#define BOT_H

#include "Frame.h"
#include "Piece.h"
#include "ValueIndex.h"
#include <vector>

using std::vector;

enum SIGNAL_BACKTRACK {
  FAILURE,
  SUCCESS
};

class Bot
{
  public:
    Bot(int numPieces, Frame *frame, Piece **allPieces);
    ~Bot();

    /*  
    + Hàm này chọn Piece làm mảnh Init có ưu tiên 
    */
    Piece *chooseInitPiece();
    /*
    + chooseNextVariabel() chọn đỉnh tiếp theo để ghép hình, đinh tiếp theo là các đỉnh trong số các đỉnh 
        của currentPiece
    + Có ưu tiên.
    @Return về đỉnh tiếp theo được chọn để ghép vào.
    */
    int chooseNextVariabel();
    /*
    + getValues(int index) tìm các value (biến thể của Piece)
    + Có ưu tiên.
    @ trả về list các phương án có thể phù hợp với currentPiece tại đỉnh index
      - Priority queue
    */
    vector<ValueIndex*> getListValues(int index);
    /*
    - Chạy thuật toán backtrack cho Bot để ghép hình. 
    - Chủ yếu cải tiến ở đây để Bot chạy fucken wow shit
    */
    SIGNAL_BACKTRACK backtrack();
    /*
    - Hàm này check xem currentPiece đã hoàn thành hay chưa, đã fit vào Frame hay chưa.
    */
    bool isComplete();
    void addValueToCurrAnswer(int index, ValueIndex *);
    void undo();

  private:
    Frame *frame;
    Piece **allPieces;
    Piece *currAnswer; // lời giải hiện tại. Chứa tấp các piece đã ghép thành công.
    int numPieces;

    /*
    + currentDomainPiece lưu miền giá trị hiện tại sử dụng được để ghép vào 1 đỉnh của currentPiece.
    + Được init ban đầu là allPieces 
    + Mỗi khi một Piece được đưa vào lời giải hiện tại (currentPiece) thì piece đó được pop ra khỏi 
      currentDomainPiece.
    */
    vector<Piece*> currentDomainPiece;
  private:

    void pushPriorityQueue(vector<ValueIndex*> &listValues, ValueIndex *value);
    /*
      - Update styleShared và Score cho một giá trị phù hợp với đỉnh index (sau khi đã check phù hợp)
      @value tham số này đã được thiết lập giá trị piece, indexPiece và flip.
        Dùng các dữ kiện trên và index của currAnswer để update styleShared và Score.
    */
    void updateStyleSharedAndScore(int index, ValueIndex *value);
};

#endif
