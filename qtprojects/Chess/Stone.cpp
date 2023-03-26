#include "Stone.h"

Stone::Stone()
{

}

void Stone::init(int id)
{
    // 每方总共有16个棋子
    struct
    {
        int row,col;
        Stone::TYPE type;
    } pos[32] = {
    {0,0,Stone::TA},
    {0,1,Stone::QI},
    {0,2,Stone::ZHUJIAO},
    {0,3,Stone::HOU},
    {0,4,Stone::WANG},
    {0,5,Stone::ZHUJIAO},
    {0,6,Stone::QI},
    {0,7,Stone::TA},

    {1,0,Stone::BING},
    {1,1,Stone::BING},
    {1,2,Stone::BING},
    {1,3,Stone::BING},
    {1,4,Stone::BING},
    {1,5,Stone::BING},
    {1,6,Stone::BING},
    {1,7,Stone::BING},
 //------------------------
    {7,0,Stone::TA},
    {7,1,Stone::QI},
    {7,2,Stone::ZHUJIAO},
    {7,3,Stone::HOU},
    {7,4,Stone::WANG},
    {7,5,Stone::ZHUJIAO},
    {7,6,Stone::QI},
    {7,7,Stone::TA},

    {6,0,Stone::BING},
    {6,1,Stone::BING},
    {6,2,Stone::BING},
    {6,3,Stone::BING},
    {6,4,Stone::BING},
    {6,5,Stone::BING},
    {6,6,Stone::BING},
    {6,7,Stone::BING},
    };

    _row = pos[id].row;
    _col = pos[id].col;
    _type = pos[id].type;

    _id = id;
    _dead = false;
    _red = id<16; //id 小于16时，bool _red为真

}
