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

class Frame
{
public:
  Frame();
  Frame(int numVertices, int id);
  // set coord for vectice index th
  void setCoord(int index, int x, int y);
  void setCoord(int index, const Dot &);

  // TO-DO
  // calculate float* angles, base on Dot **vertices
  // Need to check parse data for vertices BEFORE calculae angles
  void calcAngle();
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

  // Đếm số góc có giá trị > 180 và khác 270
  int getNumSpecialAngles();

  friend ostream &operator<<(ostream &os, const Frame &);
  const Frame &operator=(const Frame &);

  static void calcSquareEdge(Frame *);

protected:
  float *angles;
  Dot **vertices;
  int *squareEdge;
  int numVertices;
  int id;
};

#endif
