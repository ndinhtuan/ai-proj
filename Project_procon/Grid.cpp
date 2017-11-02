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

    for (int x = 0; x < width; x++)
    {

        for (int y = 0; y < height; y++)
        {

            circle(*grid, Point(x * step, y * step), 2, Scalar(0, 0, 0), -1);
        }
    }

    cout << "Init Grid successful" << endl;
}

Grid::~Grid()
{

    if (grid != NULL)
    {

        delete grid;
    }
}
void Grid::showPiece(const Frame *frame)
{

    assert(grid != NULL);
    Mat img = (*grid).clone();

    int numVertices = frame->getNumVertices();
    Dot **vertices = frame->getVertices();

    for (int i = 0; i < numVertices; i++)
    {

        Point pt1 = Point(vertices[i]->x * step, vertices[i]->y * step);
        Point pt2 = Point(vertices[(i + 1) % numVertices]->x * step, vertices[(i + 1) % numVertices]->y * step);
        line(img, pt1, pt2, Scalar(0, 0, 255), 2);
    }

    imshow("Piece", img);
    waitKey(0);
}

ostream &operator<<(ostream &os, const Grid &grid)
{

    os << *(grid.grid) << endl;
    return os;
}
