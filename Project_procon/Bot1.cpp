#include "Bot1.h"
#include "Grid.h"
#include <iostream>

using std::cout;
using std::endl;

#include <cassert>
#include <vector>

using std::vector;

#include "ValueIndex.h"

#define EPSILON (1e-3)

Bot1::Bot1(int numPieces, Piece **allPieces) {

    this->numPieces = numPieces;
    this->allPieces = allPieces;
}

Bot1::~Bot1() {
    
    // for (int i = 0; i < numPieces; i++) {
    //     delete allPieces[i];
    // }
    // if (numPieces > 0) delete[] allPieces;
}

Bot1::Bot1(int numPieces,Piece** allPieces, Frame *frame) {
    this->numPieces = numPieces;
    this->allPieces = new Piece*[numPieces];
    for (int i = 0; i < numPieces; i++) {
        this->allPieces[i] = allPieces[i];
    }
    this->frame = frame;
}

void Bot1::preprocess() {

    Grid grid(200, 200, 7);

    for (int i = 0; i < this->numPieces; i++) {
        // cout << "Piece 1 : " << i << endl;
        for (int j = i + 1; j < this->numPieces; j++) {
            // cout << "Piece 2 : " << j << endl;
            if (i != j) {
                bool changed = this->tryConcatTwoPieces(allPieces[i], allPieces[j]);
                if (changed) {
                    this->removePiece(j);
                    //grid.showPiece(allPieces[i]);
                    j = 0;
                }
            }

        }
    }

    // cout << "After preprocessing" << endl;
    // vector<Frame*> vect;
    // for (int i = 0; i < this->numPieces; i++) {
    //     grid.showPiece(allPieces[i], 0);
    //     //vect.push_back(allPieces[i]);
    // }
    // grid.showPieceSameTime(vect);
}

