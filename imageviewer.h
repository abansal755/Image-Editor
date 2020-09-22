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
        else scale(1/factor,1/factor);
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
    ImageGraphicsView(QWidget*parent):QGraphicsView(parent){
        horizontalScrollBar()->hide();
        verticalScrollBar()->hide();
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
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
private slots:
    void on_pushButton_clicked();
private:
    Ui::ImageViewer *ui;
    QGraphicsScene*scene;
    QImage image;
    viewerNode*viewNode;
};

#endif // IMAGEVIEWER_H
