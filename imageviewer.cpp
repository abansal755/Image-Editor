#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(viewerNode*viewNode,QWidget *parent) :QMainWindow(parent),ui(new Ui::ImageViewer),viewNode(viewNode)
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

void ImageViewer::on_pushButton_clicked()
{
    //Refresh Button
    if(viewNode->getInput()==NULL) return;
    if(viewNode->getInput()->imageCalculate(image)){
        scene->clear();
        scene->addPixmap(QPixmap::fromImage(image));
        graphicsView->centerOn(0,0);
    }
}
