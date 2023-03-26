#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Stone.h"
#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);//explicit 是避免构造函数的参数自动转换为类对象的标识符
    ~MainWindow();  //析构函数主要作用就是释放资源,避免内存泄漏。在QT里无析构函数无法运行
    Stone _s[32];     // 32个棋子
    int _selectid;    // 被选中的棋子
    bool _bRedTurn;   // 轮到哪方走
    bool _bSide=true;     //判断属于哪个方向（上或下），在Stone.cpp里设定上方棋子的bool _red为真
    int theLastStep[8][8];//=10的话，标记移动前的格子
    int canSelect[8][8];//=10的话，标记能走的格子
    int sendKing1=0;//送王三次则判负
    int sendKing2=0;//送王三次则判负
    bool enpt=false; // 吃过路兵判断
    QPoint enptP;  //   吃过路兵的坐标
    bool canChange=true;//王车易位判断
    QPoint ptW,ptC;//王的位置和将军之棋的位置
    bool isCheck=false;
    int changeT1=0;
    int changeT2=0; //上下方各一次机会王车易位
    int _raiseid1,_raiseid2; //兵升变id
//--------------------------功能型函数-----------------------------------------
    // 根据行和列获取棋子的id
    int getStoneId(int row, int col);
    // 判断棋子的颜色是否相同
    bool sameColor(int moveid,int killid);
    //获取棋子位置与点击位置是否是直线且该直线有多少棋子的函数
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    //获取棋子位置与点击位置是否是斜线线且其中有多少棋子的函数
    int getStoneCountObliqueLine(int row1, int col1, int row2, int col2);
    //获取两点之间的行列点位差
    int relation(int row1, int col1, int row, int col);
    //判断选择的棋在上方或是下方
    bool isTopSide(int id);
    //显示可走位置
    void showSelect(int id,int row,int col);
    //判断王车易位所经过的格子是否安全
    bool safeforChange();
//------------------象棋规则函数-----------------------------------------------
    //最基本的能不能走的判断
    bool canMove(int moveid,int killid,int row,int col);
    //判断王能不能走
    bool canMoveWANG(int moveid,int killid,int row,int col);
    //判断后能不能走
    bool canMoveHOU(int moveid,int killid,int row,int col);
    //判断主教能不能走
    bool canMoveZHUJIAO(int moveid,int killid,int row,int col);
    //判断塔能不能走
    bool canMoveTA(int moveid,int killid,int row,int col);
    //判断骑能不能走
    bool canMoveQI(int moveid,int killid,int row,int col);
    //判断兵能不能走
    bool canMoveBING(int moveid,int killid,int row,int col);
    //判断王的行走位置是否是敌对棋子的攻击空间
    bool safeForWANG(int moveid,int row,int col);
    //判断是否将军
    bool checkMate(int moveid,int row,int col);
    //判断是否应将
    bool resCheck(int row,int col);
//---------------------与显示到窗口中有关的函数--------------------------------------

    void drawStone(QPainter& p,int id);
    //输入棋子的id 返回像素坐标
    QPoint center(int id);
    //输入行列坐标 返回像素坐标
    QPoint center(int row,int col);
//---------------------与走棋和吃子有关的函数--------------------------------------
    //鼠标点击获取坐标，鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *ev);
    //是否能点击该区域
    bool getRowCol(QPoint pt, int &row, int &col);
    //点击函数1由鼠标点击坐标点得到对应棋子的坐标点
    void click(QPoint pt);
    //点击函数2点击棋子的编号和坐标尝试走棋
    void click(int id, int row, int col);
    //尝试走棋函数
    void tryMoveStone(int killid, int row, int col);
    //走棋与吃棋函数
    void moveStone(int _selectid, int killid, int row, int col);
    //能否选择函数
    void trySelectStone(int id);
protected://只允许子类及本类的成员函数访问
    void paintEvent(QPaintEvent *);
private:
    Ui::MainWindow *ui;
private:
    short size;
    QImage pic[32];

};
#endif // MAINWINDOW_H
