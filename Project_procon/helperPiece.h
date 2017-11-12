#ifndef HELPERPIECE_H
#define HELPERPIECE_H

#include <cmath>
#include "Dot.h"
#include <iostream>

using std::cout;
using std::endl;

#include <vector>

using std::vector;

int UCLN(int a, int b) {
    
    while (b != 0) {
    
        int tmp = b;
        b = a % b;
        a = tmp;
    }

    return a;
}

bool isSquareNumber(int number) {
    
    int tmp = int(sqrt(number));
    return tmp*tmp == number;
}

// Read here : https://github.com/theiron97/Procon/blob/ver7/cpp/struct_Dot.h
/*
+ vecDotA chứa tọa độ cũ và mới của DotA 
+ vecDotB chứa tọa độ cũ của B và nơi chưa tọa độ mới của B.
-> trả về <= 0 nếu không lắp được 
ngược lại thì lắp được.
*/
int fitPointToEdge(vector<Dot*> &vecDotA, vector<Dot *> &vecDotB, Dot *dotC, Dot *dotD) {

        Dot *dotA = vecDotA[0];
        Dot *newDotA = vecDotA[1];
        Dot *dotB = vecDotB[0];
        Dot *newDotB = vecDotB[1];

        int square_edge_AB = dotA->getSquareEdge(dotB);
        int x_u = dotD->x - dotC->x;
        int y_u = dotD->y - dotC->y;
    
        // He phuong trinh duong tron va duong thang
        if (square_edge_AB * x_u * x_u % (x_u * x_u + y_u * y_u) != 0) return -1;
        int tmp = square_edge_AB * x_u * x_u / (x_u * x_u + y_u * y_u);
        if (!isSquareNumber(tmp)) return -2;
        tmp = sqrt(tmp);
        // 2 truong hop
        // x_u == 0
        if (x_u == 0) {
            //cout << "x_u == 0" << endl;
            newDotB->x = newDotA->x;
    
            if (!isSquareNumber(square_edge_AB)) return -4;
            if (y_u < 0) newDotB->y = newDotA->y - sqrt(square_edge_AB);
            if (y_u > 0) newDotB->y = newDotA->y + sqrt(square_edge_AB);
    
            if (newDotB->x == dotD->x && newDotB->y == dotD->y) return 100;
            return 3;
        }
    
        if (tmp * y_u % x_u != 0) return -3;
    
        int x_B_1 = newDotA->x - tmp;
        int y_B_1 = newDotA->y - (tmp * y_u) / x_u;
        int x_B_2 = newDotA->x + tmp;
        int y_B_2 = newDotA->y + (tmp * y_u) / x_u;
    
        // Can loai 1 truong hop diem B nam ngoai doan DC
        // truong hop nay vector_AB nguoc chieu vs vector_CD : (x_d - x_c) / (x_b - x_a) < 0
        if (x_u * (x_B_1 - newDotA->x) < 0) {
            // Loai B_1
            newDotB->x = x_B_2;
            newDotB->x = y_B_2;
    
            if (x_B_2 == dotD->x && y_B_2 == dotD->y) return 100;
            return 1;
        }
    
        if (x_u * (x_B_2 - newDotA->x) < 0) {
    
            // Loai B_2
            newDotB->x = x_B_1;
            newDotB->y = y_B_1;
            if (x_B_1 == dotD->x && y_B_1 == dotD->y) return 100;
            return 2;
        }
    
        cout << "Vo ly";
        return 0;
}

#endif