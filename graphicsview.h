#pragma once
#include<QGraphicsView>
#include<QDebug>
#include<QWheelEvent>

class GraphicsView:public QGraphicsView{
private:
    void wheelEvent(QWheelEvent *event){
        int delta=event->angleDelta().y();
        float factor=1.1;
        if(delta>0) scale(factor,factor);
        else scale(1/factor,1/factor);
    }
public:
    GraphicsView(QWidget*parent):QGraphicsView(parent){}
};
