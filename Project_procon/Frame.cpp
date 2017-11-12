#include "Frame.h"
#include <cassert>
#include <iostream>
#include <cstddef>

using std::endl;

#include <cmath>

using std::abs;

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
    this->id = ob.id;

    this->angles = new float[numVertices];
    this->vertices = new Dot *[numVertices];
    this->squareEdge = new int[numVertices];

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
