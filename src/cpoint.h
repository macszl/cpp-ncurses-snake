#ifndef __CPOINT_H__
#define __CPOINT_H__

#include <list>

using namespace std;

struct CPoint
{

    int x;
    int y;

    CPoint(int _x = 0, int _y = 0) : x(_x), y(_y) {};

    //over
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
