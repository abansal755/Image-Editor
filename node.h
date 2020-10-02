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
    QVariant itemChange(GraphicsItemChange change, const QVariant &value){
        if(change==ItemPositionChange){

        }
        return QGraphicsItem::itemChange(change,value);
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
                    node*&n=connection.first.first;
                    QString&ctype=connection.first.second;
                    QGraphicsPathItem*&pitem=connection.second;
                    if((connection.first.second=="ci1" || connection.first.second=="ci2") && type=="co" && connection.first.first!=this){
                        qDebug()<<"valid";
                        output[n]=pitem;
                        QPainterPath path;
                        path.moveTo(x()+width/2,y()+height-co->getHeight());
                        if(n->iType==oneInput){
                            n->input1=this;
                            n->inputLine1=pitem;
                            path.cubicTo(x()+width/2,y()+height-co->getHeight()+25,n->x()+n->getWidth()/2,
                                         n->y()+n->getCI1()->getHeight()-25,n->x()+n->getWidth()/2,n->y()+n->getCI1()->getHeight());
                        }else if(n->iType==twoInput){
                            if(ctype=="ci1"){
                                n->input1=this;
                                n->inputLine1=pitem;
                                path.cubicTo(x()+width/2,y()+height-co->getHeight()+25,n->x()+n->getWidth()/4,
                                             n->y()+n->ci1->getHeight()-25,n->x()+n->getWidth()/4,n->y()+n->getCI1()->getHeight());
                            }else{
                                n->input2=this;
                                n->inputLine2=pitem;
                                path.cubicTo(x()+width/2,y()+height-co->getHeight()+25,n->x()+3*n->getWidth()/4,
                                             n->y()+n->ci1->getHeight()-25,n->x()+3*n->getWidth()/4,n->y()+n->getCI1()->getHeight());
                            }
                        }
                        co->setState(connected);
                        n=NULL;
                        pitem=NULL;
                        ctype="";
                    }else if(connection.first.second=="co" && (type=="ci1" || type=="ci2") && connection.first.first!=this){
                        qDebug()<<"valid";
                        QPainterPath path;
                        path.moveTo(n->x()+n->width/2,n->y()+n->height-n->co->getHeight());
                        n->output[this]=pitem;
                        if(iType==oneInput){
                            input1=n;
                            inputLine1=pitem;
                            path.cubicTo(n->x()+n->width/2,n->y()+n->height-n->co->getHeight()+25,x()+width/2,
                                         y()-ci1->getHeight()-25,x()+width/2,y()-ci1->getHeight()-25);
                        }else if(iType==twoInput){
                            if(type=="ci1"){
                                input1=n;
                                inputLine1=pitem;
                                path.cubicTo(n->x()+n->width/2,n->y()+n->height-n->co->getHeight()+25,x()+width/4,
                                             y()-ci1->getHeight()-25,x()+width/4,y()-ci1->getHeight()-25);
                            }else if(type=="ci2"){
                                input2=n;
                                inputLine2=pitem;
                                path.cubicTo(n->x()+n->width/2,n->y()+n->height-n->co->getHeight()+25,x()+3*width/4,
                                             y()-ci1->getHeight()-25,x()+3*width/4,y()-ci1->getHeight()-25);
                            }
                        }
                    }else{
                        qDebug()<<"invalid";
                        if(ctype=="ci1") n->getCI1()->setState(hoverExit);
                        else if(ctype=="ci2") n->getCI2()->setState(hoverExit);
                        else n->getCO()->setState(hoverExit);
                        n=NULL;
                        scene->removeItem(pitem);
                        ctype="";
                    }
                }
            }
        }
        QGraphicsItem::mouseReleaseEvent(event);
    };
public:
    node(QGraphicsScene*scene,inputConnection iType=twoInput,outputConnection oType=oneOutput,int width=200,int height=75)
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
