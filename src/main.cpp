#include "winsys.h"
#include "snake.h"

int main() {
    CDesktop d;
    //a window which you can input chars into, called with top left corner and bottom right
    d.insert(new CInputLine(CRect(CPoint(5, 7), CPoint(15, 15))));
    //a normal window, called with top left corner and bottom right and a char that serves as a background
    d.insert(new CWindow(CRect(CPoint(2, 3), CPoint(20, 10)), '#'));
    //game window that stores our snake
    d.insert(new CSnake(CRect(CPoint(5, 5), CPoint(45, 20))));
    //main game loop
    d.run();
    return 0;
}
