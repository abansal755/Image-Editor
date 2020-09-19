#include "mainwindow.h"
#include "ui_mainwindow.h"

int node::lastIndex=0;
node* node::inputScene=NULL;
node* node::outputScene=NULL;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i=0;i<destruc.size();i++) delete destruc[i];
}

void MainWindow::on_pushButton_clicked()
{
    node*n=new node(scene,destruc,"node"+QString::number(node::lastIndex++));
    scene->addItem(n);
}
