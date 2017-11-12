#include "Dot.h"
#include <iostream>

using std::ostream;

ostream& operator<<(ostream &os, const Dot &dot) {
    os << "(" << dot.x << "; " << dot.y << ")";
    return os;
}