#include <unistd.h>
#include <utility>
#include <algorithm>
#include <string_view>
#include "snake.h"

bool is_arrow_key(int c)
{
    switch (c) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_LEFT:
            return true;
        default:
            return false;
    }

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
    snake_body.reserve(4);
    //the starting 4 points of the snake are predetermined
    snake_body.emplace_back(2, 8);
    snake_body.emplace_back(2, 7);
    snake_body.emplace_back(2, 6);
    snake_body.emplace_back(2, 5);

}

bool CSnake::handleEvent(int key)
{
    std::map<int, snake_direction_t> key_enum_pairs= { {KEY_RIGHT, DIR_RIGHT},
                                                       {KEY_LEFT,  DIR_LEFT},
                                                       {KEY_DOWN,  DIR_DOWN},
                                                       {KEY_UP,    DIR_UP}};
    std::map<int, snake_direction_t>::iterator key_enum_pair_iterator;

    if(status == PLAY_MODE) {
        tick += 20;
    }
    if(is_arrow_key(key) && status == PLAY_MODE)
    {
        key_enum_pair_iterator = key_enum_pairs.find(key);
        if(isAllowedTurn(key))
            dir = key_enum_pair_iterator->second;
        moveSnake();
        tick = 0;
        return true;
    }
    else if(is_arrow_key(key) && status != PLAY_MODE)
    {
        moveWindow(key);
        return true;
    }
    else if( key == 'p' && status == PAUSED_MODE)
    {
        status = PLAY_MODE;
        return true;
    }
    else if( ( key == 'p' && status == PLAY_MODE )|| (key == 'h' && status == PAUSED_HELP_MODE))
    {
        status = PAUSED_MODE;
        return true;
    }
    else if( key == 'h' && (status == PAUSED_MODE || status == PLAY_MODE ))
    {
        status = PAUSED_HELP_MODE;
        return true;
    }
    else if( key == 'r')
    {
        restart();
        return true;
    }


    return true;

}

void CSnake::paint()
{
    CFramedWindow::paint();
    gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x);
    printl("Total score %d || Current level %d", highscore, level);
    level = highscore / 3 + 1;
    if( level > 10) level = 10;

    switch(status)
    {
        case PLAY_MODE:{
            drawSnake();
            drawFood();
            if(tick >= (200 - (level * 16)) ) {
                moveSnake();
                tick = 0;
            }
            return;
        }
        case PAUSED_MODE: {
            drawSnake();
            drawFood();
            drawPauseInfo();
            return;
        }
        case PAUSED_HELP_MODE: {
            drawSnake();
            drawFood();
            drawHelp();
            return;
        }
        case DEAD_MODE: {
            drawSnake();
            drawFood();
            drawKilledInfo();
            return;
        }
    }
}

void CSnake::moveWindow(int key)
{
    switch (key) {
        case KEY_UP: {
            geom.topleft.y -= 1;
            return;
        }
        case KEY_DOWN: {
            geom.topleft.y += 1;
            return;
        }
        case KEY_RIGHT: {
            geom.topleft.x += 1;
            return;
        }
        case KEY_LEFT: {
            geom.topleft.x += -1;
            return;
        }
        default:
            return;
    }
}
void CSnake::moveSnake()
{
    int size_pre_food = snake_body.size();
    switch (dir) {
        case DIR_UP: {
            CPoint new_point;
            if( isHittingWall())
                new_point = CPoint(snake_body.front().x, (geom.size.y - 2));
            else
                new_point = CPoint(snake_body.front().x, (snake_body.front().y - 1));


            checkForFoodOrBodyHit(new_point);
            updateSnakeBody(new_point, size_pre_food);
            return;
        }
        case DIR_DOWN: {
            CPoint new_point;
            if (isHittingWall())
                new_point = CPoint(snake_body.front().x, 1);
            else
                new_point = CPoint(snake_body.front().x, (snake_body.front().y + 1));

            checkForFoodOrBodyHit(new_point);
            updateSnakeBody(new_point, size_pre_food);
            return;
        }
        case DIR_RIGHT: {
            CPoint new_point;
            if (isHittingWall())
                new_point = CPoint(1 , snake_body.front().y);
            else
                new_point = CPoint( (snake_body.front().x + 1), snake_body.front().y);

            checkForFoodOrBodyHit(new_point);
            updateSnakeBody(new_point, size_pre_food);
            return;
        }
        case DIR_LEFT: {
            CPoint new_point;
            if (isHittingWall())
                new_point = CPoint((geom.size.x - 2), snake_body.front().y);
            else
                new_point = CPoint((snake_body.front().x - 1), snake_body.front().y);

            checkForFoodOrBodyHit(new_point);
            updateSnakeBody(new_point, size_pre_food);
            return;
        }
        default:
            return;
    }

}
void CSnake::updateSnakeBody(const CPoint & new_point, int size)
{
    for(int i = size - 1; i > 0; i--)
    {
        std::swap( snake_body[i], snake_body[i - 1]);
    }
    snake_body.front() = new_point;

}
void CSnake::respawnFood()
{
    food_loc.x = 1 + rand() % (geom.size.x - 2);
    food_loc.y = 1 + rand() % (geom.size.y - 2);

    if (isHittingOccupied(CPoint( food_loc.x, food_loc.y)))
    {
        respawnFood();
    }
}

