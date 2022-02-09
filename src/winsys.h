#ifndef __WINSYS_H__
#define __WINSYS_H__

#include "cpoint.h"
#include <list>

#include <string>
//#include <ctype.h>
//#include <stdarg.h>
using namespace std;

//this class is basically an interface
//all the necessary functions are defined by other classes
//except for the CRect constructor which stores the geometry of the windows
class CView {
protected:
    CRect geom;

public:
    CView(CRect g) : geom(g) {}

    virtual ~CView() {}

    virtual void paint() = 0;

    virtual bool handleEvent(int key) = 0;

    virtual void move(const CPoint &delta);
};

//CWindow defines some of the virtual functions that cview had
class CWindow : public CView {
protected:
    //the "filling" used by paint
    char c;
public:
    //constructor which specifies the geometry and the char of the filling
    //default value of the char is '*'
    CWindow(CRect r , char _c = '*') : CView(r) , c(_c) {}
    //painting each and every row and column of the window with the c character
    void paint();
    //moving the window
    bool handleEvent(int key);
};

class CFramedWindow : public CWindow {
public:
    //constructor which specifies the geometry of the window and the char of the filling
    //default value of the char is '\'
    CFramedWindow(CRect r , char _c = '\'') : CWindow(r , _c) {}

    void paint();
};

class CInputLine : public CFramedWindow {
    string text;
public:
    //constructor which specifies the geometry of the window and the char of the filling
    //default value of the char is ','
    CInputLine(CRect r , char _c = ',') : CFramedWindow(r , _c) {}

    void paint();

    bool handleEvent(int c);
};

class CGroup : public CView {
    list<CView *> children;
public:
    //constructor which specifies the geometry of the window
    CGroup(CRect g) : CView(g) {}

    ~CGroup();

    void paint();

    bool handleEvent(int key);

    void insert(CView *v);
};

class CDesktop : public CGroup {
public:
    CDesktop();

    ~CDesktop();

    void paint();

    int getEvent();

    void run();
};


#endif
