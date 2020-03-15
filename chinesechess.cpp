#include "chinesechess.h"
#include "ui_chinesechess.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include "global.h"
ChineseChess::ChineseChess(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChineseChess)
{
    ui->setupUi(this);
    this->setWindowTitle("中国象棋 V0.1 --李志远");

    i_distance=55; //棋盘距离

    for(int i=0;i<32;++i)
    {
        Solider *solider=new Solider(i);
        soliders.push_back(solider);

    //    qDebug()<<solider->xPos<<" "<<solider->yPos<<" "<<solider->id<<" "<<solider->name<<endl;
    }
    redTurn=true; //红先走

    gameStart=true;

    this->ui->opponentLaber->setGeometry(10*i_distance,1*i_distance,91,51);
    this->ui->imageLabel->setGeometry(10*i_distance,2*i_distance,81,91);
    this->ui->imageLabel->setStyleSheet("QLabel {background-color: transparent;}");

    this->ui->rbRobot->setGeometry(10*i_distance,4*i_distance,80,50);
    this->ui->rbHuman->setGeometry(10*i_distance,4.5*i_distance,80,50);

    this->ui->rbRobot->setChecked(true);

    this->ui->btnRestart->setGeometry(10*i_distance,7*i_distance,100,40);
    this->setFixedSize(12*i_distance,11*i_distance);

    robotPlay=true; //人机对战
}


void ChineseChess::drawScene(QPainter &painter)
{
    //painter.drawLine(0,0,200,200);
    for(int i=1;i<=10;i++)
    {
        painter.drawLine(i_distance,i_distance*i,9*i_distance,i_distance*i); //横线
    }
    //画竖线
    for(int i=1;i<10;i++)
    {
        if(i==1 || i==9)
            painter.drawLine(i*i_distance,i_distance,i*i_distance,10*i_distance);
        else
        {
            painter.drawLine(i*i_distance,i_distance,i*i_distance,5*i_distance);
            painter.drawLine(i*i_distance,6*i_distance,i*i_distance,10*i_distance);
        }
    }
    //画斜线
    painter.drawLine(4*i_distance,i_distance,6*i_distance,3*i_distance);
    painter.drawLine(6*i_distance,i_distance,4*i_distance,3*i_distance);
    painter.drawLine(4*i_distance,8*i_distance,6*i_distance,10*i_distance);
    painter.drawLine(6*i_distance,8*i_distance,4*i_distance,10*i_distance);


    //画外框
    QPen pen;  // creates a default pen
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setBrush(Qt::black);
    painter.setPen(pen);
    painter.setBrush(QColor(244, 209, 182, 127));
    painter.drawRect(i_distance-10,i_distance-10,(8*i_distance)+20,9*i_distance+20);

    //画界河文字
    QFont font = painter.font();
     font.setPixelSize(43);
    font.setFamily("隶书");
     painter.setFont(font);
    painter.drawText(2.5*i_distance,5.8*i_distance,"楚河");
    painter.drawText(5.7*i_distance,5.8*i_distance,"漢界");

}

void ChineseChess::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawScene(painter);
    for(int i=0;i<32;++i)
        drawSolider(painter,i);

}

