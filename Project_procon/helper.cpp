#include "helper.h"
#include <fstream>

using std::fstream;

#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;
using std::exit;

void parseInput(const char *filePath, Frame *&frame, Piece **&pieces, int &numPieces)
{

    fstream fileData;
    fileData.open(filePath, std::ios::in);
    if (!fileData.is_open())
    {
        cout << "Cannot open " << filePath << endl;
        exit(0);
    }

    fileData >> numPieces;
    // Create array of Pieces
    pieces = new Piece *[numPieces];
    int id;          // id of piece
    int numVertices; // number of vertices of Piece
    // Read data for piece
    for (int i = 0; i < numPieces; i++)
    {

        fileData >> id;
        fileData >> numVertices;
        pieces[i] = new Piece(numVertices, id);

        int x, y; // Coordition of vertice
        for (int j = 0; j < numVertices; j++)
        {

            fileData >> x >> y;
            pieces[i]->setCoord(j, x, y); // set coordition for i th vertice.
        }
    }

    // read data for frame
    fileData >> id >> numVertices;
    frame = new Frame(numVertices, id);

    int x, y; // read coordition for vertices of frame
    for (int i = 0; i < numVertices; i++)
    {

        fileData >> x >> y;
        frame->setCoord(i, x, y);
    }
}
