/*
- Class describe coordion Decartion Oxy
*/
#ifndef DOT_H
#define DOT_H

struct Dot{
    int x;
    int y;

    int getSquareEdge(const Dot *dot2) {
        
        return (this->x - dot2->x) * (this->x - dot2->x) + (this->y - dot2->y) * (this->y - dot2->y);
    }

    Dot(){

    }

    Dot(int x, int y) {
        this->x = x;
        this->y = y;
    }
};
#endif