void ChineseChess::mouseReleaseEvent(QMouseEvent *event)
{
    //只有红方可以走，用于人机对战
    if(!redTurn && robotPlay==true)
            return;

    if(gameStart==false)
        return;
    int xPos=event->x();
    int yPos=event->y();

    //如果选择的不是棋盘位置，则跳走
    if(xPos<0.5*i_distance || xPos>9.5*i_distance || yPos<0.5*i_distance || yPos>10.5*i_distance)
        return;

    //qDebug()<<xPos<<" "<<yPos;
    QPoint position=getRowColumn(xPos,yPos); //得到鼠标选择的行和列

    //判断是否是第一次进行选择
    int k;
    for(k=0;k<32;++k)
        if(soliders[k]->selected && soliders[k]->dead==false)
            break;

    //第一次选择棋子
    if(k>=32)
    {
        qDebug()<<"选择棋子";
        //判断所选择的行和列位置是否有棋子
        int i;
        for(i=0;i<32;++i)
        {
            //红色棋子走
            if(soliders[i]->xPos ==position.x() && soliders[i]->yPos==position.y() &&soliders[i]->dead==false && i<16 && redTurn)
                soliders[i]->selected=true;
            //黑色棋子走
            if(soliders[i]->xPos ==position.x() && soliders[i]->yPos==position.y() &&soliders[i]->dead==false && i>15 && redTurn==false)
                soliders[i]->selected=true;
        }
    }
    else //棋子已经被选中
    {
        //判断第二次点击的是否是同一个棋子
        if(position==soliders[k]->pos)
            {
                qDebug()<<"选择了同一个棋子"<<endl;
                return;
            }
        int i;
        for(i=0;i<32;++i)
        {
            //只允许一个棋子选中，所以要把以前选中的同色的棋子取消掉
            if(i!=k && soliders[i]->xPos ==position.x() && soliders[i]->yPos==position.y() && soliders[k]->red==soliders[i]->red && soliders[i]->dead==false)
            {
                soliders[i]->selected=true;
                soliders[k]->selected=false;
                qDebug()<<"换了一个棋子"<<soliders[i]->pos<<endl;
                break;
            }
        }
        if(i>=32)  //走到了一个无人的坐标或者其他颜色的棋子的坐标,将棋子运动过去，并取消选择状态
        {

            //在此处需要判断棋子是否可以符合规则进行移动 soliders[k]的位置 和新的position的位置
            bool b=judgeMove(soliders[k],position);
            if(!b)
            {
                QMessageBox::warning(this,"走棋错误","走法不符合规则","确定");
                return;

            }
            //如果是其他颜色的棋子，则将其杀死
            for(int w=0;w<32;++w)
                 if(soliders[w]->xPos ==position.x() && soliders[w]->yPos==position.y() &&soliders[w]->dead==false)
                 {
                     soliders[w]->dead=true;
                    // soliders[w]->pos=QPoint(-1,-1);
                 }

            soliders[k]->xPos=position.x();
            soliders[k]->yPos=position.y();
            soliders[k]->pos=position;
            soliders[k]->selected=false;

            //每次走完判断两个BOSS是否碰面.
            if(soliders[4]->yPos==soliders[20]->yPos)
            {
                //两个boss在一列,判断这一列有没有其他棋子
                int i;
                for(i=soliders[20]->xPos+1;i<soliders[4]->xPos;++i)
                {
                    if(judgePoint(QPoint(i,soliders[4]->yPos))!=-1)
                        break;
                }
                if(i>=soliders[4]->xPos) //两个boss之间无棋子
                {
                    if(redTurn)
                        soliders[4]->dead=true;
                    else
                        soliders[20]->dead=true;
                }

            }
            //黑方胜
            if(soliders[4]->dead==true)
               {
                    gameStart=false;
                    QMessageBox::information(this,"祝贺","黑方取得胜利");
                }
            if(soliders[20]->dead==true)
               {
                    gameStart=false;
                    QMessageBox::information(this,"祝贺","红方取得胜利");
                }

            redTurn=!redTurn;

            //调用黑方人工智能
            if(robotPlay)
                computerMoveSimple();
        }


    }
    update();

}

QPoint ChineseChess::getRowColumn(int xPos, int yPos)
{
    float c;
    int row,column,x,m,n;
    c=xPos*1.0/i_distance; //得到d的倍数
    c=c*10;
    x=(int)c; //取整数
    m=x/10;
    n=x%10;
    if(n>5)
        m=m+1;
    column=m;  //得到列数

    c=yPos*1.0/i_distance;
    c=c*10;
    x=(int)c;
    m=x/10;
    n=x%10;
    if(n>5)
        m=m+1;
    row=m;
    qDebug()<<row<<" "<<column<<endl;
    return QPoint(row,column);
}

