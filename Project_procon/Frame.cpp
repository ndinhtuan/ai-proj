#include "Frame.h"
#include <cassert>
#include <iostream>
#include <cstddef>

using std::cout;
using std::endl;

#include <cmath>

using std::abs;
#include <stdio.h>

// định nghĩa epsilon để so sánh góc.
#define EPSILON 1e-4

Frame::Frame(int numVertices, int id)
{

    this->numVertices = numVertices;
    this->angles = new float[numVertices];
    this->vertices = new Dot *[numVertices];
    this->squareEdge = new int[numVertices];
    this->id = id;

    for (int i = 0; i < numVertices; i++)
    {
        this->vertices[i] = new Dot();
    }

    //Frame::calcSquareEdge(this); Chưa biết tọa độ các đỉnh để mà tính.
}

Frame::Frame()
{
}

Frame::Frame(const Frame &ob) {

    this->numVertices = ob.numVertices;
    this->angles = new float[numVertices];
    this->id = ob.id;

    this->squareEdge = new int[numVertices];
    this->vertices = new Dot *[numVertices];
    for (int i = 0; i < numVertices; i++)
    {
        this->vertices[i] = new Dot();
        this->vertices[i]->x = ob.vertices[i]->x;
        this->vertices[i]->y = ob.vertices[i]->y;

        this->angles[i] = ob.angles[i];

        this->squareEdge[i] = ob.squareEdge[i];
    }
}

Frame::~Frame() {
    if (this->numVertices == 0 || this->vertices == NULL) return;

    delete[] angles;
    delete[] squareEdge;
    for (int i = 0; i < this->numVertices; i++) {
        delete vertices[i];
    }
    delete[] vertices;
}

void Frame::setCoord(int index, int x, int y)
{

    assert(this->numVertices > 0 && this->vertices != NULL);
    this->vertices[index]->x = x;
    this->vertices[index]->y = y;
}

void Frame::setCoord(int index, const Dot &coord)
{

    assert(this->numVertices > 0 && this->vertices != NULL);
    this->vertices[index]->x = coord.x;
    this->vertices[index]->y = coord.y;
}

ostream &operator<<(ostream &os, const Frame &frame)
{

    os << "ID : " << frame.id << endl;
    os << "#vertices : " << frame.numVertices << endl;

    for (int i = 0; i < frame.numVertices; i++)
    {

        os << "(" << frame.vertices[i]->x << ", " << frame.vertices[i]->y << "); ";
    }
    os << endl;

    return os;
}

const Frame &Frame::operator=(const Frame &ob)
{

    if (this == &ob)
        return *this;

    delete this;

    this->numVertices = ob.numVertices;
    this->angles = new float[numVertices];
    this->vertices = new Dot *[numVertices];
    this->squareEdge = new int[numVertices];
    this->id = ob.id;

    for (int i = 0; i < numVertices; i++)
    {
        this->vertices[i] = new Dot();
        this->vertices[i]->x = ob.vertices[i]->x;
        this->vertices[i]->y = ob.vertices[i]->y;

        angles[i] = ob.angles[i];
        squareEdge[i] = ob.squareEdge[i];
    }

    return *this;
}

void Frame::calcSquareEdge(Frame *frame)
{

    assert(frame != NULL);

    for (int i = 0; i < frame->numVertices; i++)
    {

        int nextPoint = (i + 1) % frame->numVertices;
        frame->squareEdge[i] = (frame->vertices[i]->x - frame->vertices[nextPoint]->x) * (frame->vertices[i]->x - frame->vertices[nextPoint]->x) + (frame->vertices[i]->y - frame->vertices[nextPoint]->y) * (frame->vertices[i]->y - frame->vertices[nextPoint]->y);
    }
}

