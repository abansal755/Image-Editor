#pragma once
#include<QGraphicsView>
#include<QDebug>
#include<QWheelEvent>
#include<QScrollBar>

class GraphicsView:public QGraphicsView{
private:
    void wheelEvent(QWheelEvent *event){
        int delta=event->angleDelta().y();
        float factor=1.1;
        if(delta>0) scale(factor,factor);
        if(delta<0 && transform().m11()>0.1)scale(1/factor,1/factor);
    }
public:
    GraphicsView(QWidget*parent):QGraphicsView(parent){
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setRenderHint(QPainter::Antialiasing);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setBackgroundBrush(QBrush(QColor(38,38,38)));
    }
};
