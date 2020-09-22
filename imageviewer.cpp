#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(viewerNode*viewNode,QWidget *parent) :QMainWindow(parent),ui(new Ui::ImageViewer),viewNode(viewNode)
{
    ui->setupUi(this);
    scene=new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::on_pushButton_clicked()
{
    //Refresh Button
    scene->clear();
    if(viewNode->getInput()==NULL) return;
    if(viewNode->getInput()->imageCalculate(image)){
        QGraphicsPixmapItem*pix=scene->addPixmap(QPixmap::fromImage(image));
        pix->setPos(-(float)image.width()/2,-(float)image.height()/2);
        ui->graphicsView->setSceneRect(-(float)image.width()/2,-(float)image.height()/2,image.width(),image.height());
        ui->graphicsView->centerOn(0,0);
    }
}
