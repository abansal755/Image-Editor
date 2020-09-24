#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

int node::lastIndex=0;
int readNode::lastIndex=0;
int blurNode::lastIndex=0;
int gradeNode::lastIndex=0;
int saturateNode::lastIndex=0;
int contrastNode::lastIndex=0;
int gammaNode::lastIndex=0;
int mirrorNode::lastIndex=0;
int rotateNode::lastIndex=0;
node* node::inputScene=NULL;
node* node::outputScene=NULL;
QString PropertiesWindow::fileExtensions="Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)";

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
    canv=new canvas(ui->graphicsView,scene,&destruc);
    scene->setSceneRect(canv->boundingRect());
    scene->addItem(canv);
    ui->graphicsView->setScene(scene);

    viewNode=new viewerNode(scene,destruc);
    scene->addItem(viewNode);
    viewNode->setPos(-(float)viewNode->getWidth()/2,-(float)viewNode->getHeight()/2);

    imageViewer=new ImageViewer(viewNode);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i=0;i<destruc.size();i++) delete destruc[i];
    delete canv;
    delete imageViewer;
}

void MainWindow::on_actionViewer_triggered()
{
    //View Menu Button
    if(!imageViewer->isVisible()) imageViewer->show();
    else imageViewer->activateWindow();
}

void MainWindow::on_actionExit_triggered()
{
    //Exit Menu Button
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    //About Menu Button
    AboutDialog d;
    d.exec();
}
