#pragma once
#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QPainter>
#include<QDebug>
#include<QMenu>
#include<QGraphicsSceneContextMenuEvent>
#include<QPushButton>
#include<QFileDialog>
#include<QMessageBox>

using namespace std;

class PropertiesWindow:public QWidget{
protected:
    static QString fileExtensions;
public:
    PropertiesWindow(QString title,QWidget*parent=NULL):QWidget(parent){
        setWindowTitle(title);
    }
};

class ReadNodePropertiesWindow:public PropertiesWindow{
private:
    Q_OBJECT
    QString fileName;
    QPushButton*pushButton1;
public:
    ReadNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        setMinimumSize(400,300);
        pushButton1=new QPushButton("Read Image File",this);
        connect(pushButton1,SIGNAL(clicked()),this,SLOT(pushButton1Clicked()));
    }
private slots:
    void pushButton1Clicked(){
        fileName=QFileDialog::getOpenFileName(this,"Read Image File","",fileExtensions);
        QFileInfo info(fileName);
        if(info.suffix()=="") QMessageBox::critical(this,"Error","Invalid File");
    }
};

class node:public QGraphicsItem{
protected:
    QString name;
    int width,height;
    node*input,*output;
    QGraphicsLineItem*inputLine,*outputLine;
    QGraphicsScene*scene;
    bool pressed;
    PropertiesWindow*propW;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(184,15,10));
        grad.setColorAt(0.5,QColor(225,36,0));
        grad.setColorAt(1,QColor(184,15,10));
        painter->fillRect(rect,grad);
        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(rect);
        QFont font;
        font.setPixelSize(40);
        painter->setFont(font);
        if(!pressed) pen.setColor(Qt::black);
        else pen.setColor(QColor(246,228,134));
        painter->setPen(pen);
        painter->drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignHCenter|Qt::AlignTop,"input");
        painter->drawText(rect,Qt::AlignHCenter|Qt::AlignBottom,"output");

    };
    QVariant itemChange(GraphicsItemChange change, const QVariant &value){
        if(change==ItemPositionChange){
            //p1 is output point
            //p2 is input point
            if(input!=NULL){
                QPointF p1(input->x()+input->width/2,input->y()+input->height);
                QPointF p2=value.toPointF();
                p2.setX(p2.x()+width/2);
                inputLine->setLine(QLineF(p1,p2));
            }
            if(output!=NULL){
                QPointF p1=value.toPointF();
                p1.setX(p1.x()+width/2);
                p1.setY(p1.y()+height);
                QPointF p2(output->x()+output->width/2,output->y());
                outputLine->setLine(QLineF(p1,p2));
            }
        }
        return QGraphicsItem::itemChange(change,value);
    };
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        pressed=true;
        setZValue(1);
        update();
        return QGraphicsItem::mousePressEvent(event);
    };
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        pressed=false;
        setZValue(0);
        update();
        return QGraphicsItem::mouseReleaseEvent(event);
    };
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*connectOutput=menu.addAction("Connect Output");
        QAction*connectInput=menu.addAction("Connect Input");
        QAction*disconnectOutput=menu.addAction("Disconnect Output");
        QAction*disconnectInput=menu.addAction("Disconnect Input");
        QAction*deleteNode=menu.addAction("Delete Node");
        QAction*properties=menu.addAction("Properties");
        QAction*current=menu.exec(event->screenPos());
        if(current==connectOutput){
            if(inputScene!=NULL && inputScene!=this){
                if(output!=NULL) removeOutput();
                if(inputScene->input!=NULL) inputScene->removeInput();
                setOutput(inputScene);
                inputScene=NULL;
            }
            else outputScene=this;
        }
        if(current==connectInput){
            if(outputScene!=NULL && outputScene!=NULL){
                if(input!=NULL) removeInput();
                if(outputScene->output!=NULL) outputScene->removeOutput();
                setInput(outputScene);
                outputScene=NULL;
            }
            else inputScene=this;
        }
        if(current==disconnectInput){
            if(input!=NULL) removeInput();
            inputScene=NULL;
            outputScene=NULL;
        }
        if(current==disconnectOutput){
            if(output!=NULL) removeOutput();
            inputScene=NULL;
            outputScene=NULL;
        }
        if(current==deleteNode){
            if(input!=NULL) removeInput();
            if(output!=NULL) removeOutput();
            scene->removeItem(this);
            inputScene=NULL;
            outputScene=NULL;
        }
        if(current==properties){
            propW->show();
        }
        delete connectInput;
        delete connectOutput;
        delete  disconnectInput;
        delete disconnectOutput;
        delete deleteNode;
        delete properties;
    };
