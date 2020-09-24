#pragma once
#include<QGraphicsItem>
#include<QGraphicsSceneContextMenuEvent>
#include<QDebug>
#include<QMenu>
#include"node.h"
#include"graphicsview.h"

class canvas:public QGraphicsItem{
private:
    QGraphicsScene*scene;
    vector<node*>*destruc;
    GraphicsView*gw;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QMenu*newNode=menu.addMenu("New Node");
        QAction*newReadNode=newNode->addAction("Read Node");
        QAction*newGradeNode=newNode->addAction("Grade Node");
        QAction*newGammaNode=newNode->addAction("Gamma Node");
        QAction*newContrastNode=newNode->addAction("Contrast Node");
        QAction*newSaturateNode=newNode->addAction("Saturate Node");
        QAction*newBlurNode=newNode->addAction("Blur Node");
        QAction*centerView=menu.addAction("Center View");
        QAction*resetZoom=menu.addAction("Reset Zoom");
        QAction*current=menu.exec(event->screenPos());
        if(current==newReadNode){
            readNode*n=new readNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        if(current==newBlurNode){
            blurNode*n=new blurNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        if(current==newSaturateNode){
            saturateNode*n=new saturateNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        if(current==newContrastNode){
            contrastNode*n=new contrastNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        if(current==newGradeNode){
            gradeNode*n=new gradeNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        if(current==newGammaNode){
            gammaNode*n=new gammaNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        if(current==centerView){
            gw->centerOn(0,0);
        }
        if(current==resetZoom){
            gw->resetTransform();
        }
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        painter->fillRect(boundingRect(),QColor(38,38,38));
        QPen pen(QColor(52,52,52));
        painter->setPen(pen);
        for(int y=-10000;y<=10000;y+=25){
            if(y%200==0){
                pen.setColor(QColor(22,22,22));
                painter->setPen(pen);
            }
            painter->drawLine(-10000,y,10000,y);
            if(y%200==0){
                pen.setColor(QColor(52,52,52));
                painter->setPen(pen);
            }
        }
        for(int x=-10000;x<=10000;x+=25){
            if(x%200==0){
                pen.setColor(QColor(22,22,22));
                painter->setPen(pen);
            }
            painter->drawLine(x,-10000,x,10000);
            if(x%200==0){
                pen.setColor(QColor(52,52,52));
                painter->setPen(pen);
            }
        }
        setZValue(-2);
    }
public:
    canvas(GraphicsView*gw,QGraphicsScene*scene,vector<node*>*destruc):gw(gw),scene(scene),destruc(destruc){
        setCursor(Qt::ArrowCursor);
    }
    QRectF boundingRect() const{
        return QRectF(-10000,-10000,20000,20000);
    };
};
