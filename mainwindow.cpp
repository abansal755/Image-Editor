#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

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

void MainWindow::on_actionExit_triggered()
{
    //Exit Menu Button
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    //About Menu Button
    AboutDialog dialog;
    dialog.exec();
}
