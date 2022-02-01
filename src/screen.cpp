/* screen.cpp */
#include "screen.h"
#include "cpoint.h"

#define LINE_MAX 255

static CPoint TopLeft;
static CPoint BottomRight;
static int cury, curx;

void init_screen() {
    //typical ncurses initialization functions

    //initializing the screen
    initscr();
    //disables line buffering - you don't need to press enter to transfer the characters to a program
    cbreak();
    //user doesn't see the typed input
    noecho();
    //timeout serves as the update speed of the whole window system
    timeout(20);
    //ncurses translating the enter key into something different than '\n'
    nonl();
    //function reducing the need for cursor movements
    //usually whenever ncurses functions refresh the windows they need to redraw the cursors
    //this function makes it so that the refresh functions don't redraw the cursor
    leaveok(stdscr, TRUE);
    //flushing the buffer whenever a SIGSTOP, SIGINT or quitting happens
    //set to false
    intrflush(stdscr, FALSE);
    //unlocking the arrow keys
    keypad(stdscr, TRUE);
    //setting the cursor mode to invisible
    curs_set(0);
}

void done_screen() {
    //freeing all memory related to the ncurses terminal
    endwin();
}

void update_screen() {
    //function used to check whether the window has been resized in any way

    //gets the origin cursor
    getscreenanchor(TopLeft.y, TopLeft.x);
    //gets the current possible screen size
    getscreensize(BottomRight.y, BottomRight.x);
    BottomRight += TopLeft;
}

int ngetch() {
    //wrapper for the ncurses based function getch
    return wgetch(stdscr);
}

void getscreenanchor(int &y, int &x) {
    //ncurses function, gets the origin position of the window(the top left corner)
    getbegyx(stdscr, y, x);
}

void getscreensize(int &y, int &x) {
    //ncurses function, gets the the number of rows and columns the function can take
    //stores them in y and x
    getmaxyx(stdscr, y, x);
}

int gotoyx(int y, int x) {
    //changes the cursor positions
    cury = y;
    curx = x;

    //???
    return (cury >= TopLeft.y && cury < BottomRight.y && curx >= TopLeft.x && curx < BottomRight.x);
}
//self explanatory
void getcursor(int &y, int &x) {
    y = cury;
    x = curx;
}

void printc(char c) {
    //prints out a char if the current y and x allows it
    if (c != '\n' && c != '\r')
    {
        if (cury >= TopLeft.y && cury < BottomRight.y && curx >= TopLeft.x && curx < BottomRight.x)
        {
            //moves cursor
            wmove(stdscr, cury, curx);
            //adds char
            addch(c);
        }
        //increments the x variable of the cursor
        curx++;
    }
}

int printl(const char *fmt, ...) {
    //wrapper for the ncurses based printline function

    //starting up the variable argument list
    va_list ap;
    va_start(ap, fmt);


    char dest[LINE_MAX];
    //Write formatted data from variable argument list to sized buffer
    vsnprintf(dest, LINE_MAX, fmt, ap);

    int i = 0;
    //Prints out each char from the buffer
    while (dest[i])
        printc(dest[i++]);

    va_end(ap);
    return !ERR;
}
