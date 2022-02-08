#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <iostream>
#include <vector>
#include "winsys.h"
#include "screen.h"
#include "cpoint.h"
#include <map>
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

    CPoint food_loc;

    std::size_t level;
    std::size_t highscore;

    std::vector<CPoint> snake_body;
public:
    CSnake(CRect r, char _c = ' ');

    bool handleEvent(int key) override;
    void paint() override;
    void moveWindow(int key);
    void moveSnake();
    void updateSnakeBody(const CPoint & new_point, int size);
    void checkForFoodOrBodyHit(const CPoint & point);
    void respawnFood();

    bool isAllowedTurn(int key) const;
    bool isHittingWall() const;
    bool isHittingOccupied( const CPoint & point) const;
    bool isHittingFood(const CPoint & point) const;
    bool isSpeedingUp(int key) const;

    void drawFood();
    void drawSnake();
    void drawHelp();
    void drawPauseInfo();
    void drawKilledInfo();

    void snakeKill();
    void snakeGrow();

    void restart();

};

#endif
