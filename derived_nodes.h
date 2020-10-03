#pragma once
#include"node.h"

class ReadNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
public:
    QString fileName;
    QPushButton*button1;
    ReadNodePropertiesWindow(QString title):PropertiesWindow(title){
        button1=new QPushButton("Read Image File",this);
    }
};

class ViewerNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
public:
    ImageGraphicsView*gw;
    QGraphicsScene*scene;
    ViewerNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        gw=new ImageGraphicsView;
        v1->addWidget(gw);
        setLayout(v1);
        scene=new QGraphicsScene(this);
        gw->setScene(scene);
        resize(800,600);
    }
};

class readNode:public node{
    Q_OBJECT
    ReadNodePropertiesWindow*win;
private slots:
    void button1Clicked(){
        win->fileName=QFileDialog::getOpenFileName(win,"Read Image File","","Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");
        refresh();
    }
public:
    readNode(QGraphicsScene*scene):node(scene,noInput,oneOutput,"readNode"+QString::number(lastIndex++)){
        win=new ReadNodePropertiesWindow(name);
        propW=win;
        connect(win->button1,SIGNAL(clicked()),this,SLOT(button1Clicked()));
    }
    bool imageCalculate(QImage &image){
        bool ans=image.load(win->fileName);
        return ans;
    };
    static int lastIndex;
};

class viewerNode:public node{
    Q_OBJECT
    ViewerNodePropertiesWindow*win;
    QImage image;
protected slots:
    void refresh(){
        imageCalculate(image);
    }
public:
    viewerNode(QGraphicsScene*scene):node(scene,oneInput,noOutput,"viewerNode"+QString::number(lastIndex++)){
        win=new ViewerNodePropertiesWindow(name);
        propW=win;
    }
    bool imageCalculate(QImage &image){
        win->scene->clear();
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        QGraphicsPixmapItem*pix=win->scene->addPixmap(QPixmap::fromImage(image));
        pix->setPos(-(float)image.width()/2,-(float)image.height()/2);
        QPen pen;
        pen.setColor(Qt::red);
        win->scene->addLine(-(float)image.width()/2,(float)image.height()/2,(float)image.width()/2,(float)image.height()/2,pen);
        win->scene->addLine(-(float)image.width()/2,-(float)image.height()/2,(float)image.width()/2,-(float)image.height()/2,pen);
        win->scene->addLine(-(float)image.width()/2,(float)image.height()/2,-(float)image.width()/2,-(float)image.height()/2,pen);
        win->scene->addLine((float)image.width()/2,(float)image.height()/2,(float)image.width()/2,-(float)image.height()/2,pen);
        QFont font;
        font.setPixelSize(max(image.width(),image.height())/40);
        QGraphicsTextItem*text=win->scene->addText(QString::number(image.width())+"x"+QString::number(image.height()),font);
        text->setDefaultTextColor(Qt::red);
        text->setPos((float)image.width()/2,(float)image.height()/2);
        win->gw->centerOn(0,0);
        return true;
    };
    static int lastIndex;
};
