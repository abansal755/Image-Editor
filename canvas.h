#pragma once
#include<QGraphicsItem>
#include<QGraphicsSceneContextMenuEvent>
#include<QDebug>
#include<QMenu>
#include"node.h"

class canvas:public QGraphicsItem{
private:
    QGraphicsScene*scene;
    vector<node*>*destruc;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*newReadNode=menu.addAction("New Read Node");
        QAction*current=menu.exec(event->screenPos());
        if(current==newReadNode){
            readNode*n=new readNode(scene,*destruc);
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        delete newReadNode;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){}
public:
    canvas(QGraphicsScene*scene,vector<node*>*destruc):scene(scene),destruc(destruc){
        setCursor(Qt::ArrowCursor);
    }
    QRectF boundingRect() const{
        return QRectF(-10000,-10000,20000,20000);
    };
};
