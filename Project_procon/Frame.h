/*
- class Frame describe Frame object.
- To-do : 
        + calcAngle() -----> Thắng
*/
#ifndef FRAME_H
#define FRAME_H

#include "Dot.h"
#include <iostream>

using std::ostream;



// Liệt kê thể loại cạnh tại một đỉnh (kề trước và kề kế tiếp).
//  Vì tại một đỉnh có 2 cạnh kề với nó.
enum ADJACENT_EDGE {
  NEXT_EDGE,
  PREV_EDGE
};

class Frame
{
public:
  Frame();
  Frame(int numVertices, int id);
  Frame(const Frame &ob);
  ~Frame();
  // set coord for vectice index th
  void setCoord(int index, int x, int y);
  void setCoord(int index, const Dot &);

  void crossed( Dot* a, Dot* a2, Dot* a3,int &right, int &left);
  bool insideAngle( Dot *vertice);

  // DONE.
  // calculate float* angles, base on Dot **vertices
  // Need to check parse data for vertices BEFORE calculae angles
  void calcAngle();
  void calcSquareEdge();
  void move(Dot *a);
  // Lấy giá trị của numVertices
  int getNumVertices() const
  {
    return this->numVertices;
  }
  Dot **getVertices() const
  {
    return this->vertices;
  }
  float *getAngles() const
  {
    return this->angles;
  }
  int* getSquareEdge() const {
    return this->squareEdge;
  }

  // Đếm số góc có giá trị > 180 và khác 270
  int getNumSpecialAngles();

  friend ostream &operator<<(ostream &os, const Frame &);
  const Frame &operator=(const Frame &);
  void reverseDot(int begin, int end);
  void flip(Frame *result);
  void showAngles();

  static void calcSquareEdge(Frame *);

protected:
  float *angles;
  Dot **vertices;
  int *squareEdge;
  int numVertices;
  int id;
};

#endif