bool Bot1::tryConcatTwoPieces(Piece *&piece1, Piece *piece2) {

    int index1 = -1;
    int index2 = -1;
    int indexFlip1 = -1;
    int indexFlip2 = -1;
    bool possibleNoFlip = false;
    bool possibleFlip = false;

    // cout << "Piece1 : "; piece1->showEdge();
    // cout << "Piece2 : "; piece2->showEdge();

    for (int i = 0; i < piece1->getNumVertices(); i++) {

        float angle1 = piece1->getAngles()[i];

        if (abs(angle1 - 270) > EPSILON && abs(angle1 - 90) > EPSILON) {

            for (int j = 0; j < piece2->getNumVertices(); j++) {

                float angle2 = piece2->getAngles()[j];

                // Sum angles = 360 And ratio comfortable;
                if (abs(angle1 + angle2 - 360) < EPSILON) {
                    // cout << "NGhi " << i << " va " << j << endl;
                    if (piece1->isRationalRatio(i, NEXT_EDGE, piece2, j, NEXT_EDGE) 
                        && piece1->isRationalRatio(i, PREV_EDGE, piece2, j, PREV_EDGE))
                        {
                            indexFlip1 = i; indexFlip2 = j;
                            // cout << "Flip " << angle1 << " and " << angle2 << ". " << indexFlip1 << " and " << indexFlip2 << endl;
                            possibleFlip = true;
                            //break;
                        }
                    //cout << "----------------------------" << endl;
                    if (piece1->isRationalRatio(i, NEXT_EDGE, piece2, j, PREV_EDGE) 
                        && piece1->isRationalRatio(i, PREV_EDGE, piece2, j, NEXT_EDGE))
                        {
                
                            index1 = i; index2 = j;
                            // cout << "Not Flip " << angle1 << " and " << angle2 << ". " << index1 << " and " << index2 << endl;
                            // cout << piece1->getSquareEdge()[index1] << " ~ " 
                            //     << piece2->getSquareEdge()[(index2-1+piece2->getNumVertices()) % piece2->getNumVertices()]
                            //     << ". " << piece1->getSquareEdge()[(index1-1+piece1->getNumVertices()) % piece1->getNumVertices()] 
                            //     << "~" << piece2->getSquareEdge()[index2] << endl;
                            possibleNoFlip = true;
                            //break;
                        }

                    // cout << "Piece1 : " << *piece1;
                    // cout << "Piece2 : " << *piece2;
                    // Grid grid(100, 100, 10);
                    // vector<Frame *> vect; vect.push_back(piece1); vect.push_back(piece2);
                    // grid.showPieceSameTime(vect);

                    if (possibleFlip && possibleNoFlip) {
                        if (piece1->getSquareEdge()[index1] == piece2->getSquareEdge()[(index2-1+piece2->getNumVertices()) % piece2->getNumVertices()]
                            || piece2->getSquareEdge()[index2] == piece1->getSquareEdge()[(index1-1+piece1->getNumVertices()) % piece1->getNumVertices()] ) 
                        {
                            possibleNoFlip = false;
                        }

                        if (piece1->getSquareEdge()[index1] == piece2->getSquareEdge()[index2]
                            || piece2->getSquareEdge()[(index2-1+piece2->getNumVertices()) % piece2->getNumVertices()] == piece1->getSquareEdge()[(index1-1+piece1->getNumVertices()) % piece1->getNumVertices()] ) 
                        {
                            possibleFlip = false;
                        }
                    }

                    if (possibleNoFlip || possibleFlip) break;
                }

            }

            if (possibleNoFlip || possibleFlip) break;
        }

    }
    if (possibleNoFlip) {
        
        Piece *result = new Piece(*piece2);
        bool notOverlap = piece2->transition(piece1, index1, index2, EQUAL, result);
        // Grid grid(100, 100, 10);
        // vector<Frame *> vect; vect.push_back(piece1); vect.push_back(result);
        // cout << "No Flip" << endl; 
        //grid.showPieceSameTime(vect);

        if (notOverlap && !piece1->isOverlap(result)) {

            // cout << "Ghep thanh cong" << endl;
            Piece *resultUnion = NULL;
            piece1->unionOtherPiece(result, resultUnion);
            // cout << "TEST " << resultUnion->getAngles()[1] << endl;
            piece1->assign(resultUnion);
            // cout << *piece1;
            delete result;
            delete resultUnion;
            return true;
        }
        delete result;
    }

    if (possibleFlip) {

        // Grid grid(100, 100, 10);
        // cout << "Piece 1 : " << *piece1;
        // cout << "Piece 2 : " << *piece2;
        vector<Frame*> vect1; vect1.push_back(piece1); vect1.push_back(piece2);
        // grid.showPieceSameTime(vect1);
        Piece *flipPiece2 = new Piece(*piece2);
        piece2->flip(flipPiece2);
        piece2->cloneSamePieceToMyMemory(flipPiece2);
        delete flipPiece2;
        //cout << *piece2;

        for (int i = 0; i < piece2->getNumVertices(); i++) {

            if (abs(piece1->getAngles()[indexFlip1] + piece2->getAngles()[i] - 360) <= EPSILON) {
                indexFlip2 = i;
                break;
            }
        }

        Piece *result = new Piece(*piece2);
        bool notOverlap = piece2->transition(piece1, indexFlip1, indexFlip2, EQUAL, result);
        vector <Frame *> vect; vect.push_back(piece1); vect.push_back(result);
        // cout << "FLIP" << endl;
        //grid.showPieceSameTime(vect);

        if (notOverlap && !piece1->isOverlap(result)) {
            // cout << "Ghep thanh cong" << endl;
            Piece *resultUnion = NULL;
            piece1->unionOtherPiece(result, resultUnion);
            piece1->assign(resultUnion);
            //grid.showPiece(piece1);
            delete result;
            delete resultUnion;
            return true;
        }
        delete result;
    }

    return false;
}

void Bot1::removePiece(int index) {

    assert(0 <= index && index < this->numPieces);
    // delete allPieces[index];
    // allPieces[index] = NULL;
    for (int i = index; i < this->numPieces - 1; i++) {
        allPieces[i] = allPieces[i+1];
    }
    this->numPieces--;
}

