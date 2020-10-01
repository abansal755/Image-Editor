#pragma once
#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QPainter>
#include<unordered_map>
#include<QGraphicsSceneMouseEvent>
#include<QDebug>

using namespace std;

enum connection{noInput,oneInput,twoInput,noOutput};
//noInput - 0 input, 1 output
//oneInput - 1 input, 1 output
//twoInput - 2 input, 1 output
//noOutput - 1 input, 0 output

enum connectorState{hoverEnter,hoverExit,connected};

class connector:public QGraphicsItem{
    int width,height;
    connectorState state;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event){
        if(state!=connected) state=hoverEnter;
        update();
    };
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
        if(state!=connected) state=hoverExit;
        update();
    };
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,2,2);
        QPen pen(Qt::white);
        painter->setPen(pen);
        painter->drawPath(path);
        if(state==hoverEnter || state==connected) painter->fillRect(rect,QBrush(QColor(200,200,200)));
    };
public:
    connector(QGraphicsItem*parent=NULL,int width=10,int height=10):QGraphicsItem(parent),width(width),height(height){
        setAcceptHoverEvents(true);
        state=hoverExit;
    }
    QRectF boundingRect() const{
        return QRectF(0,0,width,height);
    }
    int getWidth(){
        return width;
    }
    int getHeight(){
        return height;
    }
    void setState(connectorState state){
        this->state=state;
    }
};

class node:public QGraphicsItem{
protected:
    QGraphicsScene*scene;
    int width,height;
    connection type;
    node*input1,*input2;
    QGraphicsPathItem*inputLine1,*inputLine2;
    unordered_map<node*,QGraphicsPathItem*>output;
    connector *ci1,*ci2,*co;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){};
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(184,15,10));
        grad.setColorAt(0.5,QColor(225,36,0));
        grad.setColorAt(1,QColor(184,15,10));
        painter->fillPath(path,grad);
        QPen pen;pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
    };
public:
    node(QGraphicsScene*scene,connection type=oneInput,int width=200,int height=75)
        :scene(scene),width(width),height(height),type(type),input1(NULL),input2(NULL),inputLine1(NULL),inputLine2(NULL)
    {
        scene->addItem(this);
        setFlags(QGraphicsItem::ItemIsMovable);
        if(type==oneInput){
            ci1=new connector(this);
            ci1->setPos((width-ci1->getWidth())/2,ci1->getHeight()/2);
        }else if(type==twoInput){
            ci1=new connector(this);
            ci1->setPos((width-2*ci1->getWidth())/4,ci1->getHeight()/2);
            ci2=new connector(this);
            ci2->setPos((3*width-2*ci1->getWidth())/4,ci1->getHeight()/2);
        }
        if(type!=noOutput){
            co=new connector(this);
            co->setPos((width-co->getHeight())/2,height-(3*co->getHeight()/2));
        }
    }
    QRectF boundingRect() const{
        return QRectF(0,0,width,height);
    };
    int getWidth(){
        return width;
    }
    int getHeight(){
        return height;
    }
};