void Frame::calcSquareEdge() {

  for (int i = 0; i < this->numVertices; i++)
    {

        int nextPoint = (i + 1) % this->numVertices;
        this->squareEdge[i] = (this->vertices[i]->x - this->vertices[nextPoint]->x) * (this->vertices[i]->x - this->vertices[nextPoint]->x) + (this->vertices[i]->y - this->vertices[nextPoint]->y) * (this->vertices[i]->y - this->vertices[nextPoint]->y);
    }
}
int Frame::getNumSpecialAngles()
{

    assert(numVertices > 0); // đã init Piece;

    int numSpecialAngles = 0;
    for (int i = 0; i < numVertices; i++)
    {

        if (angles[i] > 180 + EPSILON && abs(angles[i] - 270) > EPSILON)
            numSpecialAngles++;
    }

    return numSpecialAngles;
}


// Thắng 
void Frame::crossed(Dot* a, Dot* a2, Dot* a3,int &right, int &left){

  //std::cout<<endl<<"calculating..."<<endl;
  //std::cout<<"croes_check"<<(0.333*a->x +0.454*a->y -0.333*a2->x - 0.454*a2->y)*(0.333*a->x + 0.454*a->y - 0.333*a3->x - 0.454*a3->y)<<endl;
  //cout<<endl<<"("<<a2->x<<","<<a2->y<<")";
  //cout<<endl<<"("<<a3->x<<","<<a3->y<<")";
  if((0.333*a2->x +0.454*a2->y - 0.333*a->x - 0.454*a->y)*(0.333*a3->x + 0.454*a3->y - 0.333*a->x - 0.454*a->y) > 0){
    //cout<<"ko cat"<<endl;
    return;
  }

//  std::cout<<"crossed"<<endl;

  //std::cout<<endl<<"*********************"<<endl;
  double x;
  //std::cout<<a2->y-a3->y<<endl;
  //getchar();

  if(a2->x-a3->x == 0){
    x = a2->x;
  }else{
    //y = (-(-0.333*a->x - 0.454*a->y)/ 0.333 + (a2->x*a3->y - a2->y*a3->x)/(a2->y-a3->y)) / (0.454/0.333 + (a2->x - a3->x)/(a2->y-a3->y));
    double n = a2->y - a3->y;
    double m = a2->x - a3->x;

    x = (a->y - a2->y + a2->x*(n/m) + (0.333/0.454)*a->x )/(n/m + 0.333/0.454);
  }

  //y = (-(-0.333*a->x - 0.454*a->y)/ 0.333 + (a2->x*a3->y - a2->y*a3->x)/(a2->y-a3->y)) / (0.454/0.333 + (a2->x - a3->x)/(a2->y-a3->y));
  //x = (-0.454*y + 0.333*a->x + 0.454*a->y)/0.333;
  //cout<<x<<" , "<<endl;
  //cout<<"--------------------------"<<endl;
  //std::cout<<"x = "<<x<<endl;
  //std::cout<<"y = "<<y<<endl;
  //getchar();
    if(x > a->x){
      right ++;
      return;
    }

    left ++;
    return;
}

bool Frame::insideAngle( Dot *vertice){
    Dot* firstPoint;
    Dot* secondPoint;
    int rightPoints = 0;
    int leftPoints = 0;
    //rightPoints = 0;
    //leftPoints = 0;

    int j ;
    //getchar();
    for (int i = 0; i < numVertices ; i ++){
      j = i;
      if(this->vertices[i]->x == vertice->x && this->vertices[i]->y == vertice->y){
          j ++;
          //std::cout<<"i = "<<i<<endl;
      }
      firstPoint = this->vertices[j];
      //std::cout<<"( "<<j<<" ";

      if(j < this->numVertices - 1){
        if(this->vertices[j+1]->x == vertice->x && this->vertices[j+1]->y == vertice->y){
          j ++;
        }
        if(j == this->numVertices - 1){
          secondPoint = this->vertices[0];
          //std::cout<<0;
        }else{
          secondPoint = this->vertices[j+1];
          //std::cout<<j+1;
        }
      }else if (this->vertices[0]->x == vertice->x && this->vertices[0]->y == vertice->y){
        //j = 0;
        secondPoint = this->vertices[1];
        //std::cout<<1;
      }else{
        secondPoint = this->vertices[0];
      //  std::cout<<0;
      }
      //std::cout<<")";

      //secondPoint = this->vertices[j+1];
      //std::cout<<j<<endl;
    //  getchar();
      crossed(vertice,firstPoint,secondPoint,rightPoints,leftPoints);
      //std::cout<<"right = "<<rightPoints<<endl;
      //std::cout<<"left = "<<leftPoints<<endl;
      //std::cout<<"calculated"<<endl;

      //getchar();
      i = j;
      //std::cout<<i<<endl;
    }

    if(rightPoints %2 == 0 && leftPoints % 2 ==0 ){
      return true;
    }

    return false;
}

