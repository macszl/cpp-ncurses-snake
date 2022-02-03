#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"

typedef enum snake_window_status_t {
    PLAY_MODE = 0,
    PAUSED_MODE,
    PAUSED_HELP_MODE,
    DEAD_MODE
} snake_window_status_t;

typedef enum snake_direction_t
{
    DIR_RIGHT,
    DIR_LEFT,
    DIR_UP,
    DIR_DOWN
} snake_direction_t;
class CSnake : public CFramedWindow
{
private:
    snake_window_status_t status;
    snake_direction_t dir;

    point_t food_loc;

    std::size_t level;
    std::size_t highscore;

    std::vector<CPoint> snake_body;
public:
    CSnake(CRect r, char _c = ' ');

    bool handleEvent(int key);
    void moveWindow(int key);
    void moveSnake(int key);
    void respawnFood();

    bool isAllowedTurn(int key);
    bool isPassingWall();
    bool isHittingOccupied( const CPoint & point);
    bool isHittingFood();

    void drawFood();
    void drawSnake();

    void snakeKill();

    void restart();
};

#endif
