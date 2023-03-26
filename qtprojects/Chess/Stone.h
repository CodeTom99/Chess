#ifndef STONE_H
#define STONE_H
#include "qpainter.h"
#include <QString>
class Stone
{
public:
    Stone();
    //定义棋子的所有类型
    enum TYPE{WANG,TA,QI,HOU,BING,ZHUJIAO};
    //棋子所处的行
    int _row;
    //棋子所处的列
    int _col;
    //棋子的id
    int _id;
    //棋子是否已死
    bool _dead;
    //棋子是否为红子
    bool _red;
    //棋子类型
    TYPE _type;
    //初始化棋子
    void init(int id);


};

#endif // STONE_H
