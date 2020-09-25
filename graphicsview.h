#pragma once
#include<QGraphicsView>
#include<QDebug>
#include<QWheelEvent>
#include<QScrollBar>
#include<QMenu>

class ImageGraphicsView:public QGraphicsView{
private:
    void wheelEvent(QWheelEvent *event){
        int delta=event->angleDelta().y();
        float factor=1.1;
        if(delta>0) scale(factor,factor);
        if(delta<0)scale(1/factor,1/factor);
    }
    void contextMenuEvent(QContextMenuEvent *event){
        QMenu menu;
        QAction*centerView=menu.addAction("Center View");
        QAction*resetZoom=menu.addAction("Reset Zoom");
        QAction*toggleOverlay=menu.addAction("Toggle Overlay");
        QAction*current=menu.exec(event->globalPos());
        if(current==centerView){
            centerOn(0,0);
        }
        if(current==resetZoom){
            resetTransform();
        }
        if(current==toggleOverlay){
            overlay=overlay^1;
            update();
        }
    }
public:
    ImageGraphicsView(QWidget*parent=NULL):QGraphicsView(parent){
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(QGraphicsView::ScrollHandDrag);
        overlay=true;
        setBackgroundBrush(QBrush(QColor(38,38,38)));
    }
    bool overlay;
};

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