public:
    node(QGraphicsScene*scene,vector<node*>&destruc,QString name="node"+QString::number(lastIndex++),int width=200,int height=75)
        :scene(scene),name(name),width(width),height(height),
        input(NULL),output(NULL),inputLine(NULL),outputLine(NULL)
    {
        destruc.push_back(this);
        setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemSendsScenePositionChanges);
        pressed=false;
    }
    ~node(){
        if(inputLine!=NULL){
            input->outputLine=NULL;
            delete inputLine;
        }
        if(outputLine!=NULL){
            output->inputLine=NULL;
            delete outputLine;
        }
        delete propW;
    }
    QRectF boundingRect() const{
        return QRectF(0,0,width,height);
    };
    void setInput(node*input){
        this->input=input;
        input->output=this;
        QPen pen;
        pen.setWidth(2);
        inputLine=scene->addLine(input->x()+input->width/2,input->y()+input->height,x()+width/2,y(),pen);
        inputLine->setZValue(-1);
        input->outputLine=inputLine;
    }
    void removeInput(){
        scene->removeItem(inputLine);
        delete inputLine;
        inputLine=NULL;
        input->output=NULL;
        input->outputLine=NULL;
        input=NULL;
    }
    void setOutput(node*output){
        this->output=output;
        output->input=this;
        QPen pen;
        pen.setWidth(2);
        outputLine=scene->addLine(x()+width/2,+y()+height,output->x()+output->width/2,output->y(),pen);
        outputLine->setZValue(-1);
        output->inputLine=outputLine;
    }
    void removeOutput(){
        scene->removeItem(outputLine);
        delete outputLine;
        outputLine=NULL;
        output->input=NULL;
        output->inputLine=NULL;
        output=NULL;
    }
    void setName(QString name){
        this->name=name;
    }
    QString getName(){
        return name;
    }
    node*getInput(){
        return input;
    }
    node*getOutput(){
        return output;
    }
    static int lastIndex;
    static node*inputScene,*outputScene;
};

class readNode:public node{
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
            QRectF rect=boundingRect();
            QLinearGradient grad(0,0,0,height);
            grad.setColorAt(0,QColor(184,15,10));
            grad.setColorAt(0.5,QColor(225,36,0));
            grad.setColorAt(1,QColor(184,15,10));
            painter->fillRect(rect,grad);
            QPen pen;
            pen.setWidth(2);
            painter->setPen(pen);
            painter->drawRect(rect);
            QFont font;
            font.setPixelSize(40);
            painter->setFont(font);
            if(!pressed) pen.setColor(Qt::black);
            else pen.setColor(QColor(246,228,134));
            painter->setPen(pen);
            painter->drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,name);
            font.setPixelSize(10);
            painter->setFont(font);
            painter->drawText(rect,Qt::AlignHCenter|Qt::AlignBottom,"output");

        };
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
            QMenu menu;
            QAction*connectOutput=menu.addAction("Connect Output");
            QAction*disconnectOutput=menu.addAction("Disconnect Output");
            QAction*deleteNode=menu.addAction("Delete Node");
            QAction*properties=menu.addAction("Properties");
            QAction*current=menu.exec(event->screenPos());
            if(current==connectOutput){
                if(inputScene!=NULL && inputScene!=this){
                    if(output!=NULL) removeOutput();
                    if(inputScene->getInput()!=NULL) inputScene->removeInput();
                    setOutput(inputScene);
                    inputScene=NULL;
                }
                else outputScene=this;
            }
            if(current==disconnectOutput){
                if(output!=NULL) removeOutput();
                inputScene=NULL;
                outputScene=NULL;
            }
            if(current==deleteNode){
                if(input!=NULL) removeInput();
                if(output!=NULL) removeOutput();
                scene->removeItem(this);
                inputScene=NULL;
                outputScene=NULL;
            }
            if(current==properties){
                propW->show();
            }
            delete connectOutput;
            delete disconnectOutput;
            delete deleteNode;
            delete properties;
        };
public:
    readNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="readNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        propW=new ReadNodePropertiesWindow(name);
    }
    static int lastIndex;
};