void ChineseChess::drawSolider(QPainter &painter, int id)
{
    if(soliders.at(id)->dead) //如果棋子死了，那就不画它了
        return;
    QPoint p=soliders.at(id)->pos; //得到行列坐标
    int xPos=p.y()*i_distance;
    int yPos=p.x()*i_distance;

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    if(id<16)
        pen.setColor(Qt::red);
    else
        pen.setColor(Qt::black);
     painter.setPen(pen);
  //  pen.setBrush(Qt::black);
    QFont font=painter.font();
    font.setPixelSize(40);
    font.setFamily("隶书");
    painter.setFont(font);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(soliders.at(id)->selected)
        brush.setColor(QColor(30,144,255));
    else
        brush.setColor(QColor(255,255,255));
    painter.setBrush(brush);

    QRectF rectangle1(xPos-i_distance*0.5,yPos-i_distance*0.5, i_distance, i_distance);
    painter.drawEllipse(rectangle1);

   // painter.drawText(xPos-i_distance*0.5,yPos-i_distance*0.5,soliders.at(id)->name);
     painter.drawText(rectangle1,Qt::AlignCenter,soliders.at(id)->name);
   // painter.drawText((xPos-i_distance*0.5,yPos-i_distance*0.5,soliders.at(id)->name));

}

bool ChineseChess::judgeMove(Solider *solider, QPoint p)
{

    //将和帅的走法判断
    if(solider->id==4 || solider->id==20)
        return judgeMoveShuai(solider,p);
    //车的走法判断
    if(solider->id==0|| solider->id==8 ||solider->id==16 || solider->id==24)
        return judgeMoveChe(solider,p);
    //象的走法判断
    if(solider->id==2 || solider->id==6 || solider->id==18 || solider->id==22)
        return judgeMoveXiang(solider,p);
    //马的走法判断
    if(solider->id==1 || solider->id==7 || solider->id==17 || solider->id==23)
        return  judgeMoveMa(solider,p);
    //士的走法判断
    if(solider->id==3 || solider->id==5 || solider->id==19 || solider->id==21)
        return  judgeMoveShi(solider,p);
    //兵和卒的走法判断
    if((solider->id>=11 && solider->id<=15) || (solider->id>=27 && solider->id<=31))
        return  judgeMoveBing(solider,p);

    //炮的走法判断
    if(solider->id==9 || solider->id==10 || solider->id==25 || solider->id==26)
        return  judgeMovePao(solider,p);

    return false;
}

bool ChineseChess::judgeMoveShuai(Solider *solider, QPoint p)
{
    int d;
    if(p.x()>3 && p.x()<8)
        return false;
    if(p.y()<4 || p.y()>6)
        return false;
    //走的不是一个格子
    d=abs(p.x()-solider->xPos)+abs(p.y()-solider->yPos);
    if(d!=1)
        return false;
    int i=judgePoint(p);
    //目标坐标没有棋子
    if(i==-1)
    {
        return true;

    }else
    {
        if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
            return false;
        return true;
    }

}

bool ChineseChess::judgeMoveChe(Solider *solider, QPoint p)
{
    int d;
    int dRow,dCol;
    QPoint po;
    dRow=abs(p.x()-solider->xPos);
    dCol=abs(p.y()-solider->yPos);
    int k=dRow * dCol;
    if(k!=0) //行和列的坐标差值的绝对值必须有一个为0
        return false;

    //判断车到目标点间是否有棋子
    //车是横着走
    if(dRow==0)
    {   if(p.y() - solider->yPos>0)
            d=1;
        else
            d=-1;
        for(int i=1;i<dCol;++i)
        {
            po.setX(solider->xPos);
            po.setY(solider->yPos+i*d);
            if(judgePoint(po)!=-1)
                return false;
        }
    }
    //车是竖着走
    if(dCol==0)
    {
        if(p.x()-solider->xPos>0)
            d=1;
        else
            d=-1;
        for(int i=1;i<dRow;++i)
        {
            po.setX(solider->xPos+i*d);
            po.setY(solider->yPos);
            if(judgePoint(po)!=-1)
                return false;
        }
    }
    int i=judgePoint(p);
    //目标坐标没有棋子
    if(i==-1)
    {
       //判断目标坐标和当前棋子坐标之间是否有棋子
        return true;

    }else
    {
        if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
            return false;
        return true;
    }
}

