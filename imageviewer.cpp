#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <algorithm>
#include <QFileDialog>
#include "qualityfactordialog.h"

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

void ImageViewer::on_pushButton_2_clicked()
{
    //Save Image Button
    QString fileName=QFileDialog::getSaveFileName(this,"Save Image File","","Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");
    QFileInfo info(fileName);
    if(info.suffix()=="") QMessageBox::critical(this,"Error","Unable to save image file");
    else{
        qualityFactorDialog d;
        bool ans=d.exec();
        if(!ans) QMessageBox::critical(this,"Error","Unable to save image file");
        else image.save(fileName,NULL,d.getFactor());
    }
}
