#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include "graphicsview.h"
#include "node.h"

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
    ImageGraphicsView(QWidget*parent):QGraphicsView(parent){
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(QGraphicsView::ScrollHandDrag);
        overlay=true;
        setBackgroundBrush(QBrush(QColor(38,38,38)));
    }
    bool overlay;
};

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(viewerNode*viewNode,QWidget *parent = nullptr);
    ~ImageViewer();
public slots:
    void on_pushButton_clicked();
private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ImageViewer *ui;
    QGraphicsScene*scene;
    QImage image;
    viewerNode*viewNode;
};

#endif // IMAGEVIEWER_H