bool ChineseChess::judgeMoveXiang(Solider *solider, QPoint p)
{
    int dRow,dCol;
    int rowGo,colGo; //为判断象眼，需要移动的坐标
    //红方的棋子的话，行必须在大于6的范围内移动
    if(solider->red && p.x()<6)
        return false;
    //黑方棋子的话，行必须在小于6的范围内移动
    if(solider->red==false && p.x()>5)
        return false;
    dRow=abs(p.x()-solider->xPos);
    dCol=abs(p.y()-solider->yPos);

    if(dRow!=2 || dCol!=2) //行和列的坐标差值的绝对值必须是1,
        return false;
    //判断象眼是否有棋子
    dRow=p.x()-solider->xPos;
    dCol=p.y()-solider->yPos;

    if(dRow<0)
        rowGo=solider->xPos-1;
    else
        rowGo=solider->xPos+1;

    if(dCol<0)
        colGo=solider->yPos-1;
    else
        colGo=solider->yPos+1;
    //象眼是否有棋子，如果有棋子，返回错误
    if(judgePoint(QPoint(rowGo,colGo))!=-1)
        return false;

    int i=judgePoint(p);
    //目标坐标没有棋子
    if(i==-1)
    {
        return true;

    }else
    {
        if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
            return false;
        return true;
    }
}

bool ChineseChess::judgeMoveShi(Solider *solider, QPoint p)
{
    int dRow,dCol;
    if(p.x()>3 && p.x()<8)
        return false;
    if(p.y()<4 || p.y()>6)
        return false;
    //走的不是一个格子
    dRow=abs(p.x()-solider->xPos);
    dCol=abs(p.y()-solider->yPos);

    if(dRow!=1 || dCol!=1) //行和列的坐标差值的绝对值必须是1,
        return false;
    int i=judgePoint(p);
    //目标坐标没有棋子
    if(i==-1)
    {
        return true;

    }else
    {
        if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
            return false;
        return true;
    }
}

bool ChineseChess::judgeMoveMa(Solider *solider, QPoint p)
{
    int dRow,dCol;
    //红方的棋子的话，行必须在大于6的范围内移动

    dRow=abs(p.x()-solider->xPos);
    dCol=abs(p.y()-solider->yPos);
    //通过计算可知，马走的8种情况，4种为dRow=2 dCol=1,另外4种为dRow=1 dCol=2
    //分两种情况讨论马眼的问题
    if(dRow==2 && dCol==1)
    {
        if(p.x()<solider->xPos) //目标点位于当前马的上方
        {
            if(judgePoint(QPoint(solider->xPos-1,solider->yPos))!=-1)
                return false;
        }else //目标点位于当前马的下方
        {
            if(judgePoint(QPoint(solider->xPos+1,solider->yPos))!=-1)
                return false;
        }
        int i=judgePoint(p);
        //目标坐标没有棋子
        if(i==-1)
        {
            return true;

        }else
        {
            if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
                return false;
            return true;
        }

    }
    if(dRow==1 && dCol==2)
    {
        if(p.y()<solider->yPos) //目标点位于当前马的左方
        {
            if(judgePoint(QPoint(solider->xPos,solider->yPos-1))!=-1)
                return false;
        }else //目标点位于当前马的右方
        {
            if(judgePoint(QPoint(solider->xPos,solider->yPos+1))!=-1)
                return false;
        }
        int i=judgePoint(p);
        //目标坐标没有棋子
        if(i==-1)
        {
            return true;

        }else
        {
            if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
                return false;
            return true;
        }
    }
    //其他情况需要返回失误
    return false;
}

