#include "step.h"

Step::Step(int _id,QPoint _begin,QPoint _target,int _killed)
{
    this->id=_id;
    this->begin=_begin;
    this->target=_target;
    this->killed=_killed;   //如果_target没有棋子，则killed的值是-1
}
