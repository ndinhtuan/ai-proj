#include "Grid.h"
#include "helper.h"
#include "clipper.hpp"

#include <iostream>
#include <deque> 


using std::cout;
using std::endl;



#include <boost/assign/std/vector.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/dsv/write.hpp>

#include <boost/foreach.hpp>

using namespace boost::geometry;
using namespace boost::assign;
using boost::geometry::model::polygon;

#include <cstring>

using std::string;

#define INPUT_DATA "example/data2.txt"

/*
Thêm phần check overlap và ghép hình.
*/

using namespace ClipperLib;

Paths subj;
Paths clip;
Paths solution; 

void addPoint(int x, int y, Path *path)
{
    IntPoint ip;
    ip.X = x;
    ip.Y = y;
    path->push_back(ip);
}

int main(int argc, char** argv) {

    Frame *frame;
    Piece **pieces;
    int numPieces;
    cout << "Reading data..";
    parseInput(INPUT_DATA, frame, pieces, numPieces);
    cout << " Done!" << endl;

    Grid grid(70, 110, 10);
    vector<Frame *> vectFrame;

    //Piece *newRotatedPiece = new Piece(*pieces[1]);
    //grid.showPiece(pieces[1]);
    vector<Frame*> vectPiece;
    
    for (int i = 0; i < numPieces; i++) {

        vectPiece.push_back(pieces[i]);
    }

    typedef boost::geometry::model::d2::point_xy<int> point_xy;
    vector<point_xy> points0, points1; // Tạo 2 vector points ứng với 2 Piece.

    // tạo polygon 1 từ piece this.
    for (int i = 0; i < pieces[0]->getNumVertices(); i++) {
        points0 += point_xy(pieces[0]->getVertices()[i]->x, pieces[0]->getVertices()[i]->y);
    }
    points0 += point_xy(pieces[0]->getVertices()[0]->x, pieces[0]->getVertices()[0]->y);
    boost::geometry::model::polygon<point_xy> polygon0;
    boost::geometry::assign_points(polygon0, points0);
    float area1 = boost::geometry::area(polygon0);
    cout << "Area0 " << boost::geometry::area(polygon0) << std::endl;
    boost::geometry::correct(polygon0);
    cout << *pieces[0];
    cout << boost::geometry::dsv(polygon0) << std::endl;

    // tạo polygon 2 từ piece otherPiece 
    for (int i = 0; i < pieces[1]->getNumVertices(); i++) {

        points1 += point_xy(pieces[1]->getVertices()[i]->x, pieces[1]->getVertices()[i]->y);
    }
    points1 += point_xy(pieces[1]->getVertices()[0]->x, pieces[1]->getVertices()[0]->y);
    boost::geometry::model::polygon<point_xy> polygon1;
    boost::geometry::assign_points(polygon1, points1);
    cout << "Area1 " << boost::geometry::area(polygon1) << std::endl;
    boost::geometry::correct(polygon1);
    cout << *pieces[1];
    cout <<  boost::geometry::dsv(polygon1) << std::endl;

    std::vector<polygon<point_xy> > output;
    boost::geometry::union_(polygon0, polygon1, output);

    // int i = 0;
    // std::cout << "green || blue:" << std::endl;
    // BOOST_FOREACH(polygon<point_xy> const& p, output)
    // {
    //     int a = p.outer()[0].x();
    //     std::cout << a;
    //     std::cout << i++ << ": " << boost::geometry::dsv(p) 
    //                 << endl << p.outer().size() << std::endl;
    // }

    Piece *result = new Piece(output[0].outer().size() - 1, 0);
    
    for (int i = 0; i < output[0].outer().size() - 1; i++) {
        result->setCoord(i, output[0].outer()[i].x(), output[0].outer()[i].y());
    }
    cout << "RESULT : " << *result << endl; 
    result->eliminate3DotInLine();
    cout << "RESULT : " << *result << endl; 

    grid.showPieceSameTime(vectPiece);
    grid.showPiece(result);
    return 0;
}