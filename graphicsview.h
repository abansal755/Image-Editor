#pragma once
#include<QGraphicsView>
#include<QWheelEvent>
#include"node.h"
#include<QGraphicsSceneContextMenuEvent>
#include<QMenu>

class GraphicsView:public QGraphicsView{
    void wheelEvent(QWheelEvent *event){
        int delta=event->angleDelta().y();
        float factor=1.1;
        if(delta>0) scale(factor,factor);
        if(delta<0)scale(1/factor,1/factor);
    }
public:
    GraphicsView(QWidget*parent=NULL):QGraphicsView(parent){
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setRenderHint(QPainter::Antialiasing);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setBackgroundBrush(QBrush(QColor(38,38,38)));
    }
};

class canvas:public QGraphicsItem{
    QGraphicsScene*scene;
    GraphicsView*gw;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
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
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*centerView=menu.addAction("Center View");
        QAction*resetZoom=menu.addAction("Reset Zoom");
        QAction*newNode=menu.addAction("New Node");
        QAction*current=menu.exec(event->screenPos());
        if(current==centerView){
            gw->centerOn(0,0);
        }
        if(current==resetZoom){
            gw->resetTransform();
        }
        if(current==newNode){
            node*n=new node(scene);
            n->setPos(event->scenePos());
        }
    };
public:
    canvas(QGraphicsScene*scene,GraphicsView*gw):scene(scene),gw(gw){
        setCursor(Qt::ArrowCursor);
        scene->addItem(this);
    }
    QRectF boundingRect() const{
        return QRectF(-10000,-10000,20000,20000);
    };
};
