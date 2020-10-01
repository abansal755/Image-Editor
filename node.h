#pragma once
#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QPainter>

class node:public QGraphicsItem{
    QGraphicsScene*scene;
    int width,height;
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
    node(QGraphicsScene*scene,int width=200,int height=75):scene(scene),width(width),height(height){
        scene->addItem(this);
        setFlags(QGraphicsItem::ItemIsMovable);
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
