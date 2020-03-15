#include "solider.h"
#include "global.h"
Solider::Solider(int _id)
{
    id=_id;
    red=true;
    dead=false;
    if(_id<16)
        red=1;
    else
        red=0;

    selected=false;  //默认未选中
    name=getNameFromId(id);
    pos=getPosFromId(id);
    xPos=pos.x();
    yPos=pos.y();
    addScore=0;
}
QString Solider::getNameFromId(int _id)
{
    switch (_id)
    {
    case 0:
    case 8:
    case 16:
    case 24:
            name="車";
            score=CAR;
        break;
    case 1:
    case 7:
    case 17:
    case 23:
        name="馬";
        score=HORSE;
        break;
    case 2:
    case 6:
        name="相";
        score=ELEPHANT;
        break;
    case 18:
    case 22:
        name="象";
        score=ELEPHANT;
        break;
    case 3:
    case 5:
        name="仕";
        score=BISHOP;
        break;
    case 19:
    case 21:
        name="士";
        score=BISHOP;
        break;
    case 4:
        name="帥";
        score=KING;
        break;
    case 20:
        name="將";
        score=KING;
        break;
    case 9:
    case 10:
        name="砲";
        score=CANON;
        break;
    case 25:
    case 26:
        name="炮";
        score=CANON;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        name="兵";
        score=PAWN;
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
        name="卒";
        score=PAWN;
        break;
    }
    return name;
}

QPoint Solider::getPosFromId(int _id)
{
    QPoint point;

    //红方下边的8个棋子
    if(_id<9)
    {
        point.setX(10);
        point.setY(_id+1);
    }
    //黑方的8个棋子
    if(_id>15 && _id<25)
    {
        point.setX(1);
        point.setY(_id-15);
    }
    //红方5个兵
    if(_id>10 && _id<16)
    {
        point.setX(7);
        point.setY((_id-10-1)*2+1);   //换算关系为(n-1)*2+1
    }
    //黑方5个卒子
    if(_id>26 && _id<32)
    {
        point.setX(4);
        point.setY((_id-26-1)*2+1); //换算关系为(n-1)*2+1;
    }
    //红方两个炮
    if(_id==9)
    {
        point.setX(8);
        point.setY(2);
    }
    if(_id==10)
    {
        point.setX(8);
        point.setY(8);
    }
    //黑方两个炮
    if(_id==25)
    {
        point.setX(3);
        point.setY(2);
    }
    if(_id==26)
    {
        point.setX(3);
        point.setY(8);
    }
    return point;


}