bool ChineseChess::judgeMoveBing(Solider *solider, QPoint p)
{
    int dRow,dCol;
    //红方的棋子的话，行必须在大于6的范围内移动
    dRow=p.x()-solider->xPos;
    dCol=abs(p.y()-solider->yPos);
    //判断士兵的颜色，如果红色士兵，则当前位置小于5时候，允许横着移动
    if(solider->red)
    {
       if(solider->xPos>5)
       {
           if(!(dRow== -1 && dCol==0)) //红方士兵向上方移动一格
               return false;
       }else
       {
           if(!((dRow==-1 && dCol==0) || (dRow==0 && dCol==1)))
               return false;
       }
    }else //黑色士兵，当前位置大于5的时候，允许横着移动
    {
        if(solider->xPos<=5)
        {
            if(!(dRow==1 && dCol==0)) //黑色士兵向下方移动一格
               return false;
        }else //黑色士兵过了河界，允许横着跑
        {
            if(!((dRow==1 && dCol==0) || (dRow==0 && dCol==1)))
               return false;
        }
    }

    int i=judgePoint(p);
    //目标坐标没有棋子
    if(i==-1)
    {
        return true;

    }else
    {
        if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
            return false;
        return true;
    }
}

bool ChineseChess::judgeMovePao(Solider *solider, QPoint p)
{
    int d;
    int dRow,dCol;
    QPoint po;
    dRow=abs(p.x()-solider->xPos);
    dCol=abs(p.y()-solider->yPos);
    int k=dRow * dCol;
    if(k!=0) //行和列的坐标差值的绝对值必须有一个为0
        return false;


    int i=judgePoint(p);

    //目标坐标没有棋子
    if(i==-1)
    {
       //判断目标坐标和当前棋子坐标之间是否有棋子

        //判断炮到目标点间是否有棋子
        //炮是横着走
        if(dRow==0)
        {   if(p.y() - solider->yPos>0)
                d=1;
            else
                d=-1;
            for(int i=1;i<dCol;++i)
            {
                po.setX(solider->xPos);
                po.setY(solider->yPos+i*d);
                if(judgePoint(po)!=-1)
                    return false;
            }
        }
        //炮是竖着走
        if(dCol==0)
        {
            if(p.x()-solider->xPos>0)
                d=1;
            else
                d=-1;
            for(int i=1;i<dRow;++i)
            {
                po.setX(solider->xPos+i*d);
                po.setY(solider->yPos);
                if(judgePoint(po)!=-1)
                    return false;
            }
        }
        //目标坐标没有棋子，且到目标棋子中间没有棋子
        return true;

    }else
    {
        if(solider->red==soliders[i]->red) //目标位置相同棋子颜色
            return false;
        //目标坐标是不同颜色棋子，需要判断中间隔了几个棋子，如果是一个，可以吃，如果是0个或者大于1个，则不能走
        int k=0;
        //判断炮到目标点间是否有棋子
        //炮是横着走
        if(dRow==0)
        {   if(p.y() - solider->yPos>0)
                d=1;
            else
                d=-1;
            for(int i=1;i<dCol;++i)
            {
                po.setX(solider->xPos);
                po.setY(solider->yPos+i*d);
                if(judgePoint(po)!=-1)
                    k++;
            }
        }
        //炮是竖着走
        if(dCol==0)
        {
            if(p.x()-solider->xPos>0)
                d=1;
            else
                d=-1;
            for(int i=1;i<dRow;++i)
            {
                po.setX(solider->xPos+i*d);
                po.setY(solider->yPos);
                if(judgePoint(po)!=-1)
                    k++;
            }
        }
        //如果中间的棋子是1个，则可以吃，负责不能吃
        if(k==1)
            return true;
        else
            return false;
    }
}

int ChineseChess::judgePoint(QPoint p)
{
    for(int i=0;i<32;++i)
        if(soliders[i]->xPos==p.x() && soliders[i]->yPos==p.y() && soliders[i]->dead==false)
            return i;
    return -1;
}

void ChineseChess::deleteQVector()
{
    while(soliders.count())
    {
        Solider *temp=soliders.back();
        soliders.removeLast();
        delete temp;
    }

    while(allSteps.count())
    {
        Step *step=allSteps.back();
        allSteps.removeLast();
        delete step;
    }

    while(redAllSteps.count())
    {
        Step *step=redAllSteps.back();
        redAllSteps.removeLast();
        delete step;
    }
}


