/*
- Class Grid tạo class để mô phỏng Piece và Frame trên Opencv
*/

#ifndef GRID_H
#define GRID_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;

#include "Frame.h"

class Grid
{

  public:
    Grid();
    /*
    - Khởi tạo Grid với 
    @height : chiều cao của bảng mô phỏng 
    @Width : chiều dài của bảng mô phỏng 
    @step : độ dài mỗi cell
    */
    Grid(int height, int width, int step);
    ~Grid();

    void showPiece(const Frame *frame);
    friend ostream &operator<<(ostream &os, const Grid &grid);

  private:
    Mat *grid;
    int height, width, step;
};
#endif
