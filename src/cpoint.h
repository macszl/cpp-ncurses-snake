#ifndef __CPOINT_H__
#define __CPOINT_H__

#include <list>

using namespace std;

struct CPoint
{
    //values informing which column and which row is this point in
    int x;
    int y;
    //constructor that has default values of 0 for x and y
    CPoint(int _x = 0, int _y = 0) : x(_x), y(_y) {};


    CPoint &operator+=(const CPoint &delta) {
        x += delta.x;
        y += delta.y;
        return *this;
    }
};

struct CRect
{
    //the "anchor" of our rectangle
    CPoint topleft;
    //size stores the width and length of the rectangle
    CPoint size;

    //initializing both values using an initializer list
    CRect(CPoint t1 = CPoint(), CPoint s = CPoint()) :
            topleft(t1), size(s) {};
};

#endif
