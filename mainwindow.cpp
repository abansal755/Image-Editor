#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

pair<pair<node*,QString>,QGraphicsPathItem*> node::connection={{NULL,""},NULL};
int node::lastIndex=0;
int readNode::lastIndex=0;
int viewerNode::lastIndex=0;
int blurNode::lastIndex=0;
int saturateNode::lastIndex=0;
int contrastNode::lastIndex=0;

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
