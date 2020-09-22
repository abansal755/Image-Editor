#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include "graphicsview.h"
#include "node.h"

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
    GraphicsView*graphicsView;
    Ui::ImageViewer *ui;
    QGraphicsScene*scene;
    QImage image;
    viewerNode*viewNode;
};

#endif // IMAGEVIEWER_H