void Bot1::run() {

    this->preprocess();
    Grid grid(100, 100, 10);
    Piece *currAnswer = this->chooseFirstPiece();
    this->currVect.push_back(currAnswer);

    for (int i = 0; i < this->numPieces; i++) {
        if (allPieces[i] != currAnswer) currDomain.push_back(allPieces[i]);
    }
    // cout << currDomain.size() << " and " << this->numPieces << endl;
    SIGNAL sig = this->backtrack(currAnswer);
    grid.showPiece(currAnswer);
}

SIGNAL Bot1::backtrack(Piece *currAnswer) {

    if (this->isComplete(currAnswer, frame)) return SUCCESS1;

    vector<int> queueIndex;
    this->sortPrioIndex(currAnswer, queueIndex);

    while (queueIndex.size() > 0) {

        int variable = queueIndex[queueIndex.size() - 1]; queueIndex.pop_back();
        vector<ValueIndex*> listValues;
        // cout << "OK" << endl;
        currAnswer->moveIfOutMap();
        this->getListValues(currAnswer, variable, listValues);
        // cout << "OK2" << endl;
        //cout << "Size : " << listValues.size() << endl;
        Grid grid(200, 200, 3);
        // for (int t = listValues.size() - 1; t >= 0 ; t--) {
        //     vector<Frame *> vect; vect.push_back(currAnswer); vect.push_back(listValues[t]->transitionPiece);
        //     cout << listValues[t]->score << " : " << currAnswer->getPerimeter()
        //             << " - " << listValues[t]->transitionPiece->getPerimeter() << endl;
        //     grid.showPieceSameTime(vect, 0);
        // }

        for (int t = listValues.size() - 1; t >= 0 ; t--) {
            Piece *tmpPiece = new Piece(*currAnswer);
            Piece *resultUnion = NULL;
            vector<Frame *> vect; vect.push_back(currAnswer); vect.push_back(listValues[t]->transitionPiece);
            currAnswer->unionOtherPiece(listValues[t]->transitionPiece, resultUnion);
            this->currVect.push_back(listValues[t]->transitionPiece);
            currAnswer->assign(resultUnion);
            //currAnswer->moveIfOutMap();
            grid.showPieceSameTime(this->currVect, 10);
            this->removePieceFromDomain(listValues[t]->piece);

            SIGNAL sig = this->backtrack(currAnswer);
            if (sig == SUCCESS1) return SUCCESS1;
            currAnswer->assign(tmpPiece);
            this->currVect.pop_back();
            this->currDomain.push_back(listValues[t]->piece);
            delete listValues[t]->transitionPiece;
        }

    }

    return FAILURE1;
}

void Bot1::removePieceFromDomain(Piece *piece) {

    int index = -1;
    for (int i = 0; i < currDomain.size(); i++) {
        if (piece == currDomain[i]) {
            index = i;
            break;
        }
    }

    assert(index != -1);

    for (int i = index; i < currDomain.size() - 1; i++) {
        currDomain[i] = currDomain[i+1];
    }
    currDomain.pop_back();
}

