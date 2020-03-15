#ifndef STEP_H
#define STEP_H

#include <QPoint>


class Step
{
public:
    Step(int _id,QPoint _begin,QPoint _target,int _killed);
    int id; /*棋子ID */
    QPoint begin;  //棋子的开始位置
    QPoint target; //棋子去的位置
    int killed;
};

#endif // STEP_H
