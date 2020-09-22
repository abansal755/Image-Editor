#include "mainwindow.h"
#include "ui_mainwindow.h"

int node::lastIndex=0;
int readNode::lastIndex=0;
node* node::inputScene=NULL;
node* node::outputScene=NULL;
QString PropertiesWindow::fileExtensions="Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)";

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
    canv=new canvas(scene,&destruc);
    scene->setSceneRect(canv->boundingRect());
    scene->addItem(canv);
    ui->graphicsView->setScene(scene);

    viewNode=new viewerNode(scene,destruc);
    scene->addItem(viewNode);
    viewNode->setPos(-100,-37.5);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i=0;i<destruc.size();i++) delete destruc[i];
    delete canv;
}