void Bot1::getListValues(Piece *currAnswer, int index, vector<ValueIndex*> &listValues) {

    float angle1 = currAnswer->getAngles()[index];
    int edgeNext1 = currAnswer->getSquareEdge()[index];
    int edgePrev1 = currAnswer->getSquareEdge()[(index - 1 + currAnswer->getNumVertices()) % currAnswer->getNumVertices()];

    for (int i = 0; i < currDomain.size(); i++) {

        Piece *otherPiece = currDomain[i];
        
        for (int j = 0; j < otherPiece->getNumVertices(); j++) {

            float angle2 = otherPiece->getAngles()[j];
            int edgeNext1 = otherPiece->getSquareEdge()[j];
            int edgePrev1 = otherPiece->getSquareEdge()[(j - 1 + otherPiece->getNumVertices()) % otherPiece->getNumVertices()];

            bool possibleFlip = false;
            bool possibleNoFlip = false;
            
            if (abs(angle1 + angle2 - 360) < EPSILON) {

                if (currAnswer->isRationalRatio(index, NEXT_EDGE, otherPiece, j, NEXT_EDGE) 
                        && currAnswer->isRationalRatio(index, PREV_EDGE, otherPiece, j, PREV_EDGE))
                        {
                            possibleFlip = true;
                        }

                if (currAnswer->isRationalRatio(index, NEXT_EDGE, otherPiece, j, PREV_EDGE) 
                   && currAnswer->isRationalRatio(index, PREV_EDGE, otherPiece, j, NEXT_EDGE))
                    {
                
                        possibleNoFlip = true;
                    }

            }
            
            if (possibleNoFlip) {
        
                Piece *result = new Piece(*otherPiece);
                bool notOverlap = otherPiece->transition(currAnswer, index, j, EQUAL, result);

                // Grid grid(100, 100, 10);
                vector<Frame*> vect; vect.push_back(currAnswer); vect.push_back(result);
                // grid.showPieceSameTime(vect);
                if (notOverlap && !currAnswer->isOverlap(result)) {
                    ValueIndex *newAns = new ValueIndex(otherPiece, result, j, false);
                    newAns->styleShared = SharedEdgeStyle::EQUAL_ANGLE;
                    // compute score
                    Piece *resultUnion = NULL;
                    currAnswer->unionOtherPiece(result, resultUnion);
                    newAns->score = 0*(currAnswer->getPerimeter() + result->getPerimeter() - resultUnion->getPerimeter());
                    newAns->score += 1*(currAnswer->getNumVertices() + result->getNumVertices() - resultUnion->getNumVertices() );
                    newAns->score += 0*resultUnion->getNumVertices();
                    if (resultUnion->comfortReference(this->frame))  
                    this->pushToListValues(listValues, newAns);
                    // cout << *newAns->piece;

                    // delete result;
                    delete resultUnion;
                }
                //delete result;
            } 
            // cout << "OK3" << endl;
            if (possibleFlip) {
                    // cout << "FLIP" << endl;
                    Piece *flipPiece2 = new Piece(*otherPiece);
                    otherPiece->flip(flipPiece2);
                    int indexFlip2 = -1;
                    // cout << "j = " << j << " with angle " << otherPiece->getAngles()[j]
                    //     << " with Edge " << otherPiece->getSquareEdge()[j] << endl;
                    // otherPiece->showAngles();
                    // otherPiece->showEdge();
                    // flipPiece2->showAngles();
                    // flipPiece2->showEdge();
                    for (int k = 0; k < otherPiece->getNumVertices(); k++) {

                        if (abs(otherPiece->getAngles()[j] - flipPiece2->getAngles()[k]) <= EPSILON
                            && otherPiece->getSquareEdge()[j] == flipPiece2->getSquareEdge()[k]
                            && otherPiece->getSquareEdge()[(j - 1 + otherPiece->getNumVertices()) % otherPiece->getNumVertices()] == flipPiece2->getSquareEdge()[(k + 1) % flipPiece2->getNumVertices()]) {
                            indexFlip2 = k;
                            break;
                        }
                    }

                    Piece *result = new Piece(*flipPiece2);
                    // cout << "OK6 " << index << " and " << indexFlip2 << endl;
                    // Grid grid(100, 100, 10);
                    // vector<Frame*> vect; vect.push_back(otherPiece); vect.push_back(flipPiece2);
                    // grid.showPieceSameTime(vect);
                    bool notOverlap = flipPiece2->transition(currAnswer, index, indexFlip2, EQUAL, result);
                    // cout << "OKE7" << endl; 
                    if (notOverlap && !currAnswer->isOverlap(result)) {
                        // cout << "Ghep thanh cong" << endl;
                        ValueIndex *newAns = new ValueIndex(otherPiece, result, indexFlip2, false);
                        newAns->styleShared = SharedEdgeStyle::EQUAL_ANGLE;
                        // compute score
                        Piece *resultUnion = NULL;
                        currAnswer->unionOtherPiece(result, resultUnion);
                        newAns->score = 0*(currAnswer->getPerimeter() + result->getPerimeter() - resultUnion->getPerimeter());
                        newAns->score += 1 * (currAnswer->getNumVertices() + result->getNumVertices() - resultUnion->getNumVertices());
                        newAns->score += 0 * resultUnion->getNumVertices();
                        if (resultUnion->comfortReference(this->frame)) 
                        this->pushToListValues(listValues, newAns);
                        // cout << *newAns->piece;

                        // delete result;
                        delete resultUnion;
                    }

                    delete flipPiece2;
                    //delete result;
            }

        } // - END OF ANGLE

    }

}

