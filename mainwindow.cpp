#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    canvas*canv=new canvas(scene,ui->graphicsView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

