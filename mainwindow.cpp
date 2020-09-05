#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "image.h"
#include "aboutdialog.h"

#define RESET_ZOOM ui->horizontalSlider_3->setValue(500)

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
}

MainWindow::~MainWindow(){
    delete ui;
    delete scene;
}


void MainWindow::on_pushButton_clicked()
{
    int radius=ui->horizontalSlider->value();
    float saturate_v=(float)(ui->horizontalSlider_2->value())/100;
    if(radius!=0) blur(image,radius);
    if(saturate_v!=0) saturate(image,saturate_v);
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_actionOpen_image_triggered()
{
    QString fileName=QFileDialog::getOpenFileName(this,"Open Image","",fileExtensions);
    bool res=image.load(fileName);
    if(res){
        delete scene;
        scene=new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        scene->addPixmap(QPixmap::fromImage(image));
        ui->pushButton->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->horizontalSlider_2->setEnabled(true);
        ui->horizontalSlider->setValue(0);
        ui->horizontalSlider_2->setValue(0);
        RESET_ZOOM;
    }else QMessageBox::critical(this,"Error","Unable to open image file");
}

void MainWindow::on_actionSave_Image_triggered()
{
    QString fileName=QFileDialog::getSaveFileName(this,"Save Image","",fileExtensions);
    bool res=image.save(fileName);
    if(!res) QMessageBox::critical(this,"Error","Unable to save the image file");
}

void MainWindow::on_actionClose_Image_triggered()
{
    scene->clear();
    ui->pushButton->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);
    ui->horizontalSlider_2->setDisabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    float scale=(float)value/500;
    ui->graphicsView->resetMatrix();
    ui->graphicsView->scale(scale,scale);
}

void MainWindow::on_pushButton_2_clicked()
{
    RESET_ZOOM;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog d;
    d.exec();
}