void Bot1::pushToListValues(vector<ValueIndex*> &listValues, ValueIndex *newAns) {

    listValues.push_back(newAns);
    // cout << listValues.size()  << endl;

    for (int i = listValues.size() - 1; i > 0; i--) {
        // cout << "HOLA2" << endl;
        if (listValues[i]->score < listValues[i - 1]->score) {

            ValueIndex *tmp = listValues[i];
            listValues[i] = listValues[i-1];
            listValues[i-1] = tmp;
        }
        else {
            break;
        }
    }
}

bool Bot1::isComplete(Piece* p1, Frame* p2) {
    if(p1->getNumVertices() != p2->getNumVertices()){
        return false;
    }
    //cout<<"testingn 00000"<<endl;
    //getchar();
    Dot** ver1 = p1->getVertices();
    Dot** ver2 = p2->getVertices();

    float chuvi1 = 0;
    float chuvi2 = 0;

    for(int i = 0; i < p1->getNumVertices(); i ++){
        if(i < p1->getNumVertices() - 1){
            chuvi1 += this->distance(ver1[i],ver1[i + 1]);
        }else{
            chuvi1 += this->distance(ver1[0],ver1[p1->getNumVertices() - 1]);
        }

    }

    for(int i = 0; i < p2->getNumVertices(); i ++){
            if(i < p2->getNumVertices() - 1){
                chuvi2 += this->distance(ver2[i],ver2[i + 1]);
            }else{
                chuvi2 += this->distance(ver2[0],ver2[p2->getNumVertices() - 1]);
            }

    }

    if(abs(chuvi1 - chuvi2) > EPSILON){
        return false;
    }

    return true;


}

float Bot1::distance(Dot* d1, Dot* d2){
    return sqrt(pow(d1->x - d2->x, 2.0) + pow(d1->y - d2->y, 2.0));
}


bool biggerPri(Piece *piece, int i1, int i2) {

    int squareEdgeI1 = piece->getSquareEdge()[i1];
    int squareEdgeI2 = piece->getSquareEdge()[i2];
    int squareEdgePrevI1 = piece->getSquareEdge()[(i1 - 1 + piece->getNumVertices()) % piece->getNumVertices()];
    int squareEdgePrevI2 = piece->getSquareEdge()[(i2 - 1 + piece->getNumVertices()) % piece->getNumVertices()];

    if (squareEdgeI1 + squareEdgePrevI1 < squareEdgeI2 + squareEdgePrevI2) return true;
    //if (squareEdgeI1 < squareEdgeI2) return true;
    return false;
}

void pushToQueue(Piece *piece, int index, vector<int> &queue) {

    queue.push_back(index);
    for (int i = queue.size()-1; i > 0; i--) {
        if (!biggerPri(piece, queue[i], queue[i-1])) {
            int tmp = queue[i];
            queue[i] =  queue[i-1];
            queue[i-1] = tmp;
        }
        else {
            return;
        }
    }
}

void Bot1::sortPrioIndex(Piece *piece, vector<int> &queueIndex) {

    queueIndex.push_back(0);

    for (int i = 1; i < piece->getNumVertices(); i++) {

        pushToQueue(piece, i, queueIndex);
    }
}

Piece* Bot1::chooseFirstPiece() {

    int index = 0;
    int min = allPieces[0]->getPerimeter();

    for (int i = 1; i < this->numPieces; i++) {

        if (min < allPieces[i]->getPerimeter()) {
            min = allPieces[i]->getPerimeter();
            index = i;
        }
    }

    return allPieces[index];
}

// Piece* Bot1::chooseFirstPiece() {

//     int index = 0;
//     int min = allPieces[0]->getNumVertices();

//     for (int i = 1; i < this->numPieces; i++) {

//         if (min < allPieces[i]->getNumVertices()) {
//             min = allPieces[i]->getNumVertices();
//             index = i;
//         }
//     }

//     return allPieces[index];
// }