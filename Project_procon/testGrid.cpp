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

    Paths subj;
    
    Path p;
    addPoint(100,100, &p);
    addPoint(200,100, &p);
    addPoint(200,200, &p);
    addPoint(100,200, &p);
    subj.push_back(p);

    typedef boost::geometry::model::d2::point_xy<int> point_xy;
    vector<point_xy> points[2];

    Frame *frame;
    Piece **pieces;
    int numPieces;
    cout << "Reading data..";
    parseInput(INPUT_DATA, frame, pieces, numPieces);
    cout << " Done!" << endl;

    Grid grid(70, 110, 10);
    vector<Frame *> vectFrame;

    string hCoord[2] = {"POLYGON((", "POLYGON(("};

    for (int i = 0; i < numPieces; i++) {
        vectFrame.push_back(pieces[i]);

        for (int j = 0; j < pieces[i]->getNumVertices(); j++) {
            points[i] += point_xy(pieces[i]->getVertices()[j]->x, 
                                    pieces[i]->getVertices()[j]->y);
        }
    }

    boost::geometry::model::polygon<point_xy> polygon1;
    boost::geometry::assign_points(polygon1, points[0]);
    cout << boost::geometry::dsv(polygon1) << " Area : " << boost::geometry::area(polygon1) << endl;

    boost::geometry::model::polygon<point_xy> polygon2;
    boost::geometry::assign_points(polygon2, points[1]);
    cout << boost::geometry::dsv(polygon2) << " Area : " << boost::geometry::area(polygon2) << endl;

    grid.showPiece(pieces[0]);
    grid.showPiece(pieces[1]);
    grid.showPieceSameTime(vectFrame);

    //if (overlaps(polygon1, polygon2)) cout << "OVERLAP";

    std::deque<polygon<point_xy> > output;
    boost::geometry::intersection(polygon1, polygon2, output);

    if (output.size() <= 0) cout << "Not overlap";
    else {
        cout << "OVERLAP";
    }


    boost::geometry::union_(polygon1, polygon2, output);

    
    return 0;
}
