#include "winsys.h"
#include "cpoint.h"
#include "screen.h"

#include <list>
#include <string>
#include <ctype.h>
#include <stdarg.h>

using namespace std;

//moving a window
//the functionality of moving a window stays the same no matter which class is used
//so no function overloads this
void CView::move(const CPoint &delta) {
    geom.topleft += delta;
}

//painting each and every row and column of the window with the c character
void CWindow::paint() {
    for (int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++)
        for (int x = geom.topleft.x; x < geom.topleft.x + geom.size.x; x++) {
            gotoyx(y, x);
            printl("%c", c);
        }
}

//calling the move function depending on which key was pressed
bool CWindow::handleEvent(int key) {
    switch (key) {
        case KEY_UP:
            move(CPoint(0, -1));
            return true;
        case KEY_DOWN:
            move(CPoint(0, 1));
            return true;
        case KEY_RIGHT:
            move(CPoint(1, 0));
            return true;
        case KEY_LEFT:
            move(CPoint(-1, 0));
            return true;
    };
    return false;
}

//painting a frame
void CFramedWindow::paint() {
    for (int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++) {
        if((y == geom.topleft.y) || (y == geom.topleft.y + geom.size.y - 1)) {
            //painting the horizontal paths or the corners
            for (int x = geom.topleft.x; x < geom.topleft.x + geom.size.x; x++) {
                gotoyx(y, x);
                if((x == geom.topleft.x) || (x == geom.topleft.x + geom.size.x - 1))
                    printl("+");
                else
                    printl("-");
            }
        } else {
            //painting the vertical parts
            //or the inside parts
            for (int x = geom.topleft.x; x < geom.topleft.x + geom.size.x; x++) {
                gotoyx(y, x);
                if((x == geom.topleft.x) || (x == geom.topleft.x + geom.size.x - 1))
                    printl("|");
                else
                    printl("%c", c);
            }
        }
    }
}

void CInputLine::paint() {
    CFramedWindow::paint();

    unsigned int row = 0;
    for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++, row++) {
        unsigned int letter = row * (geom.size.x - 2);
        for (int x = geom.topleft.x + 1; x < geom.topleft.x + geom.size.x - 1; x++, letter++) {
            gotoyx(y, x);
            printl("%c", letter < text.size() ? text[letter] : c);
        }
    }
}


bool CInputLine::handleEvent(int c) {
    //if an arrow key keystroke happens, handleEvent of CFramedWindow will return true, since the window will be moved
    //CFramedWindow inherits the handleEvent from CWindow
    if(CFramedWindow::handleEvent(c))
        return true;
    if((c == KEY_DC) || (c == KEY_BACKSPACE)) {
        if(text.length() > 0) {
            text.erase(text.length() - 1);
            return true;
        };
    }
    if((c > 255) || (c < 0))
        return false;
    if(!isalnum(c) && (c != ' '))
        return false;
    text.push_back(c);
    return true;
}

//painting out the group space with dots
//and then calling the paint() function for every part of the list
void CGroup::paint() {
    for (int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++) {
        gotoyx(y, geom.topleft.x);
        for (int x = 0; x < geom.size.x; x++)
            printl(".");
    }

    for (list<CView *>::iterator i = children.begin(); i != children.end(); i++)
        (*i)->paint();
}

bool CGroup::handleEvent(int key) {
    //checking if a keystroke was accepted by the subordinate structures
    if(!children.empty() && children.back()->handleEvent(key))
        return true;
    //switching between the windows using the tab key
    if(key == '\t') {
        //if the user pressed tab and the group is not empty,
        //the function copies the last element into the front
        //and deletes the last element
        //children.back() is the active window
        if(!children.empty()) {
            children.push_front(children.back());
            children.pop_back();
        };
        return true;
    }
    return false;
}

//inserting a window into the group
void CGroup::insert(CView *v) {
    children.push_back(v);
}

//freeing all memory related to the windows
CGroup::~CGroup() {
    for (list<CView *>::iterator i = children.begin();
         i != children.end(); i++)
        delete (*i);
}


//the CDesktop constructor
//basically starts up the screen
CDesktop::CDesktop() : CGroup(CRect()) {
    init_screen();
    update_screen();
}

//deallocation of the memory
CDesktop::~CDesktop() {
    done_screen();
}

//the paint function for cdesktop
//used for filling out the background with dots
void CDesktop::paint() {
    getscreensize(geom.size.y, geom.size.x);

    for (int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++) {
        gotoyx(y, geom.topleft.x);
        for (int x = 0; x < geom.size.x; x++)
            printl(".");
    }

    CGroup::paint();
}

//wrapper for wgetch()
int CDesktop::getEvent() {
    return ngetch();
}


//main game loop
//updates screen,
void CDesktop::run() {
    update_screen();
    paint();
    refresh();

    while (1) {
        //the function gets a keystroke from the player, if the player doesnt press anything it returns -1
        int c = getEvent();
        flushinp();
        //user has quit the program
        if(c == 'q' || c == 'Q')
            break;

        //KEY_RESIZE is a predefined ncurses keycode
        //if it is detected by the program
        if(c == KEY_RESIZE || handleEvent(c)) {
            update_screen();
            paint();
            refresh();
        }
    }
}

