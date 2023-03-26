#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QPalette>
#include <QMouseEvent>
#include <QLabel>
#include <QMessageBox>

//通过id获取行列，与bool类型的获取函数相区别
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//设定窗口信息
    //this 就代表指向该函数所作用的对象的指针，访问成员变量或成员函数。
    ui->setupUi(this);
    size = 80;
    this->setWindowTitle(tr("Chess byMLZ"));
    this->setFixedSize(size*10,size*10);
    this->move(400,0);
//初始32颗棋子的信息，由于是交换着对战，所以实际上只有一个棋盘对象
//_selectid表示当前选中的棋子坐标；_bRedTurn为真表示下方走棋，为否表示上方走棋
    for(int i=0;i<32;i++)
        {
            _s[i].init(i);
        }
        _selectid = -1;
        _bRedTurn = true;
        update();
}
MainWindow::~MainWindow()
{
    delete ui;
}
//绘制棋盘
void MainWindow::paintEvent(QPaintEvent *)
{
    setPalette(QPalette(Qt::lightGray));
    setAutoFillBackground(true);
    QPainter p;
    //开始绘制
    p.begin(this);
    // 设置画笔
    QPen pen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    p.drawPoint(size-5,size-5);
    p.drawPoint(size*9+5,size-5);
    p.drawPoint(size-5,size*9+5);
    p.drawPoint(size*9+5,size*9+5);
    //设置画笔颜色和宽度
    QPen background=p.pen();
    background.setColor(Qt::black);
    background.setWidth(7);
    p.setPen(background);
    //画棋盘的轮廓线
    p.drawLine(size-60,size-60,size*9+60,size-60);//上横
    p.drawLine(size-60,size-60,size-60,size*9+60);//左竖
    p.drawLine(size-60,size*9+60,size*9+60,size*9+60);//下横
    p.drawLine(size*9+60,size-60,size*9+60,size*9+60);//右竖
    background.setWidth(5);
    p.setPen(background);
    short n=1;
    //画棋盘的垂直线
    for(n=1;n<=9;n++)
    {
        p.drawLine(size*n,size,size*n,size*9);//Vertical
    }
    //画棋盘的水平线
    for(n=1;n<=9;n++)
    {
        p.drawLine(size,size*n,size*9,size*n);//Horizon
    }
    //写棋盘上下字母
    QPoint pointU(size+30, size-5);
    p.setFont(QFont("宋体", 30));
    p.drawText(pointU, tr("A\tB\tC\tD\tE\tF\tG\tH"));
    QPoint pointD(size+30, size*9+45);
    p.setFont(QFont("宋体", 30));
    p.drawText(pointD, tr("A\tB\tC\tD\tE\tF\tG\tH"));
    //写棋盘左右数字
    p.setFont(QFont("宋体", 30));
    p.drawText(size/2,size+50, tr("8"));
    p.drawText(size/2,2*size+50, tr("7"));
    p.drawText(size/2,3*size+50, tr("6"));
    p.drawText(size/2,4*size+50, tr("5"));
    p.drawText(size/2,5*size+50, tr("4"));
    p.drawText(size/2,6*size+50, tr("3"));
    p.drawText(size/2,7*size+50, tr("2"));
    p.drawText(size/2,8*size+50, tr("1"));
    p.setFont(QFont("宋体", 30));
    p.drawText(9*size+20,size+50, tr("8"));
    p.drawText(9*size+20,2*size+50, tr("7"));
    p.drawText(9*size+20,3*size+50, tr("6"));
    p.drawText(9*size+20,4*size+50, tr("5"));
    p.drawText(9*size+20,5*size+50, tr("4"));
    p.drawText(9*size+20,6*size+50, tr("3"));
    p.drawText(9*size+20,7*size+50, tr("2"));
    p.drawText(9*size+20,8*size+50, tr("1"));
    //涂格子
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);// 反走样
    painter.setBrush(Qt::darkGray);
    int a[4]={1,3,5,7};
    int b[4]={2,4,6,8};
    for(int i=1;i<=8;i++)
    {
        if(i%2==1)
        {
            for (int j=0;j<4;j++)
            {
                painter.drawRect(size*b[j], size*i, size, size);
            }
        }
        else
        {
            for (int j=0;j<4;j++)
            {
                painter.drawRect(size*a[j], size*i, size, size);
            }
        }
    }
    //将棋子对应的图片导入
    pic[0]=QImage("E:/qtprojects/Chess/pic/黑塔.png");
    pic[1]=QImage("E:/qtprojects/Chess/pic/黑骑.png");
    pic[2]=QImage("E:/qtprojects/Chess/pic/黑主教.png");
    pic[3]=QImage("E:/qtprojects/Chess/pic/黑后.png");
    pic[4]=QImage("E:/qtprojects/Chess/pic/黑王.png");
    pic[5]=QImage("E:/qtprojects/Chess/pic/黑主教.png");
    pic[6]=QImage("E:/qtprojects/Chess/pic/黑骑.png");
    pic[7]=QImage("E:/qtprojects/Chess/pic/黑塔.png");
    for(int i=8;i<=15;i++)
    {
        pic[i]=QImage("E:/qtprojects/Chess/pic/黑兵.png");
    }
    pic[16]=QImage("E:/qtprojects/Chess/pic/白塔.png");
    pic[17]=QImage("E:/qtprojects/Chess/pic/白骑.png");
    pic[18]=QImage("E:/qtprojects/Chess/pic/白主教.png");
    pic[19]=QImage("E:/qtprojects/Chess/pic/白后.png");
    pic[20]=QImage("E:/qtprojects/Chess/pic/白王.png");
    pic[21]=QImage("E:/qtprojects/Chess/pic/白主教.png");
    pic[22]=QImage("E:/qtprojects/Chess/pic/白骑.png");
    pic[23]=QImage("E:/qtprojects/Chess/pic/白塔.png");
    for(int i=24;i<=31;i++)
    {
        pic[i]=QImage("E:/qtprojects/Chess/pic/白兵.png");
    }
    //生成棋子
    for(int i=0;i<32;i++)
    {
        if(i == _raiseid1)
        {
            if(_s[i]._dead)
            {
                continue;
            }
            QPoint c=center(i);
            p.drawImage(QRect(c.x(),c.y(),50,50),QImage("E:/qtprojects/Chess/pic/黑后.png"));
            continue;
        }
        if(i == _raiseid2)
        {
            if(_s[i]._dead)
            {
                continue;
            }
            QPoint c=center(i);
            p.drawImage(QRect(c.x(),c.y(),50,50),QImage("E:/qtprojects/Chess/pic/白后.png"));
            continue;
        }
        drawStone(p,i);
    }
    //生成前一步走棋提示框
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            QPoint c1=center(i,j);
            QRect rect = QRect(c1.x()-15,c1.y()-10,80,80);
            if(theLastStep[i][j]==10)
            {
                p.drawImage(rect,QImage("E:/qtprojects/Chess/pic/被选中1.png"));
                theLastStep[i][j]=0;
            }
        }
    }
    //生成可走格子提示
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            QPoint c2=center(i,j);
            QRect rect = QRect(c2.x()-15,c2.y()-10,80,80);
            if(canSelect[i][j]==10)
            {
                p.drawImage(rect,QImage("E:/qtprojects/Chess/pic/可走.png"));
                canSelect[i][j]=0;
            }
        }
    }
    //结束绘制
    p.end();
}
//棋子生成函数
void MainWindow::drawStone(QPainter & p, int id)
{
    if(_s[id]._dead)
    {
        return;
    }
    //如果该id棋子被选中，则
    QPoint c=center(id);
    QRect rect = QRect(c.x(),c.y(),50,50);
    if(id == _selectid)
    {
        p.drawImage(QRect(c.x()-15,c.y()-10,80,80),QImage("E:/qtprojects/Chess/pic/被选中1.png"));
    }

    p.drawImage(rect,pic[id]);
}
//从行和列获取棋子id所对应的像素点
QPoint MainWindow::center(int row,int col)
{
    QPoint ret;
    ret.rx()=col*80+95;
    ret.ry()=row*80+90;
    return ret;
}
//从棋子id所对应的像素点获取行和列
QPoint MainWindow::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}
//------------------------------------------------------------------------------------
//鼠标释放事件函数
void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }
    QPoint pt = ev->pos();
    click(pt);
}
//点击函数1由鼠标点击坐标点得到对应棋子的坐标点
void MainWindow::click(QPoint pt)
{
    int row, col;
    bool bClicked = getRowCol(pt, row, col);
    // 点击地方不合理,即棋盘外
    if(!bClicked)
    {
        return;
    }
    //点击合理
    int id = getStoneId(row,col);
    //显示棋子能走的区域
    showSelect(id,row,col);
    //进行下一步
    click(id,row,col);

}
//点击函数2点击棋子的编号和坐标尝试走棋
void MainWindow::click(int id, int row, int col)
{
    if(this->_selectid == -1)        // 如果没有选择到棋子，选择棋子（即获得有效点击范围内的id）
    {
        trySelectStone(id);
    }
    else                             // 选择到棋子，移动棋子
    {
        tryMoveStone(id,row,col);
    }
}
// 尝试走棋函数
void MainWindow::tryMoveStone(int killid, int row, int col)
{
    // 如果尝试吃颜色相同的棋子，那么重新选择棋子
    if(killid !=-1 && !sameColor(killid, _selectid))
    {
        trySelectStone(killid);
        return;
    }
    // 如果可以移动棋子，那么移动棋子
    if(canMove(_selectid, killid, row, col))
    {
        //送王警告
        if(!safeForWANG(_selectid,row,col))
        {
            if(_s[_selectid]._red)
            {
                sendKing1+=1;
                //生成送王警告框
                if(sendKing1==1)
                {
                    QString str =QStringLiteral("The first time to send the king!");
                    QMessageBox::information(NULL,  "WARNING!",str, QMessageBox::Yes);
                }
                if(sendKing1==3)
                {
                    QString str =QStringLiteral("The second time to send the king!");
                    QMessageBox::information(NULL,  "WARNING！",str, QMessageBox::Yes);
                }
                if(sendKing1==5)
                {
                    QString str = _bRedTurn?QStringLiteral("Black Win!"):QStringLiteral("White Win!");
                    QMessageBox::information(NULL,  "GAME OVER",str, QMessageBox::Yes);
                }
                sendKing1+=1;
                return;
            }
            if(!_s[_selectid]._red)
            {
                sendKing2+=1;
                //生成送王警告框
                if(sendKing2==1)
                {
                    QString str =QStringLiteral("The first time to send the king!");
                    QMessageBox::information(NULL,  "WARNING!",str, QMessageBox::Yes);
                }
                if(sendKing2==3)
                {
                    QString str =QStringLiteral("The second time to send the king!");
                    QMessageBox::information(NULL,  "WARNING！",str, QMessageBox::Yes);
                }
                if(sendKing2==5)
                {
                    QString str = _bRedTurn?QStringLiteral("Black Win!"):QStringLiteral("White Win!");
                    QMessageBox::information(NULL,  "GAME OVER",str, QMessageBox::Yes);
                }
                sendKing2+=1;
                return;
            }
        }
        moveStone(_selectid, killid, row, col);
        _selectid = -1;
        update();
    }
}
//同色棋子判断
bool MainWindow::sameColor(int moveid, int killid)
{
    if(_s[moveid]._red == _s[killid]._red)  //颜色一样
    {
        //换选择
        _selectid=killid;
        update();
        return false;
    }
    return true;
}
//走棋函数：吃子和移动
void MainWindow::moveStone(int _selectid, int killid, int row, int col)
{
    _s[_selectid]._row = row;
    _s[_selectid]._col = col;
    //吃子
    if(killid!=-1)
    {
        _s[killid]._dead = true;
        if(checkMate(_selectid,row,col))
        {
            update();
            QString str =_bRedTurn?QStringLiteral("White Check!"):QStringLiteral("Black Check!");
            QMessageBox::information(NULL,  "WARNING!",str, QMessageBox::Yes,QMessageBox::Yes);
            isCheck=true;
            canChange=false;
        }
        if(resCheck(row,col))
        {
            isCheck=false;
            canChange=true;
        }
        if(_s[killid]._type==Stone::WANG)
        {
            QString str = _bRedTurn?QStringLiteral("White Win!"):QStringLiteral("Black Win!");
            QMessageBox::warning(NULL,  "GAME OVER",str, QMessageBox::Yes ,QMessageBox::Yes);
        }
    }
    if(killid==-1)
    {
        if(checkMate(_selectid,row,col))
        {
            update();
            QString str =_bRedTurn?QStringLiteral("White Check!"):QStringLiteral("Black Check!");
            QMessageBox::information(NULL,  "WARNING!",str, QMessageBox::Yes,QMessageBox::Yes);
            isCheck=true;
            canChange=false;
        }
        if(resCheck(row,col))
        {
            isCheck=false;
            canChange=true;
        }
    }
    _selectid = -1;
    //轮到另一边走棋
    _bRedTurn = !_bRedTurn;
}
//棋子鉴别函数
void MainWindow::trySelectStone(int id)
{
    if(_bRedTurn != _s[id]._red)//鉴别是否该某一方走棋
    {
        _selectid = id;
        update();
    }

}
//判断能否选择棋子
bool MainWindow::getRowCol(QPoint pt, int &row, int &col)//临时引用
{
    for(row=0; row<=7; row++)
    {
        for(col=0; col<=7; col++)
        {
            QPoint c = center(row, col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx+dy*dy;
            if(dist < 80*80)    //需要优化
            {
                return true;
            }
        }
    }
    return false;
}
//通过行和列获取棋子ID
int MainWindow::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !_s[i]._dead)
        {
            return i;
        }
    }
    return -1;
}
//最基本的能否行走函数
bool MainWindow::canMove(int moveid,int killid,int row,int col)
{
    switch (_s[moveid]._type)
    {
        case Stone::WANG:
            return canMoveWANG(moveid, killid, row, col);
            break;
        case Stone::HOU:
            return canMoveHOU(moveid, killid, row, col);
            break;
        case Stone::TA:
            return canMoveTA(moveid, killid, row, col);
            break;
        case Stone::BING:
            return canMoveBING(moveid, killid, row, col);
            break;
        case Stone::ZHUJIAO:
            return canMoveZHUJIAO(moveid, killid, row, col);
            break;
        case Stone::QI:
            return canMoveQI(moveid, killid, row, col);
            break;
        default:break;
    }
    return false;
}
//判断塔能否移动
bool MainWindow::canMoveTA(int moveid,int ,int row,int col) //此处忽略killid，避免warning
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    if((row1==row || col1==col) && ret==0)  //标记该棋上一步的动作，下同
    {
        theLastStep[row1][col1]=10;
        theLastStep[row][col]=10;
    }
    if(ret == 0)
    {
        if(row1==0  && (col1==0 || col1==7))
        {
            changeT1+=1;
        }
        if(row1==7  && (col1==0 || col1==7))
        {
            changeT2+=1;
        }
        return true;
    }
    return false;

}
//判断主教能否移动
bool MainWindow::canMoveZHUJIAO(int moveid, int , int row, int col) //此处忽略killid，避免warning
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountObliqueLine(row1, col1, row, col);
    if(qAbs(row1-row)==qAbs(col1-col) && ret==0)
    {
        theLastStep[row1][col1]=10;
        theLastStep[row][col]=10;
    }
    if(ret == 0 )
    {
        return true;
    }
    return false;

}
//判断后能否移动
bool MainWindow::canMoveHOU(int moveid,int ,int row,int col) //此处忽略killid，避免warning
{
    GetRowCol(row1, col1, moveid);
    int ret1 = getStoneCountAtLine(row1, col1, row, col);
    int ret2 = getStoneCountObliqueLine(row1,col1,row,col);
    if((row1==row || col1==col || qAbs(row1-row)==qAbs(col1-col)) && ((ret1 == 0 && ret2== -1) || (ret2 == 0 && ret1 == -1)))
    {
        theLastStep[row1][col1]=10;
        theLastStep[row][col]=10;
    }
    if(ret1 == 0 && ret2 == -1)
    {
        return true;
    }
    if(ret1 == -1 && ret2 == 0)
    {
        return true;
    }
    return false;

}
//判断骑能否移动
bool MainWindow::canMoveQI(int moveid,int ,int row,int col)//此处忽略killid，避免warning
{
    GetRowCol(row1, col1, moveid);
    if((qAbs(row1-row)==1 && qAbs(col1-col)==2)||(qAbs(row1-row)==2 && qAbs(col1-col)==1))
    {
        theLastStep[row1][col1]=10;
        theLastStep[row][col]=10;
    }
    int r = relation(row1, col1, row, col);
    if(r == 12 || r == 21)
    {
        return true;
    }
    return false;
}
//判断兵能否移动
bool MainWindow::canMoveBING(int moveid,int killid,int row,int col)//此处忽略killid，避免warning
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    //在上方
    if(isTopSide(moveid))
    {
        //只能往前走
        if(row>row1)
        {
            //兵在初始点的时候，可以选择往前走一步或两步
            if(row1==1)
            {
                if(r==10 && killid == -1)
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    return true;
                }
                if(r==20 && ret==0 && killid == -1)
                {
                    //特殊规则：吃过路兵
                    int cid1=getStoneId(row,col+1);
                    int cid2=getStoneId(row,col-1);
                    if(_s[cid1]._type==Stone::BING || _s[cid2]._type==Stone::BING)
                    {
                        if(_s[moveid]._red!=_s[cid1]._red||_s[moveid]._red!=_s[cid2]._red)
                        {
                            enptP.rx()=row-1;
                            enptP.ry()=col;
                            enpt=true;
                        }
                    }
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    return true;
                }
                if(r==11 && killid !=-1)
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    return true;
                }
            }
            //其他时候除了斜格吃子，只能往前走一步
            else
            {
                if(r==10 && killid ==-1)
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    //特殊规则，兵升变
                    if(row==7)
                    {
                        _s[moveid]._type=Stone::HOU;
                        _raiseid1=moveid;
                    }
                    return true;
                }
                if(r==11 && (killid !=-1 || (enpt==true&& row==enptP.rx()&& col==enptP.ry() && killid ==-1)))
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    int cid=getStoneId(row-1,col);
                    //特殊规则，兵升变
                    if(row==7 )
                    {
                        _s[moveid]._type=Stone::HOU;
                        _raiseid1=moveid;
                    }
                    if(enpt==true&& row==enptP.rx()&& col==enptP.ry())
                    {
                        _s[cid]._dead=true;
                        enpt = false;
                    }
                    return true;
                }
            }
        }
        //不允许后退或横走
        else
        {
            return false;
        }
    }
    //在下方
    else
    {
        //只能往前走
        if(row<row1)
        {
            //兵在初始点的时，可以选择往前走一步或两步
            if(row1==6)
            {
                if(r==10 && killid ==-1)
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    return true;
                }
                if(r==20 && ret==0 && killid == -1)
                {
                    //特殊规则：吃过路兵
                    int cid1=getStoneId(row,col+1);
                    int cid2=getStoneId(row,col-1);
                    if((_s[cid1]._type==Stone::BING && _s[moveid]._red!=_s[cid1]._red)||(_s[cid2]._type==Stone::BING && _s[moveid]._red!=_s[cid2]._red))
                    {
                        enptP.rx()=row+1;
                        enptP.ry()=col;
                        enpt=true;
                    }
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    return true;
                }
                if(r==11 && killid !=-1)
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    return true;
                }

            }
            //其他时候除了斜格吃子，只能往前走一步
            else
            {
                if(r==10 && killid == -1)
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    //特殊规则：兵升变
                    if(row==0 )
                    {
                        _s[moveid]._type=Stone::HOU;
                        _raiseid2 = moveid;
                    }
                    return true;
                }
                if(r==11 && (killid !=-1 || (enpt==true&& row==enptP.rx()&& col==enptP.ry() && killid ==-1)))
                {
                    theLastStep[row1][col1]=10;
                    theLastStep[row][col]=10;
                    int cid=getStoneId(row+1,col);
                    //特殊规则：兵升变
                    if(row==0 )
                    {
                        _s[moveid]._type=Stone::HOU;
                        _raiseid2 = moveid;
                    }
                    if(enpt==true&& row==enptP.rx()&& col==enptP.ry())
                    {
                        _s[cid]._dead=true;
                        enpt = false;
                    }
                    return true;
                }
            }
        }
        //不允许后退或横走
        else
        {
            return false;
        }
    }
    return false;
}
//判断王能否移动
bool MainWindow::canMoveWANG(int moveid,int ,int row,int col)//此处忽略killid，避免warning
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    int r = relation(row1, col1, row, col);
    if(r == 10 || r == 1 || r == 11)
    {
        if(row1==row || col1==col || qAbs(row1-row)==qAbs(col1-col))
        {
            theLastStep[row1][col1]=10;
            theLastStep[row][col]=10;
        }
        if(row1==0  && col1==4)
        {
            changeT1+=1;
        }
        if(row1==7  && col1==4)
        {
            changeT2+=1;
        }
        return true;
    }
    //特殊规则：王车易位
    if(row==7 && ret==0 && safeForWANG(moveid,row,col) && r==2 && canChange==true && isCheck==false && changeT2==0  )
    {
        if(col==2)
        {
            _s[16]._col=3;
            changeT2+=1;
            return true;
        }
        if(col==6)
        {
            _s[23]._col=6;
            changeT2+=1;
            return true;
        }
    }
    if(row==0 && ret==0 && safeForWANG(moveid,row,col) && r==2 && canChange==true && isCheck==false && changeT1==0  )
    {
        if(col==2)
        {
            _s[0]._col=3;
            changeT1+=1;
            return true;
        }
        if(col==6)
        {
            _s[7]._col=6;
            changeT1+=1;
            return true;
        }
    }
    return false;
}
//获取棋子位置与点击位置是否是直线且该直线有多少棋子的函数
int MainWindow::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
    {
        return -1;
    }
    if(row1 == row2 && col1 == col2)
    {
        return -1;
    }
    //行相等，取最小行累加到最大行的值，列的值固定，则根据行列得到的id判别该列行有无棋子（空行列默认值为-1）
    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1)
            {
                ++ret;
            }
        }
    }
    //列相等，同理
    if(col1 == col2)
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1)
            {
                ++ret;
            }
        }
    }

    return ret;
}
//获取棋子位置与点击位置是否是斜线且其中有多少棋子的函数
int MainWindow::getStoneCountObliqueLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1==row2 && col1==col2)
    {
        return -1;
    }
    //斜线上两位置行之间的差=列之间的差，则根据行列得到的id判别该列行有无棋子（空行列默认值为-1）
    if(qAbs(row1-row2)==qAbs(col1-col2))
    {
        int mincol = col1 < col2 ? col1 : col2;
        int maxcol = col1 < col2 ? col2 : col1;

        int minrow = row1 < row2 ? row1 : row2;
        int maxrow = row1 < row2 ? row2 : row1;
        for(int col = mincol+1; col<maxcol; ++col)
        {
            for(int row = minrow+1;row<maxrow;++row)
            {
                if(getStoneId(row, col) != -1 && qAbs(row-row2)==qAbs(col-col2))
                {
                    ++ret;
                }
            }
        }
    }
    else
    {
        return -1;
    }
    return ret;
}
//得到两点之间的关系
int MainWindow::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}
//判断是否是上方
bool MainWindow::isTopSide(int id)
{
    return _bSide == _s[id]._red;
}
//判断王的行走位置是否是敌对棋子的攻击空间,即送王三次判负
bool MainWindow::safeForWANG(int moveid,int row, int col)
{
    for(int r=0;r<8;r++)
    {
        for(int c=0;c<8;c++)
        {
            int s=getStoneId(r,c);
            //判断是否在敌对骑士的攻击格子内
            if(_s[moveid]._type==Stone::WANG && _bRedTurn != _s[moveid]._red )
            {
                if((qAbs(r-row)==1 && qAbs(c-col)==2)||(qAbs(r-row)==2 && qAbs(c-col)==1))
                {
                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::QI)
                    {
                        return  false;
                    }

                }
            }
            //判断是否在敌对塔的攻击格子内
            if(_s[moveid]._type==Stone::WANG && _bRedTurn != _s[moveid]._red )
            {
                if(r==row || c==col)
                {
                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::TA)
                    {
                        int ret = getStoneCountAtLine(r, c, row, col);
                        if(ret == 0)
                        {
                            return false;
                        }
                    }

                }
            }
            //判断是否在敌对主教的攻击格子内
            if(_s[moveid]._type==Stone::WANG && _bRedTurn != _s[moveid]._red )
            {
                if(qAbs(r-row)==qAbs(c-col))
                {
                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::ZHUJIAO)
                    {
                        int ret = getStoneCountObliqueLine(r, c, row, col);
                        if(ret == 0 )
                        {
                            return false;
                        }
                    }
                }
            }
            //判断是否在敌对后的攻击格子内
            if(_s[moveid]._type==Stone::WANG && _bRedTurn != _s[moveid]._red )
            {
                if(r==row || c==col || qAbs(r-row)==qAbs(c-col))
                {

                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::HOU)
                    {
                        int ret1 = getStoneCountObliqueLine(r, c, row, col);
                        int ret2 = getStoneCountAtLine(r, c, row, col);
                        if((ret1 == 0 && ret2== -1) || (ret2 == 0 && ret1 == -1) )
                        {
                            return false;
                        }
                    }

                }
            }
            //判断是否在敌对兵的攻击格子内
            if(_s[moveid]._type==Stone::WANG && _bRedTurn != _s[moveid]._red )
            {
                int rel = relation(r, c, row, col);
                //在上方
                if(isTopSide(moveid))
                {
                    if(row<r)
                    {
                        if(rel==11 && (_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::BING))
                        {
                            return false;
                        }
                    }
                }
                //在下方
                else
                {
                    if(row>r)
                    {
                        if(rel==11 && (_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::BING))
                        {
                            return false;
                        }
                    }
                }
            }
            //判断是否在敌对王的攻击格子之类
            if(_s[moveid]._type==Stone::WANG && _bRedTurn != _s[moveid]._red ) //待王的安全位置函数完善后修改
            {
                int rel = relation(r, c, row, col);
                if(rel == 10 || rel == 1 || rel == 11)
                {
                    if(r==row || c==col || qAbs(r-row)==qAbs(c-col))
                    {
                        if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
//判断是否将军
bool MainWindow::checkMate(int moveid, int row, int col)
{
    for(int r=0;r<8;r++)
    {
        for(int c=0;c<8;c++)
        {
            int s=getStoneId(r,c);
            //判断骑士是否将军
            if(_s[moveid]._type==Stone::QI && _bRedTurn != _s[moveid]._red )
            {
                if((qAbs(r-row)==1 && qAbs(c-col)==2)||(qAbs(r-row)==2 && qAbs(c-col)==1))
                {
                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG)
                    {
                        ptW.rx()=_s[s]._row;
                        ptW.ry()=_s[s]._col;
                        ptC.rx()=_s[moveid]._row;
                        ptC.ry()=_s[moveid]._col;
                        return  true;
                    }

                }
            }
            //判断塔是否将军
            if(_s[moveid]._type==Stone::TA && _bRedTurn != _s[moveid]._red )
            {
                if(r==row || c==col)
                {
                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG)
                    {
                        ptW.rx()=_s[s]._row;
                        ptW.ry()=_s[s]._col;
                        ptC.rx()=_s[moveid]._row;
                        ptC.ry()=_s[moveid]._col;
                        int ret = getStoneCountAtLine(r, c, row, col);
                        if(ret == 0)
                        {
                            return true;
                        }
                    }

                }
            }
            //判断主教是否将军
            if(_s[moveid]._type==Stone::ZHUJIAO && _bRedTurn != _s[moveid]._red )
            {
                if(qAbs(r-row)==qAbs(c-col))
                {
                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG)
                    {
                        ptW.rx()=_s[s]._row;
                        ptW.ry()=_s[s]._col;
                        ptC.rx()=_s[moveid]._row;
                        ptC.ry()=_s[moveid]._col;
                        int ret = getStoneCountObliqueLine(r, c, row, col);
                        if(ret == 0)
                        {
                            return true;
                        }
                    }
                }
            }
            //判断后是否将军
            if(_s[moveid]._type==Stone::HOU && _bRedTurn != _s[moveid]._red )
            {
                if(r==row || c==col || qAbs(r-row)==qAbs(c-col))
                {

                    if(_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG)
                    {
                        ptW.rx()=_s[s]._row;
                        ptW.ry()=_s[s]._col;
                        ptC.rx()=_s[moveid]._row;
                        ptC.ry()=_s[moveid]._col;
                        int ret1 = getStoneCountObliqueLine(r, c, row, col);
                        int ret2 = getStoneCountAtLine(r, c, row, col);
                        if((ret1 == 0 && ret2== -1) || (ret2 == 0 && ret1 == -1) )
                        {
                            return true;
                        }
                    }

                }
            }
            //判断兵是否将军
            if(_s[moveid]._type==Stone::BING && _bRedTurn != _s[moveid]._red )
            {
                int rel = relation(r, c, row, col);
                //在上方
                if(isTopSide(moveid))
                {
                    if(row<r)
                    {
                        if(rel==11 && (_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG))
                        {
                            ptW.rx()=_s[s]._row;
                            ptW.ry()=_s[s]._col;
                            ptC.rx()=_s[moveid]._row;
                            ptC.ry()=_s[moveid]._col;
                            return true;
                        }
                    }
                }
                //在下方
                else
                {
                    if(row>r)
                    {
                        if(rel==11 && (_s[s]._red!=_s[moveid]._red && _s[s]._type==Stone::WANG))
                        {
                            ptW.rx()=_s[s]._row;
                            ptW.ry()=_s[s]._col;
                            ptC.rx()=_s[moveid]._row;
                            ptC.ry()=_s[moveid]._col;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
//显示可选择格子的函数
void MainWindow::showSelect(int id, int row, int col)
{
    for(int r=0;r<8;r++)
    {
        for(int c=0;c<8;c++)
        {
            int s=getStoneId(r,c);
            //显示骑士能走的格子
            if(_s[id]._type==Stone::QI && _bRedTurn != _s[id]._red )
            {
                if((qAbs(r-row)==1 && qAbs(c-col)==2)||(qAbs(r-row)==2 && qAbs(c-col)==1))
                {

                    if(_s[s]._red!=_s[id]._red || s==-1)
                    {
                        canSelect[r][c]=10;
                    }

                }
            }
            //显示塔能走的格子
            if(_s[id]._type==Stone::TA && _bRedTurn != _s[id]._red )
            {
                if(r==row || c==col)
                {

                    if(_s[s]._red!=_s[id]._red || s==-1)
                    {
                        int ret = getStoneCountAtLine(r, c, row, col);
                        if(ret == 0)
                        {
                            canSelect[r][c]=10;
                        }
                    }

                }
            }
            //显示主教能走的格子
            if(_s[id]._type==Stone::ZHUJIAO && _bRedTurn != _s[id]._red )
            {
                if(qAbs(r-row)==qAbs(c-col))
                {

                    if(_s[s]._red!=_s[id]._red || s==-1)
                    {
                        int ret = getStoneCountObliqueLine(r, c, row, col);
                        if(ret == 0)
                        {
                            canSelect[r][c]=10;
                        }
                    }

                }
            }
            //显示后能走的格子
            if(_s[id]._type==Stone::HOU && _bRedTurn != _s[id]._red )
            {
                if(r==row || c==col || qAbs(r-row)==qAbs(c-col))
                {

                    if(_s[s]._red!=_s[id]._red || s==-1)
                    {
                        int ret1 = getStoneCountObliqueLine(r, c, row, col);
                        int ret2 = getStoneCountAtLine(r, c, row, col);
                        if((ret1 == 0 && ret2== -1) || (ret2 == 0 && ret1 == -1))
                        {
                            canSelect[r][c]=10;
                        }
                    }

                }
            }
            //显示兵能走的格子
            if(_s[id]._type==Stone::BING && _bRedTurn != _s[id]._red )
            {
                int rel = relation(r, c, row, col);
                int ret = getStoneCountAtLine(r, c, row, col);
                //在上方
                if(isTopSide(id))
                {
                    if(row<r)
                    {
                        if(row==1)
                        {
                            if(rel ==10 && s==-1)
                            {
                                canSelect[r][c]=10;
                            }
                            if(rel==20 && ret==0 && s==-1)
                            {
                                canSelect[r][c]=10;
                            }
                            if(rel==11 && (_s[s]._red!=_s[id]._red && s!=-1))
                            {
                                canSelect[r][c]=10;
                            }
                        }
                        //其他时候除了斜格吃子，只能往前走一步
                        else
                        {
                            if(rel==10 && s==-1)
                            {
                                canSelect[r][c]=10;
                            }
                            if(rel==11 && ( (_s[s]._red!=_s[id]._red && s!=-1) || (enpt==true&& r==enptP.rx()&& c==enptP.ry()) ))
                            {
                                canSelect[r][c]=10;
                            }
                        }
                    }
                }
                //在下方
                else
                {
                    if(row>r)
                    {
                        if(row==6)
                        {
                            if(rel==10 && s==-1)
                            {
                                canSelect[r][c]=10;
                            }
                            if(rel==20 && ret==0 && s==-1)
                            {
                                canSelect[r][c]=10;
                            }
                            if(rel==11 && (_s[s]._red!=_s[id]._red && s!=-1))
                            {
                                canSelect[r][c]=10;
                            }
                        }
                        //其他时候除了斜格吃子，只能往前走一步
                        else
                        {
                            if(rel==10 &&  s==-1)
                            {
                                canSelect[r][c]=10;
                            }
                            if(rel==11 && ( (_s[s]._red!=_s[id]._red && s!=-1) || (enpt==true&& r==enptP.rx()&& c==enptP.ry()) ))
                            {
                                canSelect[r][c]=10;
                            }
                        }
                    }
                }
            }
            //显示王能走的格子
            if(_s[id]._type==Stone::WANG && _bRedTurn != _s[id]._red )
            {
                int rel = relation(r, c, row, col);
                int ret = getStoneCountAtLine(r,c,row,col);
                if(row==7 && ret==0 && safeForWANG(id,7,2) && rel==2 && canChange==true && isCheck==false && changeT2==0  )
                {
                    canSelect[r][c]=10;
                }
                if(row==0 && ret==0 && safeForWANG(id,7,2) && rel==2 && canChange==true && isCheck==false && changeT1==0  )
                {
                    canSelect[r][c]=10;
                }
                if(rel == 10 || rel == 1 || rel == 11)
                {
                    if(r==row || c==col || qAbs(r-row)==qAbs(c-col))
                    {
                        if(_s[s]._red!=_s[id]._red || s==-1)
                        {
                            canSelect[r][c]=10;
                        }
                    }
                }
            }
        }
    }
    update();
}
//是否应将判断函数
bool MainWindow::resCheck(int row,int col)
{
    int rid=getStoneId(ptC.rx(),ptC.ry());
    //对塔应将
    if(_s[rid]._type==Stone::TA)
    {
        //其他棋子应将的情况
        int ret1 = getStoneCountAtLine(ptW.rx(), ptW.ry(), ptC.rx(), ptC.ry());
        //移动王应将的情况
        int ret2 = getStoneCountAtLine(row, col, ptC.rx(), ptC.ry());
        if(ret1 != 0 || ret2 ==-1)
        {
            return true;
        }
        if(_s[rid]._dead==true)
        {
            return true;
        }
    }
    //对主教应将
    if(_s[rid]._type==Stone::ZHUJIAO)
    {
        int ret1 = getStoneCountObliqueLine(ptW.rx(), ptW.ry(), ptC.rx(), ptC.ry());
        int ret2 = getStoneCountObliqueLine(row, col, ptC.rx(), ptC.ry());
        if(ret1 != 0 || ret2==-1)
        {
            return true;
        }
        if(_s[rid]._dead==true)
        {
            return true;
        }
    }
    //对后应将
    if(_s[rid]._type==Stone::HOU)
    {
        int ret1 = getStoneCountAtLine(ptW.rx(), ptW.ry(), ptC.rx(), ptC.ry());
        int ret2 = getStoneCountObliqueLine(ptW.rx(), ptW.ry(), ptC.rx(), ptC.ry());
        //其他棋子应将的情况
        if(ret1 != 0 && ret2 == -1)
        {
            return true;
        }
        if(ret1 == -1 && ret2 != 0)
        {
            return true;
        }
        //移动王应将的情况
        if(ret1 == -1 && ret2 == -1)
        {
            return true;
        }
        if(_s[rid]._dead==true)
        {
            return true;
        }
    }
    //对骑应将
    if(_s[rid]._type==Stone::QI)
    {
        int r1 = relation(ptW.rx(), ptW.ry(), ptC.rx(), ptC.ry());
        int r2 = relation(row, col, ptC.rx(), ptC.ry());
        if(r1 != 12 && r1 != 21)
        {
            return true;
        }
        if(r2 != 12 && r2 != 21)
        {
            if((row==2 && col==7)||(row==6 && col==7))
            {
                return false;
            }
            return true;
        }
        if(_s[rid]._dead==true)
        {
            return true;
        }
    }
    //对兵应将
    if(_s[rid]._type==Stone::BING)
    {
        int r1 = relation(ptW.rx(), ptW.ry(), ptC.rx(), ptC.ry());
        int r2 = relation(row, col, ptC.rx(), ptC.ry());
        if(r1!=11 || r2!=11)
        {
            if((row==2 && col==7)||(row==6 && col==7))
            {
                return false;
            }
            return true;
        }
        if(_s[rid]._dead==true)
        {
            return true;
        }
    }
    return false;
}

