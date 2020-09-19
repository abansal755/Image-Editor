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
        QAction*newNode=menu.addAction("New Node");
        QAction*current=menu.exec(event->screenPos());
        if(current==newNode){
            node*n=new node(scene,*destruc,"node"+QString::number(node::lastIndex++));
            scene->addItem(n);
            n->setPos(event->scenePos());
        }
        delete newNode;
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