/* 计算机下棋，使用思考2步法 */
void ChineseChess::computerMoveSimple()
{
  if(gameStart==false)
      return;
    //每次走之前，清空所有的步数
   while(allSteps.count())
   {
       Step *step=allSteps.back();
       allSteps.removeLast();
       delete step;
   }
   while(redAllSteps.count())
   {
       Step *step=redAllSteps.back();
       redAllSteps.removeLast();
       delete step;
   }

   //每次走之前，清空所有的棋子的额外分
   for(int i=0;i<32;++i)
       soliders[i]->addScore=0;

   //得到黑色旗子所有可能的走法
   getAllPossible(); //得到所有可能的步数
   QVector<Step *>::iterator it=allSteps.begin();

   //对每一步走一下试试，计算分数
   int k=0; int max= -1000000;
   int min=1000000;
   int blackMin=1000000;
   Step * bestStep;
   for(;it!=allSteps.end();++it)
   {

       tryStep(*it); //尝试走一走

       getRedAllPossible();// 得到红方所有可能的走法
       QVector<Step *>::iterator itRed=redAllSteps.begin();
        min=1000000;
       for(;itRed!=redAllSteps.end();++itRed)
       {
           tryStep(*itRed);
           k=getScore(); //得到一个分数
           if(k<min)
           {
               min=k;
           }
           resoreStep(*itRed);
       }
       if(min>max)
       {
           max=min;
           bestStep=*it;
       }

       resoreStep(*it); //返回走的那一步
   }
    qDebug()<<blackMin<<endl;
   //bestStep是最好的，将其实际走了
    soliders[bestStep->id]->xPos=bestStep->target.x();
    soliders[bestStep->id]->yPos=bestStep->target.y();
    soliders[bestStep->id]->pos=bestStep->target;
    if(bestStep->killed!=-1)
        soliders[bestStep->killed]->dead=true;

    //每次走完判断两个BOSS是否碰面.
    if(soliders[4]->yPos==soliders[20]->yPos)
    {
        //两个boss在一列,判断这一列有没有其他棋子
        int i;
        for(i=soliders[20]->xPos+1;i<soliders[4]->xPos;++i)
        {
            if(judgePoint(QPoint(i,soliders[4]->yPos))!=-1)
                break;
        }
        if(i>=soliders[4]->xPos) //两个boss之间无棋子
        {
            if(redTurn)
                soliders[20]->dead=true;
            else
                soliders[4]->dead=true;
        }
    }
    //黑方胜
    if(soliders[4]->dead==true)
       {
            gameStart=false;
            QMessageBox::information(this,"祝贺","黑方取得胜利");
        }
    if(soliders[20]->dead==true)
       {
            gameStart=false;
            QMessageBox::information(this,"祝贺","红方取得胜利");
        }

    redTurn=!redTurn;
    update();

}
/* 得到黑色棋子所有能走的步数 */
void ChineseChess::getAllPossible()
{
    //所有黑方棋子 在整个棋盘进行遍历，看看棋子是否能走，如果可以。加入进去
    for(int i=16;i<32;i++)
    {
        for(int m=1;m<=10;++m)
            for(int n=1;n<=9;++n)
            {
                if(soliders[i]->dead==false)
                    if(judgeMove(soliders[i],QPoint(m,n))==true)
                    {
                        //这个点可以去，判断point点有没有棋子，如果有棋子，必然是相异色
                        int p=judgePoint(QPoint(m,n));
                        Step *step=new Step(i,QPoint(soliders[i]->xPos,soliders[i]->yPos),QPoint(m,n),p);
                        //如果point点可以过去，则当前棋子的可以走需要额外加分。
                        //soliders[i]->addScore+=getExtraScore(i);
                        allSteps.push_back(step);
                    }
            }
    }
}
/*得到红色棋子所有能走的步数（假设黑色走一步，红色的所有应对 */
void ChineseChess::getRedAllPossible()
{
    //清空红色可走的所有步数
    while(redAllSteps.count())
    {
        Step *step=redAllSteps.back();
        redAllSteps.removeLast();
        delete step;
    }

    for(int i=0;i<16;++i)
    {
        for(int m=1;m<=10;++m)
            for(int n=1;n<=9;++n)
            {
                if(soliders[i]->dead==false)
                    if(judgeMove(soliders[i],QPoint(m,n))==true)
                    {
                        //这个点可以去，判断point点有没有棋子，如果有棋子，必然是相异色
                        int p=judgePoint(QPoint(m,n));
                        Step *step=new Step(i,QPoint(soliders[i]->xPos,soliders[i]->yPos),QPoint(m,n),p);
                        //如果point点可以过去，则当前棋子的可以走需要额外加分。
                       // soliders[i]->addScore+=getExtraScore(i);
                        redAllSteps.push_back(step);
                    }
            }
    }
}
int ChineseChess::getExtraScore(int _id)
{
    int score=0;

    switch (_id)
    {
    case 0:
    case 8:
    case 16:
    case 24:

        score=CAR_EXRA;
        break;
    case 1:
    case 7:
    case 17:
    case 23:
        score=HORSE_EXRA;
        break;
    case 2:
    case 6:
        score=ELEPHANT_EXRA;
        break;
    case 18:
    case 22:
        score=ELEPHANT_EXRA;
        break;
    case 3:
    case 5:
        score=BISHOP_EXRA;
        break;
    case 19:
    case 21:
        score=BISHOP_EXRA;
        break;
    case 4:
        score=KING_EXRA;
        break;
    case 20:
        score=KING_EXRA;
        break;
    case 9:
    case 10:
        score=CANON_EXRA;
        break;
    case 25:
    case 26:
        score=CANON_EXRA;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        score=PAWN_EXRA;
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
        score=PAWN_EXRA;
        break;
    }
    return score;
}
void ChineseChess::tryStep(Step * step)
{
    if(step->killed!=-1)
        soliders[step->killed]->dead=true;  //将对方杀死
    //将棋子挪过去
    soliders[step->id]->xPos=step->target.x();
    soliders[step->id]->yPos=step->target.y();
    soliders[step->id]->pos=step->target;
}
void ChineseChess::tryRedStep(Step *)
{

}
void ChineseChess::resoreStep(Step * step)
{
    if(step->killed!=-1)
        soliders[step->killed]->dead=false; //棋子恢复
    //棋子挪回去
    soliders[step->id]->xPos=step->begin.x();
    soliders[step->id]->yPos=step->begin.y();
    soliders[step->id]->pos=step->begin;
}

