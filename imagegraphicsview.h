#pragma once
#include<QGraphicsView>
#include<QWheelEvent>
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
        QAction*current=menu.exec(event->globalPos());
        if(current==centerView){
            centerOn(0,0);
        }
        if(current==resetZoom){
            resetTransform();
        }
    }
public:
    ImageGraphicsView(QWidget*parent=NULL):QGraphicsView(parent){
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setBackgroundBrush(QBrush(QColor(38,38,38)));
    }
};