void Frame::calcAngle(){
  //this->vertices[0];

  Dot* rightPoint;
  Dot* leftPoint;
  for(int i = 0; i < this->numVertices; i ++){
    if(i == 0){
      leftPoint = this->vertices[this->numVertices - 1];
    }else{
      leftPoint = this->vertices[i - 1];
    }

    if(i == this->numVertices - 1){
      rightPoint = this->vertices[0];
    }else{
      rightPoint = this->vertices[i + 1];
    }
    //cout<<endl<<rightPoint->x<<"  "<<rightPoint->y<<endl;
    //cout<<leftPoint->x<<"  "<<leftPoint->y<<endl;

    //getchar();
    Dot* v1 = new Dot();
    Dot* v2 = new Dot();

    v1->x = rightPoint->x - this->vertices[i]->x;
    v1->y = rightPoint->y - this->vertices[i]->y;
    //getchar();
    //cout<<"cross";
    v2->x = leftPoint->x - this->vertices[i]->x;
    v2->y = leftPoint->y - this->vertices[i]->y;
    //getchar();
    float vcos = (v1->x*v2->x + v1->y*v2->y) / (sqrt( pow(float(v1->x),2.0) + pow(float(v1->y),2.0) ) * sqrt(pow(float(v2->x),2.0) + pow(float(v2->y),2.0)));
    this->angles[i] = acos(vcos) * 180 / 3.14159265;

    if(!insideAngle(this->vertices[i])){
      this->angles[i] = 360 - this->angles[i];
    }

  }


}

void Frame::move(Dot* a){
    assert(a != NULL);
    for(int i = 0; i < this->numVertices; i ++){
        this->vertices[i]->x += a->x;
        this->vertices[i]->y += a->y;
    }
}

void Frame::flip(Frame *result) {

  assert(this->numVertices == result->numVertices);

  for (int i = 0; i < result->numVertices; i++) {
    result->vertices[i]->x = -this->vertices[i]->x;
    result->vertices[i]->y = this->vertices[i]->y;
  }

  Dot *a = new Dot(-result->vertices[0]->x + this->vertices[0]->x, -result->vertices[0]->y + this->vertices[0]->y);
  result->move(a);
  delete a;
  result->reverseDot(1 , result->numVertices-1);
}

/*
Đảo tọa độ các đỉnh từ begin đến end
*/
void Frame::reverseDot(int begin, int end) {

    for (int i = begin; i <= (end + begin)/2; i++) {
      Dot tmp(this->vertices[i]->x, this->vertices[i]->y);
      float tmpAngle = this->angles[i];
      int tmpEdge = this->squareEdge[i];

      //cout << begin+end-i << " VS " << this->numVertices << endl;
      this->vertices[i]->x = this->vertices[begin+end-i]->x;
      // cout << "Halo" << endl;
      this->vertices[i]->y = this->vertices[begin+end-i]->y;
      this->angles[i] = this->angles[begin+end-i];
      this->squareEdge[i] = this->squareEdge[begin+end-i];

      this->vertices[begin+end-i]->x = tmp.x;
      this->vertices[begin+end-i]->y = tmp.y;
      this->angles[begin+end-i] = tmpAngle;
      this->squareEdge[begin+end-i] = tmpEdge;

    }
}
#include <iostream>

using std::cout; 
using std::endl;

void Frame::showAngles() {

  for (int i = 0; i < this->numVertices; i++) {
    cout << this->angles[i] << ", ";
  }
  cout << endl;
}