bool CSnake::isAllowedTurn(int key) const
{
    if((dir == DIR_RIGHT && key == KEY_LEFT)
       || ( dir == DIR_UP   && key == KEY_DOWN)
       || ( dir == DIR_LEFT && key == KEY_RIGHT)
       || ( dir == DIR_DOWN && key == KEY_UP))
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool CSnake::isHittingWall() const
{
    if((dir == DIR_UP    && snake_body.front().y == 1 )
       || (dir == DIR_DOWN  && snake_body.front().y == geom.size.y - 2)
       || (dir == DIR_LEFT  && snake_body.front().x == 1 )
       || (dir == DIR_RIGHT && snake_body.front().x == geom.size.x - 2) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CSnake::isHittingOccupied( const CPoint & point) const
{
    for(auto & i : snake_body)
    {
        if( i.x == point.x && i.y == point.y)
        {
            return true;
        }
    }
    return false;
}
bool CSnake::isHittingFood( const CPoint & point) const
{
    if( point.x == food_loc.x && point.y == food_loc.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CSnake::isSpeedingUp(int key) const
{
    if( (key == KEY_RIGHT && dir == DIR_RIGHT)
        ||  (key == KEY_LEFT  && dir == DIR_LEFT)
        ||  (key == KEY_UP    && dir == DIR_UP)
        ||  (key == KEY_DOWN  && dir == DIR_DOWN))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSnake::checkForFoodOrBodyHit(const CPoint & point)
{
    if(isHittingOccupied(point))
    {
        snakeKill();
        return;
    }

    if(isHittingFood(point))
    {
        highscore++;
        respawnFood();
        snakeGrow();
        return;
    }
}
void CSnake::drawFood()
{
    gotoyx(geom.topleft.y + food_loc.y, geom.topleft.x + food_loc.x);
    printc('*');
}
void CSnake::drawSnake()
{
    for(auto & i : snake_body)
    {
        gotoyx(geom.topleft.y + i.y, geom.topleft.x + i.x);
        printc('o');
    }
}

void CSnake::drawHelp()
{

    gotoyx( geom.topleft.y + geom.size.y / 2 - 2, geom.topleft.x + geom.size.x / 4 - 7);
    printl("HOTKEY INFO");
    gotoyx( geom.topleft.y + geom.size.y / 2 - 1, geom.topleft.x + geom.size.x / 4 - 7);
    printl("H to display or clear help information");
    gotoyx( geom.topleft.y + geom.size.y / 2,     geom.topleft.x + geom.size.x / 4 - 7);
    printl("P to pause, arrow keys to move");
    gotoyx( geom.topleft.y + geom.size.y / 2 + 1, geom.topleft.x + geom.size.x / 4 - 7);
    printl("R to restart.");
    gotoyx( geom.topleft.y + geom.size.y / 2 + 2, geom.topleft.x + geom.size.x / 4 - 7);
    printl("Q to quit.");
    drawPauseInfo();
}

void CSnake::drawPauseInfo()
{
    gotoyx( geom.topleft.y + geom.size.y / 2 + 3, geom.topleft.x + geom.size.x / 4 - 7);
    printl("Currently paused. Press P to unpause.");
}

void CSnake::drawKilledInfo()
{
    gotoyx( geom.topleft.y + geom.size.y + 1, geom.topleft.x);
    printl("|....Game over....|");
}
void CSnake::snakeKill()
{
    status = DEAD_MODE;
}

void CSnake::snakeGrow()
{
    int last_index = snake_body.size() - 1;
    snake_body.push_back( snake_body[last_index]);
}

void CSnake::restart()
{
    highscore = 0;
    level = 0;
    dir = DIR_RIGHT;
    respawnFood();
    //snake_body.front() will be our head
    status = PLAY_MODE;

    snake_body.clear();
    //reserving space for at least 4 starting elements and then using push_back
    snake_body.reserve(4);
    //the starting 4 points of the snake are predetermined
    snake_body.emplace_back(2, 8);
    snake_body.emplace_back(2, 7);
    snake_body.emplace_back(2, 6);
    snake_body.emplace_back(2, 5);

}
