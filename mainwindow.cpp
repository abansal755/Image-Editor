#include <vector>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "math.h"
#include "image.h"
#include "aboutdialog.h"
#include "qualityfactordialog.h"

void MainWindow::setDisabled(){
    for(int i=0;i<statusChanging.size();i++) statusChanging[i]->setDisabled(true);
}

void MainWindow::setEnabled(){
    for(int i=0;i<statusChanging.size();i++) statusChanging[i]->setEnabled(true);
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView:: ScrollHandDrag);

    //push_back all objects which are to be disabled if no image is opened and enabled if an image is opened
    statusChanging.push_back(ui->pushButton);
    statusChanging.push_back(ui->pushButton_3);
    statusChanging.push_back(ui->pushButton_4);
    statusChanging.push_back(ui->pushButton_5);
    statusChanging.push_back(ui->pushButton_6);
    statusChanging.push_back(ui->pushButton_7);
    statusChanging.push_back(ui->pushButton_8);
    statusChanging.push_back(ui->horizontalSlider);
    statusChanging.push_back(ui->horizontalSlider_2);
    statusChanging.push_back(ui->horizontalSlider_4);
    statusChanging.push_back(ui->horizontalSlider_5);
    statusChanging.push_back(ui->horizontalSlider_6);
    statusChanging.push_back(ui->horizontalSlider_7);
    statusChanging.push_back(ui->horizontalSlider_8);

    ui->actionSave_Image->setDisabled(true);
    ui->actionGraph->setDisabled(true);
    setDisabled();

    gw=new GraphWindow(this);
    gw->image=&image;
    gw->copy=&copy;

    connect(this,SIGNAL(refreshSignal()),this,SLOT(refresh()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::refresh(){
    int radius=ui->horizontalSlider->value();
    float saturate_v=(float)(ui->horizontalSlider_2->value())/100;
    float gamma=(float)(ui->horizontalSlider_4->value())/100;
    float lift=(float)(ui->horizontalSlider_5->value())/100;
    float gain=(float)(ui->horizontalSlider_6->value())/100;
    float offset=(float)(ui->horizontalSlider_7->value())/100;
    float contrast_v=ui->horizontalSlider_8->value();
    image=copy;
    grade(image,lift,gain,offset);
    gammaCorrect(image,gamma);
    saturate(image,saturate_v);
    contrast(image,contrast_v);
    blur(image,radius);
    for(int y=0;y<image.height();y++){
        for(int x=0;x<image.width();x++){
            QColor ci=image.pixelColor(x,y);
            QColor cc=copy.pixelColor(x,y);
            ci.setAlpha(cc.alpha());
            image.setPixelColor(x,y,ci);
        }
    }
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_pushButton_clicked()
{
    //Apply
    refresh();
}

void MainWindow::on_actionOpen_image_triggered()
{
    //Open Image
    QString fileName=QFileDialog::getOpenFileName(this,"Open Image","",fileExtensions);
    bool res=image.load(fileName);
    if(res){
        copy=image;
        scene->clear();
        scene->addPixmap(QPixmap::fromImage(image));
        ui->actionSave_Image->setEnabled(true);
        ui->actionGraph->setEnabled(true);
        setEnabled();
        ui->horizontalSlider->setValue(0);
        ui->horizontalSlider_2->setValue(0);
        ui->horizontalSlider_4->setValue(100);
        ui->horizontalSlider_5->setValue(0);
        ui->horizontalSlider_6->setValue(100);
        ui->horizontalSlider_7->setValue(0);
        ui->horizontalSlider_8->setValue(0);
        ui->graphicsView->resetTransform();
    }else QMessageBox::critical(this,"Error","Unable to open image file");
}

void MainWindow::on_actionSave_Image_triggered()
{
    //Save Image
    QString fileName=QFileDialog::getSaveFileName(this,"Save Image","",fileExtensions);
    QFileInfo fi(fileName);
    if(fi.suffix()=="") QMessageBox::critical(this,"Error","Unable to save the image file");
    else{
        QualityFactorDialog d;
        bool res=d.exec();
        if(!res) QMessageBox::critical(this,"Error","Unable to save the image file");
        else image.save(fileName,"",d.qualityFactor);
    }
}

void MainWindow::on_actionClose_Image_triggered()
{
    //Close Image
    scene->clear();
    setDisabled();
    ui->actionSave_Image->setDisabled(true);
    ui->actionGraph->setDisabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    //Exit
    QApplication::quit();
}

void MainWindow::on_pushButton_3_clicked()
{
    //Reset
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_4->setValue(100);
    ui->horizontalSlider_5->setValue(0);
    ui->horizontalSlider_6->setValue(100);
    ui->horizontalSlider_7->setValue(0);
    ui->horizontalSlider_8->setValue(0);
    image=copy;
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    //Blur slider
    ui->spinBox->setValue(value);
    emit refreshSignal();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    //Saturate slider
    ui->doubleSpinBox->setValue((float)value/100);
    emit refreshSignal();
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    //Gamma slider
    ui->doubleSpinBox_2->setValue((float)value/100);
    emit refreshSignal();
}

void MainWindow::on_pushButton_4_clicked()
{
    //RotateClockwise90
    rotateClockwise90(image);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_pushButton_5_clicked()
{
    //RotateAntiClockwise90
    rotateAntiClockwise90(image);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_pushButton_6_clicked()
{
    //Rotate180
    rotate180(image);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    //Lift slider
    ui->doubleSpinBox_3->setValue((float)value/100);
    emit refreshSignal();
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    //Gain slider
    ui->doubleSpinBox_4->setValue((float)value/100);
    emit refreshSignal();
}

void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
    //Offset slider
    ui->doubleSpinBox_5->setValue((float)value/100);
    emit refreshSignal();
}

void MainWindow::on_horizontalSlider_8_valueChanged(int value)
{
    //Contrast slider
    ui->spinBox_2->setValue(value);
    emit refreshSignal();
}

void MainWindow::on_pushButton_7_clicked()
{
    //mirrorimage horizontal
    mirror_imgH(image);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_pushButton_8_clicked()
{
    //mirrorimage vertical
    mirror_imgV(image);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_actionGraph_triggered()
{
    //Graph
    gw->show();
}

void MainWindow::on_actionAbout_2_triggered()
{
    //About
    AboutDialog d;
    d.exec();
}
