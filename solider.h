#ifndef SOLIDER_H
#define SOLIDER_H

#include <QPoint>
#include <QString>



class Solider
{
public:
    Solider(int _id);
    int xPos,yPos;
    int id;
    bool selected;
    enum   TYPES{ };
    bool dead;    //是否已经死亡
    bool red;     //1是红色棋子 0是黑色棋子
    QString name; //名字
    QPoint pos;   //位于的行和列坐标

    int score;  //棋子的权值分数
    int addScore;  //根据棋子可移动的范围来加分
private:
    QString getNameFromId(int _id); //从id得到名称
    QPoint getPosFromId(int _id);// 从id得到坐标

};

#endif // SOLIDER_H
