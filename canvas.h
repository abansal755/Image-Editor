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
        QAction*newReadNode=menu.addAction("New Read Node");
        QAction*newBlurNode=menu.addAction("New Blur Node");
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
        if(current==centerView){
            gw->centerOn(0,0);
        }
        if(current==resetZoom){
            gw->resetTransform();
        }
        delete newReadNode;
        delete newBlurNode;
        delete centerView;
        delete resetZoom;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){}
public:
    canvas(GraphicsView*gw,QGraphicsScene*scene,vector<node*>*destruc):gw(gw),scene(scene),destruc(destruc){
        setCursor(Qt::ArrowCursor);
    }
    QRectF boundingRect() const{
        return QRectF(-10000,-10000,20000,20000);
    };
};
