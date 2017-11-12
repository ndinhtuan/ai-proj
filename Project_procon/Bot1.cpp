#include "Bot1.h"
#include "Grid.h"
#include <iostream>

using std::cout;
using std::endl;

#include <cassert>

#define EPSILON (1e-3)

Bot1::Bot1(int numPieces, Piece **allPieces) {

    this->numPieces = numPieces;
    this->allPieces = allPieces;
}

void Bot1::preprocess() {

    Grid grid(100, 110, 10);

    for (int i = 0; i < this->numPieces; i++) {
        cout << "Piece 1 : " << i << endl;
        for (int j = i + 1; j < this->numPieces; j++) {
            cout << "Piece 2 : " << j << endl;
            if (i != j) {
                bool changed = this->tryConcatTwoPieces(allPieces[i], allPieces[j]);
                if (changed) {
                    cout << "ghep duoc " << i << " va " << j << endl;
                    // cout << "VL : " << allPieces[i]->getAngles()[1] << endl;
                    this->removePiece(j);
                    //grid.showPiece(allPieces[i]);
                    j = 0;
                }
            }

        }
    }

    cout << "After preprocessing" << endl;
    for (int i = 0; i < this->numPieces; i++) {
        grid.showPiece(allPieces[i], 2000);
    }
}

bool Bot1::tryConcatTwoPieces(Piece *&piece1, Piece *piece2) {

    int index1 = -1;
    int index2 = -1;
    int indexFlip1 = -1;
    int indexFlip2 = -1;
    bool possibleNoFlip = false;
    bool possibleFlip = false;

    for (int i = 0; i < piece1->getNumVertices(); i++) {

        float angle1 = piece1->getAngles()[i];

        if (abs(angle1 - 270) > EPSILON && abs(angle1 - 90) > EPSILON) {

            for (int j = 0; j < piece2->getNumVertices(); j++) {

                float angle2 = piece2->getAngles()[j];

                // Sum angles = 360 And ratio comfortable;
                if (abs(angle1 + angle2 - 360) < EPSILON) {

                    if (piece1->isRationalRatio(i, NEXT_EDGE, piece2, j, PREV_EDGE) 
                        && piece1->isRationalRatio(i, PREV_EDGE, piece2, j, NEXT_EDGE))
                        {
                            index1 = i; index2 = j;
                            cout << "Not Flip " << angle1 << " and " << angle2 << ". " << index1 << " and " << index2 << endl;
                            cout << piece1->getSquareEdge()[index1] << " ~ " 
                                << piece2->getSquareEdge()[(index2-1+piece2->getNumVertices()) % piece2->getNumVertices()]
                                << ". " << piece1->getSquareEdge()[(index1-1+piece1->getNumVertices()) % piece1->getNumVertices()] 
                                << "~" << piece2->getSquareEdge()[index2] << endl;
                            possibleNoFlip = true;
                            //break;
                        }
                    //cout << "----------------------------" << endl;
                    if (piece1->isRationalRatio(i, NEXT_EDGE, piece2, j, NEXT_EDGE) 
                        && piece1->isRationalRatio(i, PREV_EDGE, piece2, j, PREV_EDGE))
                        {
                            indexFlip1 = i; indexFlip2 = j;
                            cout << "Flip " << angle1 << " and " << angle2 << ". " << indexFlip1 << " and " << indexFlip2 << endl;
                            possibleFlip = true;
                            //break;
                        }

                    cout << "Piece1 : " << *piece1;
                    cout << "Piece2 : " << *piece2;
                    Grid grid(100, 100, 10);
                    vector<Frame *> vect; vect.push_back(piece1); vect.push_back(piece2);
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
        Grid grid(100, 100, 10);
        vector<Frame *> vect; vect.push_back(piece1); vect.push_back(result);
        cout << "No Flip" << endl; 
        grid.showPieceSameTime(vect);

        if (notOverlap) {

            cout << "Ghep thanh cong" << endl;
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

        Grid grid(100, 100, 10);
        cout << "Piece 1 : " << *piece1;
        cout << "Piece 2 : " << *piece2;
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
        cout << "FLIP" << endl;
        grid.showPieceSameTime(vect);

        if (notOverlap) {
            cout << "Ghep thanh cong" << endl;
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
    delete allPieces[index];
    allPieces[index] = NULL;
    for (int i = index; i < this->numPieces - 1; i++) {
        allPieces[i] = allPieces[i+1];
    }
    this->numPieces--;
}