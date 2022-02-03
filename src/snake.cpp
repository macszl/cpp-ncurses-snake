#include "snake.h"

bool is_arrow_key(int c)
{
    switch (key) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_LEFT:
            return true;
        default:
            return false;
    };
}
CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c)
{
    highscore = 0;
    level = 0;
    dir = DIR_RIGHT;
    status = PAUSED_HELP_MODE;
    respawnFood();
    //snake_body.front() will be our head

    //reserving space for at least 4 starting elements and then using push_back
    snake_body.reserve(4)
    //the starting 4 points of the snake are predetermined
    snake_body.push_back(CPoint(2, 5));
    snake_body.push_back(CPoint(2, 4));
    snake_body.push_back(CPoint(2, 3));
    snake_body.push_back(CPoint(2, 2));

}

bool CSnake::handleEvent(int key)
{
    if( key == 'p' && status == PAUSED_MODE)
    {
        status = PLAY_MODE;
        return true;
    }
    else if( key == 'h' && status == PAUSED_HELP_MODE)
    {
        status = PAUSED_MODE;
        return true;
    }
    else if( key == 'r')
    {
        restart();
        return true;
    }
    else if(is_arrow_key(key) && status != PLAY_MODE)
    {
        moveWindow(key)
        return true;
    }
    else if(is_arrow_key(key) && status == PLAY_MODE)
    {
        moveSnake(key)
        return true;
    }
    else
    {
        return false;
    }
}



void CSnake::moveWindow(int key)
{
    switch (key) {
        case KEY_UP:

            return;
        case KEY_DOWN:

            return;
        case KEY_RIGHT:

            return;
        case KEY_LEFT:

            return;
        default:
            return;
    };
}
void CSnake::moveSnake(int key)
{
    switch (key) {
        case KEY_UP:

            return;
        case KEY_DOWN:

            return;
        case KEY_RIGHT:

            return;
        case KEY_LEFT:

            return;
        default:
            return;
    };
}
void CSnake::respawnFood()
{
    food_loc.x = rand() % geom.size.x;
    food_loc.y = rand() % geom.size.x;

    if (isHittingOccupied(CPoint( rand_y, rand_x)))
    {
        respawnFood();
    }
}

bool CSnake::isAllowedTurn(int key)
{
    if(dir == DIR_RIGHT && key == KEY_LEFT)
    {
        return false;
    }
    else if( dir == DIR_UP && key == KEY_DOWN)
    {
        return false;
    }
    else if( dir == DIR_LEFT && key == KEY_RIGHT)
    {
        return false;
    }
    else if( dir == DIR_DOWN && key == KEY_UP)
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool CSnake::isPassingWall()
{

}
bool CSnake::isHittingOccupied( const CPoint & point)
{
    for(int i = 0; i < snake_body.size(); i++)
    {
        if( snake_body[i].x == point.x && snake_body[i].y == point.y)
        {
            return true;
        }
    }
    return false;
}
bool CSnake::isHittingFood()
{
    if( snake_body.front().x == food_loc.x && snake_body.front().y == food_loc.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSnake::drawFood()
{

}
void CSnake::drawSnake()
{

}

void CSnake::snakeKill()
{

}

void CSnake::restart()
{
    highscore = 0;
    level = 0;
    dir = DIR_RIGHT;
    respawnFood();
    //snake_body.front() will be our head

    //reserving space for at least 4 starting elements and then using push_back
    snake_body.reserve(4)
    //the starting 4 points of the snake are predetermined
    snake_body.push_back(CPoint(2, 5));
    snake_body.push_back(CPoint(2, 4));
    snake_body.push_back(CPoint(2, 3));
    snake_body.push_back(CPoint(2, 2));

}
