#include "Grid.h"
#include <iostream>

using std::cout;
using std::endl;

#include <cassert>

Grid::Grid()
{

    grid = NULL;
}

Grid::Grid(int height, int width, int step)
{

    this->height = height;
    this->width = width;
    this->step = step;

    grid = new Mat(height * step, width * step, CV_8UC3, Scalar(255, 255, 255));

    // for (int x = 0; x < width; x++)
    // {

    //     for (int y = 0; y < height; y++)
    //     {

    //         circle(*grid, Point(x * step, y * step), 2, Scalar(0, 0, 0), -1);
    //     }
    // }

    //cout << "Init Grid successful" << endl;
}

Grid::~Grid()
{

    if (grid != NULL)
    {

        delete grid;
    }
}
void Grid::showPiece(const Frame *frame, int time)
{

    assert(grid != NULL);
    Mat img = (*grid).clone();

    int numVertices = frame->getNumVertices();
    Dot **vertices = frame->getVertices();

    for (int i = 0; i < numVertices; i++)
    {

        Point pt1 = Point(vertices[i]->x * step, vertices[i]->y * step);
        if (i == 0) {
            circle(img, pt1, 2, Scalar(0, 255, 0), -1);
        }
        Point pt2 = Point(vertices[(i + 1) % numVertices]->x * step, vertices[(i + 1) % numVertices]->y * step);
        line(img, pt1, pt2, Scalar(0, 0, 255), 2);
    }
    imshow("Piece", img);
    waitKey(time);
    //destroyWindow("Piece");
}

ostream &operator<<(ostream &os, const Grid &grid)
{

    os << *(grid.grid) << endl;
    return os;
}

void Grid::showPieceSameTime(const vector<Frame *> &vectFrame, int time) {

    assert(grid != NULL);
    Mat img = (*grid).clone();

    for (int i = 0; i < vectFrame.size(); i++) {

        int numVertices = vectFrame[i]->getNumVertices();
        Dot **vertices = vectFrame[i]->getVertices();
        for (int j = 0; j < numVertices; j++)
        {
    
            Point pt1 = Point(vertices[j]->x * step, vertices[j]->y * step);
            if (j == 0) {
                circle(img, pt1, 3, Scalar(0, 255, 0), -1);
            }
            Point pt2 = Point(vertices[(j + 1) % numVertices]->x * step, vertices[(j + 1) % numVertices]->y * step);
            line(img, pt1, pt2, Scalar(0, 0, 255), 2);
        }
    }

    imshow("Piece", img);
    waitKey(time);
    //destroyWindow("Piece"); 
}