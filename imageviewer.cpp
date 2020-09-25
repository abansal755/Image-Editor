#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <algorithm>

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
        if(ui->graphicsView->overlay){
            QPen pen;
            pen.setColor(Qt::red);
            scene->addLine(-(float)image.width()/2,(float)image.height()/2,(float)image.width()/2,(float)image.height()/2,pen);
            scene->addLine(-(float)image.width()/2,-(float)image.height()/2,(float)image.width()/2,-(float)image.height()/2,pen);
            scene->addLine(-(float)image.width()/2,(float)image.height()/2,-(float)image.width()/2,-(float)image.height()/2,pen);
            scene->addLine((float)image.width()/2,(float)image.height()/2,(float)image.width()/2,-(float)image.height()/2,pen);
            QFont font;
            font.setPixelSize(max(image.width(),image.height())/40);
            QGraphicsTextItem*text=scene->addText(QString::number(image.width())+"x"+QString::number(image.height()),font);
            text->setDefaultTextColor(Qt::red);
            text->setPos((float)image.width()/2,(float)image.height()/2);
        }
        ui->graphicsView->centerOn(0,0);
    }
}
