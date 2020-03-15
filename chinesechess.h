#ifndef CHINESECHESS_H
#define CHINESECHESS_H

#include <QMainWindow>
#include "solider.h"
#include "step.h"
#include <QVector>
QT_BEGIN_NAMESPACE
namespace Ui { class ChineseChess; }
QT_END_NAMESPACE

class ChineseChess : public QMainWindow
{
    Q_OBJECT

public:
    ChineseChess(QWidget *parent = nullptr);
    ~ChineseChess();
    void drawScene(QPainter &painter);
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint getRowColumn(int xPos,int yPos);  //从鼠标得到棋盘的行和列
    void drawSolider(QPainter &painter,int _id);

    bool judgeMove(Solider *solider,QPoint p);
    //以下是判断帅、车、象、士、马、兵的走法判断，p表示将要去的坐标
    bool judgeMoveShuai(Solider *solider,QPoint p);
    bool judgeMoveChe(Solider *solider,QPoint p);
    bool judgeMoveXiang(Solider *solider,QPoint p);
    bool judgeMoveShi(Solider *solider,QPoint p);
    bool judgeMoveMa(Solider *solider,QPoint p);
    bool judgeMoveBing(Solider *solider,QPoint p);
    bool judgeMovePao(Solider *solider,QPoint p);

    int  judgePoint(QPoint p); //判断p点是否有棋子存在，返回棋子ID，如果没有，返回-1.

    bool redTurn; //判断红方走棋
    QVector<Solider *> soliders;
    int i_distance;  //棋盘格子宽度

    bool gameStart; //游戏是否开始

    void deleteQVector(); //清空指针里的值

    void computerMoveSimple(); //最简单的人工智能
    void getAllPossible();  //得到黑色棋子所有的走法

    void getRedAllPossible(); //得到红色棋子所有的走法

    QVector<Step *> allSteps; //存储所有黑方步数

    QVector<Step *> redAllSteps; //存储 所有红方步数
    void tryStep(Step *);  //尝试黑方走一步
    void tryRedStep(Step *); //尝试红方走一步
    void resoreStep(Step *); //返回走之前的状态;
    int getExtraScore(int id); //得到当前棋子可以额外加的分
    int getScore(); //当前步数 黑方和红方的分数差别
    bool robotPlay;
private slots:
    void on_btnRestart_clicked();

    void on_rbRobot_clicked();

    void on_rbHuman_clicked();

private:
    Ui::ChineseChess *ui;

};
#endif // CHINESECHESS_H
