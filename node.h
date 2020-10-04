#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QPainter>
#include<unordered_map>
#include<QGraphicsSceneMouseEvent>
#include<QDebug>
#include<QMenu>
#include"imagegraphicsview.h"
#include<unordered_set>

using namespace std;

enum inputConnection{noInput,oneInput,twoInput};
enum outputConnection{noOutput,oneOutput};
enum connectorState{hoverEnter,hoverExit,connected};
//hoverEnter - hover enter when not connected
//hovetExit - hover exit when not connected
//connected - when an edge is connected

class PropertiesWindow:public QWidget{
    Q_OBJECT
public:
    PropertiesWindow(QString title){
        setWindowTitle(title);
    }
};

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
        setToolTip("Right-click for creating edges\nMiddle-button for deleting edges");
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

class node:public QObject,public QGraphicsItem{
    Q_OBJECT
protected:
    QGraphicsScene*scene;
    int width,height;
    inputConnection iType;
    outputConnection oType;
    node*input1,*input2;
    QGraphicsPathItem*inputLine1,*inputLine2;
    unordered_multimap<node*,QGraphicsPathItem*>output;
    connector *ci1,*ci2,*co;
    QString name;
    PropertiesWindow*propW;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){};
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(184,15,10));
        grad.setColorAt(0.5,QColor(240,36,0));
        grad.setColorAt(1,QColor(184,15,10));
        painter->fillPath(path,grad);
        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        if(iType==oneInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/2-ci1->getWidth()/2-5,15);
            painter->drawText(rect1,Qt::AlignRight,"input");
        }else if(iType==twoInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/4-ci1->getWidth()/2-2,15);
            painter->drawText(rect1,Qt::AlignRight,"A");
            rect1.setWidth(3*width/4-ci2->getWidth()/2-2);
            painter->drawText(rect1,Qt::AlignRight,"B");
        }
        if(oType==oneOutput){
            QRectF rect1(0,0,width/2-co->getWidth()/2-5,height-co->getHeight()/2+2);
            painter->drawText(rect1,Qt::AlignRight|Qt::AlignBottom,"output");
        }
    };
    QVariant itemChange(GraphicsItemChange change, const QVariant &value){
        if(change==ItemPositionChange){
            QPointF p=value.toPointF();
            //inputLine
            if(iType==oneInput){
                if(input1!=NULL){
                    QPainterPath path;
                    path.moveTo(input1->x()+input1->getWidth()/2,input1->y()+input1->getHeight()-input1->getCO()->getHeight());
                    path.cubicTo(input1->x()+input1->getWidth()/2,input1->y()+input1->getHeight()-input1->getCO()->getHeight()+40,
                                 p.x()+width/2,p.y()+ci1->getHeight()-40,p.x()+width/2,p.y()+ci1->getHeight());
                    inputLine1->setPath(path);
                }
            }
            else if(iType==twoInput){
                if(input1!=NULL){
                    QPainterPath path;
                    path.moveTo(input1->x()+input1->getWidth()/2,input1->y()+input1->getHeight()-input1->getCO()->getHeight());
                    path.cubicTo(input1->x()+input1->getWidth()/2,input1->y()+input1->getHeight()-input1->getCO()->getHeight()+40,
                                 p.x()+width/4,p.y()+ci1->getHeight()-40,p.x()+width/4,p.y()+ci1->getHeight());
                    inputLine1->setPath(path);
                }
                if(input2!=NULL){
                    QPainterPath path;
                    path.moveTo(input2->x()+input2->getWidth()/2,input2->y()+input2->getHeight()-input2->getCO()->getHeight());
                    path.cubicTo(input2->x()+input2->getWidth()/2,input2->y()+input2->getHeight()-input2->getCO()->getHeight()+40,
                                 p.x()+3*width/4,p.y()+ci2->getHeight()-40,p.x()+3*width/4,p.y()+ci2->getHeight());
                    inputLine2->setPath(path);
                }
            }
            //outputLine
            for(auto it=output.begin();it!=output.end();it++){
                node*n=it->first;
                QGraphicsPathItem*pitem=it->second;
                if(n->iType==oneInput){
                    QPainterPath path;
                    path.moveTo(x()+width/2,y()+height-co->getHeight());
                    path.cubicTo(x()+width/2,y()+height-co->getHeight()+40,n->x()+n->width/2,n->y()+n->ci1->getHeight()-40,
                                 n->x()+n->width/2,n->y()+n->getCI1()->getHeight());
                    pitem->setPath(path);
                }else if(n->iType==twoInput){
                    if(n->inputLine1==pitem){
                        QPainterPath path;
                        path.moveTo(x()+width/2,y()+height-co->getHeight());
                        path.cubicTo(x()+width/2,y()+height-co->getHeight()+40,n->x()+n->width/4,n->y()+n->ci1->getHeight()-40,
                                     n->x()+n->width/4,n->y()+n->getCI1()->getHeight());
                        pitem->setPath(path);
                    }else{
                        QPainterPath path;
                        path.moveTo(x()+width/2,y()+height-co->getHeight());
                        path.cubicTo(x()+width/2,y()+height-co->getHeight()+40,n->x()+3*n->width/4,n->y()+n->ci1->getHeight()-40,
                                     n->x()+3*n->width/4,n->y()+n->getCI1()->getHeight());
                        pitem->setPath(path);
                    }
                }
            }
        }
        return QGraphicsItem::itemChange(change,value);
    };
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        QPointF rel=event->scenePos();
        rel.setX(rel.x()-x());
        rel.setY(rel.y()-y());
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
        if(!clicked && event->button()==Qt::RightButton){
            QMenu menu;
            QAction*deleteNode=menu.addAction("Delete Node");
            QAction*properties;
            if(propW!=NULL) properties=menu.addAction("Properties");
            QAction*current=menu.exec(event->screenPos());
            if(current==deleteNode){
                if(input1!=NULL) removeInput1();
                if(input2!=NULL) removeInput2();
                if(oType==oneOutput){
                    unordered_set<node*> set;
                    for(auto it=output.begin();it!=output.end();it++) set.insert(it->first);
                    removeAllOutputs();
                    for(auto it=set.begin();it!=set.end();it++) (*it)->refresh();
                }
                delete this;
            }
            if(current==properties){
                if(!(propW->isVisible())) propW->show();
                else propW->activateWindow();
            }
        }
        else if(clicked && event->button()==Qt::MiddleButton){
            if(connection.first.first==NULL){
                if(type=="ci1" && input1!=NULL){
                    removeInput1();
                    refresh();
                }
                else if(type=="ci2" && input2!=NULL){
                    removeInput2();
                    refresh();
                }
                else if(type=="co"){
                    unordered_set<node*> set;
                    for(auto it=output.begin();it!=output.end();it++) set.insert(it->first);
                    removeAllOutputs();
                    for(auto it=set.begin();it!=set.end();it++) (*it)->refresh();
                }
            }
        }
        else if(clicked && event->button()==Qt::RightButton){
            if(connection.first.first==NULL){
                connection.first={this,type};
                connection.second=new QGraphicsPathItem;
                QPen pen(Qt::white);
                pen.setWidth(2);
                connection.second->setPen(pen);
                scene->addItem(connection.second);
                connection.second->setZValue(-1);

                if(type=="ci1") ci1->setState(connected);
                else if(type=="ci2") ci2->setState(connected);
                else co->setState(connected);
            }else{
                node*&n=connection.first.first;
                QString&ctype=connection.first.second;
                QGraphicsPathItem*&pitem=connection.second;
                if((connection.first.second=="ci1" || connection.first.second=="ci2") && type=="co" && connection.first.first!=this){
                    qDebug()<<"valid";
                    output.insert({n,pitem});
                    if(n->iType==oneInput){
                        if(n->input1!=NULL) n->removeInput1();
                        n->input1=this;
                        n->inputLine1=pitem;
                        n->getCI1()->setState(connected);
                    }else if(n->iType==twoInput){
                        if(ctype=="ci1"){
                            if(n->input1!=NULL) n->removeInput1();
                            n->input1=this;
                            n->inputLine1=pitem;
                            n->getCI1()->setState(connected);
                        }else{
                            if(n->input2!=NULL) n->removeInput2();
                            n->input2=this;
                            n->inputLine2=pitem;
                            n->getCI2()->setState(connected);
                        }
                    }
                    n->refresh();
                    co->setState(connected);
                    n=NULL;
                    pitem=NULL;
                    ctype="";
                }else if(connection.first.second=="co" && (type=="ci1" || type=="ci2") && connection.first.first!=this){
                    qDebug()<<"valid";
                    n->output.insert({this,pitem});
                    if(iType==oneInput){
                        if(input1!=NULL) removeInput1();
                        input1=n;
                        inputLine1=pitem;
                        ci1->setState(connected);
                    }else if(iType==twoInput){
                        if(type=="ci1"){
                            if(input1!=NULL) removeInput1();
                            input1=n;
                            inputLine1=pitem;
                            ci1->setState(connected);
                        }else if(type=="ci2"){
                            if(input2!=NULL) removeInput2();
                            input2=n;
                            inputLine2=pitem;
                            ci2->setState(connected);
                        }
                    }
                    refresh();
                    n=NULL;
                    pitem=NULL;
                    ctype="";
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
        QGraphicsItem::mouseReleaseEvent(event);
    };
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event){
        pair<node*,QString>&p=node::connection.first;
        node*&n=p.first;
        QString&type=p.second;
        QGraphicsPathItem*&gpath=node::connection.second;
        if(n!=NULL){
            QPainterPath path;
            if(n->getIType()==oneInput){
                if(type=="ci1"){
                    path.moveTo(n->x()+n->getWidth()/2,n->y()+n->getCI1()->getHeight());
                    path.cubicTo(n->x()+n->getWidth()/2,n->y()+n->getCI1()->getHeight()-40,
                                 event->scenePos().x(),event->scenePos().y()+40,event->scenePos().x(),event->scenePos().y());
                }
            }else if(n->getIType()==twoInput){
                if(type=="ci1"){
                    path.moveTo(n->x()+n->getWidth()/4,n->y()+n->getCI1()->getHeight());
                    path.cubicTo(n->x()+n->getWidth()/4,n->y()+n->getCI1()->getHeight()-40,
                                 event->scenePos().x(),event->scenePos().y()+40,event->scenePos().x(),event->scenePos().y());
                }else if(type=="ci2"){
                    path.moveTo(n->x()+3*n->getWidth()/4,n->y()+n->getCI1()->getHeight());
                    path.cubicTo(n->x()+3*n->getWidth()/4,n->y()+n->getCI1()->getHeight()-40,
                                 event->scenePos().x(),event->scenePos().y()+40,event->scenePos().x(),event->scenePos().y());
                }
            }
            if(n->getOType()==oneOutput){
                if(type=="co"){
                    path.moveTo(n->x()+n->getWidth()/2,n->y()+n->getHeight()-n->getCO()->getHeight());
                    path.cubicTo(n->x()+n->getWidth()/2,n->y()+n->getHeight()-n->getCO()->getHeight()+40,
                                 event->scenePos().x(),event->scenePos().y()-40,event->scenePos().x(),event->scenePos().y());
                }
            }
            gpath->setPath(path);
        }
        QGraphicsItem::hoverMoveEvent(event);
    };
public:
    node(QGraphicsScene*scene,inputConnection iType=oneInput,outputConnection oType=oneOutput,QString name="node"+QString::number(lastIndex++),int width=200,int height=75)
        :scene(scene),width(width),height(height),iType(iType),oType(oType),input1(NULL),input2(NULL),inputLine1(NULL),inputLine2(NULL),name(name)
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
        propW=NULL;
        setToolTip("Right-click for context menu");
    }
    ~node(){
        if(propW!=NULL) delete propW;
    }
    QRectF boundingRect() const{
        return QRectF(0,0,width,height);
    };
    void removeInput1(){
        auto p=input1->output.equal_range(this);
        for(auto it=p.first;it!=p.second;){
            if(it->second==inputLine1){
                input1->output.erase(it);
                break;
            }
            it++;
        }
        if(input1->output.empty()) input1->getCO()->setState(hoverExit);
        input1=NULL;
        scene->removeItem(inputLine1);
        delete inputLine1;
        inputLine1=NULL;
        ci1->setState(hoverExit);
    }
    void removeInput2(){
        auto p=input2->output.equal_range(this);
        for(auto it=p.first;it!=p.second;){
            if(it->second==inputLine2){
                input2->output.erase(it);
                break;
            }
            it++;
        }
        if(input2->output.empty()) input2->getCO()->setState(hoverExit);
        input2=NULL;
        scene->removeItem(inputLine2);
        delete inputLine2;
        inputLine2=NULL;
        ci2->setState(hoverExit);
    }
    void removeAllOutputs(){
        for(auto it=output.begin();it!=output.end();){
            if(it->second==it->first->inputLine1) it->first->removeInput1();
            else it->first->removeInput2();
            it++;
        }
    }
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
    unordered_multimap<node*,QGraphicsPathItem*>* getOutput(){
        return &output;
    }
    node* getInput1(){
        return input1;
    }
    node* getInput2(){
        return input2;
    }
    virtual bool imageCalculate(QImage&image){
        return false;
    }
    float clampF(float i){
        if(i>1) return 1;
        else if(i<0) return 0;
        else return i;
    }
    int clamp(int i){
        if(i>255) return 255;
        else if(i<0) return 0;
        else return i;
    }
    static pair<pair<node*,QString>,QGraphicsPathItem*> connection;
    static int lastIndex;
public slots:
    virtual void refresh(){
        for(auto it=output.begin();it!=output.end();it++)it->first->refresh();
    }
};
