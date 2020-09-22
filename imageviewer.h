#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include "graphicsview.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
    GraphicsView*graphicsView;
private:
    Ui::ImageViewer *ui;
    QGraphicsScene*scene;
};

#endif // IMAGEVIEWER_H
