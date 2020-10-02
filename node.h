#pragma once
#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QPainter>
#include<unordered_map>
#include<QGraphicsSceneMouseEvent>
#include<QDebug>

using namespace std;

enum inputConnection{noInput,oneInput,twoInput};
enum outputConnection{noOutput,oneOutput};
enum connectorState{hoverEnter,hoverExit,connected};
//hoverEnter - hover enter when not connected
//hovetExit - hover exit when not connected
//connected - when an edge is connected

class connector:public QGraphicsItem{
    int width,height;
    connectorState state;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event){
        if(state!=connected) state=hoverEnter;
        update();
        QGraphicsItem::hoverEnterEvent(event);
    };
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
        if(state!=connected) state=hoverExit;
        update();
        QGraphicsItem::hoverLeaveEvent(event);
    };
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,2,2);
        QPen pen(Qt::white);
        painter->setPen(pen);
        painter->drawPath(path);
        if(state==hoverEnter) painter->fillRect(rect,QBrush(QColor(200,200,200)));
        else if(state==connected) painter->fillRect(rect,QBrush(Qt::white));
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
        update();
    }
};

class node:public QGraphicsItem{
protected:
    QGraphicsScene*scene;
    int width,height;
    inputConnection iType;
    outputConnection oType;
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
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        QPointF rel=event->scenePos();
        rel.setX(rel.x()-x());
        rel.setY(rel.y()-y());
        if(event->button()==Qt::RightButton){
            QString type;
            bool clicked=false;
            if(iType==oneInput){
                if(rel.x()>=(width-ci1->getWidth())/2 && rel.x()<=(width+ci1->getWidth())/2
                        && rel.y()>=ci1->getHeight()/2 && rel.y()<=3*ci1->getHeight()/2)
                {
                    qDebug()<<"ci1";
                    type="ci1";
                    clicked=true;
                }
            }else if(iType==twoInput){
                if(rel.x()>=(width-2*ci1->getWidth())/4 && rel.x()<=(width+2*ci1->getWidth())/4
                        && rel.y()>=ci1->getHeight()/2 && rel.y()<=3*ci1->getHeight()/2)
                {
                    qDebug()<<"ci1";
                    type="ci1";
                    clicked=true;
                }
                else if(rel.x()>=(3*width-2*ci1->getWidth())/4 && rel.x()<=(3*width+2*ci1->getWidth())/4
                         && rel.y()>=ci1->getHeight()/2 && rel.y()<=3*ci1->getHeight()/2)
                {
                    qDebug()<<"ci2";
                    type="ci2";
                    clicked=true;
                }
            }
            if(oType==oneOutput){
                if(rel.x()>=(width-co->getWidth())/2 && rel.x()<=(width+co->getWidth())/2
                        && rel.y()<=height-(co->getHeight()/2) && rel.y()>=height-(3*co->getHeight()/2))
                {
                    qDebug()<<"co";
                    type="co";
                    clicked=true;
                }
            }
            if(clicked){
                if(connection.first.first==NULL){
                    connection.first={this,type};
                    connection.second=new QGraphicsPathItem;
                    QPen pen(Qt::white);
                    pen.setWidth(2);
                    connection.second->setPen(pen);
                    scene->addItem(connection.second);

                    if(type=="ci1") ci1->setState(connected);
                    else if(type=="ci2") ci2->setState(connected);
                    else co->setState(connected);
                }else{

                }
            }
        }
        QGraphicsItem::mouseReleaseEvent(event);
    };
public:
    node(QGraphicsScene*scene,inputConnection iType=oneInput,outputConnection oType=oneOutput,int width=200,int height=75)
        :scene(scene),width(width),height(height),iType(iType),oType(oType),input1(NULL),input2(NULL),inputLine1(NULL),inputLine2(NULL)
    {
        scene->addItem(this);
        setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemSendsScenePositionChanges);
        setAcceptHoverEvents(true);
        if(iType==oneInput){
            ci1=new connector(this);
            ci1->setPos((width-ci1->getWidth())/2,ci1->getHeight()/2);
        }else if(iType==twoInput){
            ci1=new connector(this);
            ci1->setPos((width-2*ci1->getWidth())/4,ci1->getHeight()/2);
            ci2=new connector(this);
            ci2->setPos((3*width-2*ci1->getWidth())/4,ci1->getHeight()/2);
        }
        if(oType==oneOutput){
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
    inputConnection getIType(){
        return iType;
    }
    outputConnection getOType(){
        return oType;
    }
    connector* getCI1(){
        return ci1;
    }
    connector* getCI2(){
        return ci2;
    }
    connector* getCO(){
        return co;
    }
    static pair<pair<node*,QString>,QGraphicsPathItem*> connection;
};

pair<pair<node*,QString>,QGraphicsPathItem*> node::connection={{NULL,""},NULL};