int ChineseChess::getScore()
{
    //得到黑色棋子的分数
    int k=0;
    int w=0;
    for(int i=16;i<32;++i)
        if(soliders[i]->dead==false)
        {
            k+=soliders[i]->score;
         //   k+=soliders[i]->addScore;
        }
    //得到红色棋子的分数
    for(int i=0;i<16;++i)
        if(soliders[i]->dead==false)
        {
            w+=soliders[i]->score;
        //    w+=soliders[i]->addScore;
        }

    return k-w;  //返回黑色比红色棋子高的分数

}

ChineseChess::~ChineseChess()
{
    deleteQVector();
    delete ui;
}


void ChineseChess::on_btnRestart_clicked()
{
    //释放指针
    deleteQVector();
    for(int i=0;i<32;++i)
    {
        Solider *solider=new Solider(i);
        soliders.push_back(solider);

      //  qDebug()<<solider->xPos<<" "<<solider->yPos<<" "<<solider->id<<" "<<solider->name<<endl;
    }
    redTurn=true; //红先走

    gameStart=true;
    update();


}

void ChineseChess::on_rbRobot_clicked()
{
    if(this->ui->rbRobot->isChecked())
    {
        robotPlay=true;
        qDebug()<<"人机对战"<<endl;
    }
}

void ChineseChess::on_rbHuman_clicked()
{
    if(this->ui->rbHuman->isChecked())
    {
        robotPlay=false;
        qDebug()<<"人人对战"<<endl;
    }
}
