#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    graphicsView=ui->graphicsView;
    scene=new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
