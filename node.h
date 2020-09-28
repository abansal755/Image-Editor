#pragma once
#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QPainter>
#include<QDebug>
#include<QMenu>
#include<QGraphicsSceneContextMenuEvent>
#include<QPushButton>
#include<QFileDialog>
#include<QMessageBox>
#include<QTextEdit>
#include"sliderint.h"
#include"sliderfloat.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpacerItem>
#include<QLabel>
#include<math.h>
#include<QCheckBox>
#include<QButtonGroup>
#include<QRadioButton>
#include<QImageReader>
#include<unordered_map>
#include<QLineEdit>
#include<QGraphicsPathItem>
#include<QSpinBox>
#include"graphicsview.h"
#include<QTimer>

//x - font size, y - input, z- output
#define PAINT_NODE(x,y,z) QRectF rect=boundingRect();QPainterPath path;path.addRoundedRect(rect,15,15);QLinearGradient grad(0,0,0,height);grad.setColorAt(0,QColor(184,15,10));grad.setColorAt(0.5,QColor(225,36,0));grad.setColorAt(1,QColor(184,15,10));painter->fillPath(path,grad);QPen pen;pen.setWidth(2);painter->setPen(pen);painter->drawPath(path);QFont font;font.setPixelSize(x);painter->setFont(font);if(!pressed) pen.setColor(Qt::black);else pen.setColor(QColor(246,228,134));painter->setPen(pen);painter->drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,name);font.setPixelSize(10);painter->setFont(font);if(y) painter->drawText(rect,Qt::AlignHCenter|Qt::AlignTop,"input");if(z) painter->drawText(rect,Qt::AlignHCenter|Qt::AlignBottom,"output");
#define BEZIER_CURVE(path,p1,p2,d) path.moveTo(p1);path.cubicTo(p1.x(),p1.y()+d,p2.x(),p2.y()-d,p2.x(),p2.y());

using namespace std;

class PropertiesWindow:public QWidget{
protected:
    static QString fileExtensions;
    QString fileName;
public:
    PropertiesWindow(QString title,QWidget*parent=NULL):QWidget(parent){
        setWindowTitle(title);
    }
    QString getFileName(){
        return fileName;
    }
};

class ReadNodePropertiesWindow:public PropertiesWindow{
private:
    Q_OBJECT
    QHBoxLayout*hBoxLayout1;
    QHBoxLayout*hBoxLayout2;
    QHBoxLayout*hBoxLayout3;
    QHBoxLayout*hBoxLayout4;
    QHBoxLayout*hBoxLayout5;
    QVBoxLayout*vBoxLayout1;
    QPushButton*pushButton1;
    QTextEdit*textEdit1;
    QTextEdit*textEdit2;
    QTextEdit*textEdit3;
    QTextEdit*textEdit4;
    QTextEdit*textEdit5;
    QLabel*label1;
    QLabel*label2;
    QLabel*label3;
    QLabel*label4;
public:
    ReadNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        hBoxLayout1=new QHBoxLayout;
        hBoxLayout2=new QHBoxLayout;
        hBoxLayout3=new QHBoxLayout;
        hBoxLayout4=new QHBoxLayout;
        hBoxLayout5=new QHBoxLayout;
        vBoxLayout1=new QVBoxLayout;
        textEdit1=new QTextEdit;
        textEdit2=new QTextEdit;
        textEdit3=new QTextEdit;
        textEdit4=new QTextEdit;
        textEdit5=new QTextEdit;
        pushButton1=new QPushButton("Read Image File");
        label1=new QLabel("File Size(in kB):");
        label2=new QLabel("File Extension:");
        label3=new QLabel("Width(in px):");
        label4=new QLabel("Height(in px):");

        textEdit1->setFixedHeight(23);
        textEdit1->setDisabled(true);
        textEdit2->setFixedHeight(23);
        textEdit2->setDisabled(true);
        textEdit3->setFixedHeight(23);
        textEdit3->setDisabled(true);
        textEdit4->setFixedHeight(23);
        textEdit4->setDisabled(true);
        textEdit5->setFixedHeight(23);
        textEdit5->setDisabled(true);

        hBoxLayout1->addWidget(textEdit1);
        hBoxLayout1->addWidget(pushButton1);

        hBoxLayout2->addWidget(label1);
        hBoxLayout2->addWidget(textEdit2);

        hBoxLayout3->addWidget(label2);
        hBoxLayout3->addWidget(textEdit3);

        hBoxLayout4->addWidget(label3);
        hBoxLayout4->addWidget(textEdit4);

        hBoxLayout5->addWidget(label4);
        hBoxLayout5->addWidget(textEdit5);

        vBoxLayout1->addLayout(hBoxLayout1);
        vBoxLayout1->addLayout(hBoxLayout3);
        vBoxLayout1->addLayout(hBoxLayout2);
        vBoxLayout1->addLayout(hBoxLayout4);
        vBoxLayout1->addLayout(hBoxLayout5);
        vBoxLayout1->insertStretch(5);
        setLayout(vBoxLayout1);

        connect(pushButton1,SIGNAL(clicked()),this,SLOT(pushButton1Clicked()));
    }
private slots:
    void pushButton1Clicked(){
        fileName=QFileDialog::getOpenFileName(this,"Read Image File","",fileExtensions);
        QFileInfo info(fileName);
        QImageReader info2(fileName);
        if(!info2.canRead()){
            QMessageBox::critical(this,"Error","Invalid File");
            fileName="";
            textEdit1->setText("");
            textEdit2->setText("");
            textEdit3->setText("");
            textEdit4->setText("");
            textEdit5->setText("");
        }else{
            textEdit1->setText(fileName);
            textEdit2->setText(QString::number((float)info.size()/1000));
            textEdit3->setText(info.suffix());
            QSize size=info2.size();
            textEdit4->setText(QString::number(size.width()));
            textEdit5->setText(QString::number(size.height()));
        }
    }
};

class ViewerNodePropertiesWindow:public PropertiesWindow{
private:
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1;
    QLabel*label1;
public:
    QPushButton*pushButton1;
    QCheckBox*checkBox1;
    QSpinBox*spinBox1;
    QGraphicsScene*scene;
    ImageGraphicsView*imageGraphicsView;
    ViewerNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        vBoxLayout1=new QVBoxLayout;
        hBoxLayout1=new QHBoxLayout;
        imageGraphicsView=new ImageGraphicsView;
        pushButton1=new QPushButton("Refresh");
        checkBox1=new QCheckBox("Automatic refresh");
        label1=new QLabel("Time Interval(in ms):");
        spinBox1=new QSpinBox;
        spinBox1->setRange(500,10000);
        spinBox1->setValue(1000);
        spinBox1->setMaximumWidth(60);
        spinBox1->setDisabled(true);

        hBoxLayout1->addWidget(checkBox1);
        hBoxLayout1->addWidget(label1);
        hBoxLayout1->addWidget(spinBox1);
        hBoxLayout1->addStretch();
        hBoxLayout1->addWidget(pushButton1);
        vBoxLayout1->addWidget(imageGraphicsView);
        vBoxLayout1->addLayout(hBoxLayout1);
        setLayout(vBoxLayout1);

        scene=new QGraphicsScene(this);
        imageGraphicsView->setScene(scene);
        resize(800,600);
    }
};

class BlurNodePropertiesWindow:public PropertiesWindow{
private:
    sliderInt*slider;
    QVBoxLayout*vBoxLayout1;
public:
    BlurNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        slider=new sliderInt;
        slider->setText("Blur Radius:");
        slider->setRange(0,5);

        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addWidget(slider);
        vBoxLayout1->insertStretch(1);
        setLayout(vBoxLayout1);
    }
    int getValue(){
        return slider->getValue();
    }
};

class SaturateNodePropertiesWindow:public PropertiesWindow{
private:
    sliderFloat*slider;
    QVBoxLayout*vBoxLayout1;
public:
    SaturateNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        slider=new sliderFloat;
        slider->setText("Saturate:");
        slider->setRange(-100,100,100);

        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addWidget(slider);
        vBoxLayout1->insertStretch(1);
        setLayout(vBoxLayout1);
    }
    float getValue(){
        return slider->getValue();
    }
};

class ContrastNodePropertiesWindow:public PropertiesWindow{
private:
    sliderInt*slider;
    QVBoxLayout*vBoxLayout1;
public:
    ContrastNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        slider=new sliderInt;
        slider->setText("Contrast:");
        slider->setRange(-255,255);

        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addWidget(slider);
        vBoxLayout1->insertStretch(1);
        setLayout(vBoxLayout1);
    }
    float getValue(){
        return slider->getValue();
    }
};

class GradeNodePropertiesWindow:public PropertiesWindow{
private:
    sliderFloat*sliderLift;
    sliderFloat*sliderGain;
    sliderFloat*sliderOffset;
    QVBoxLayout*vBoxLayout1;
public:
    GradeNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        sliderLift=new sliderFloat;
        sliderGain=new sliderFloat;
        sliderOffset=new sliderFloat;

        sliderLift->setText("Lift:");
        sliderLift->setRange(-500,500,100);
        sliderGain->setText("Gain:");
        sliderGain->setRange(-500,500,100);
        sliderGain->setDefaultValue(1);
        sliderOffset->setText("Offset:");
        sliderOffset->setRange(-500,500,100);

        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addWidget(sliderLift);
        vBoxLayout1->addWidget(sliderGain);
        vBoxLayout1->addWidget(sliderOffset);
        vBoxLayout1->insertStretch(3);
        setLayout(vBoxLayout1);
    }
    float getLift(){
        return sliderLift->getValue();
    }
    float getGain(){
        return sliderGain->getValue();
    }
    float getOffset(){
        return sliderOffset->getValue();
    }
};

class GammaNodePropertiesWindow:public PropertiesWindow{
private:
    sliderFloat*slider1;
    QVBoxLayout*vBoxLayout1;
public:
    GammaNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        slider1=new sliderFloat;
        slider1->setText("Gamma:");
        slider1->setRange(0,500,100);
        slider1->setDefaultValue(1);
        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addWidget(slider1);
        vBoxLayout1->insertStretch(1);
        setLayout(vBoxLayout1);
    }
    float getValue(){
        return slider1->getValue();
    }
};

class MirrorNodePropertiesWindow:public PropertiesWindow{
private:
    QHBoxLayout*hBoxLayout1;
    QVBoxLayout*vBoxLayout1;
    QCheckBox*checkBox1;
    QCheckBox*checkBox2;
public:
    MirrorNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        checkBox1=new QCheckBox("Mirror Horizontally");
        checkBox2=new QCheckBox("Mirror Vertically");
        hBoxLayout1=new QHBoxLayout;
        vBoxLayout1=new QVBoxLayout;
        hBoxLayout1->addWidget(checkBox1);
        hBoxLayout1->addWidget(checkBox2);
        hBoxLayout1->insertStretch(2);
        vBoxLayout1->insertLayout(0,hBoxLayout1);
        vBoxLayout1->insertStretch(1);
        setLayout(vBoxLayout1);
    }
    bool mirrorH(){
        return checkBox1->isChecked();
    }
    bool mirrorV(){
        return checkBox2->isChecked();
    }
};

class RotateNodePropertiesWindow:public PropertiesWindow{
private:
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1;
    QHBoxLayout*hBoxLayout2;
    QSpinBox*spinBox1;
    QLabel*label1;
    QButtonGroup*buttonGroup1;
    QRadioButton*radioButton1;
    QRadioButton*radioButton2;
public:
    RotateNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        label1=new QLabel("Clockwise angle(in deg):");
        spinBox1=new QSpinBox;
        spinBox1->setMinimum(-360);
        spinBox1->setMaximum(360);
        spinBox1->setValue(0);
        spinBox1->setFixedWidth(50);
        hBoxLayout1=new QHBoxLayout;
        hBoxLayout1->addWidget(label1);
        hBoxLayout1->addWidget(spinBox1);
        hBoxLayout1->addStretch();

        hBoxLayout2=new QHBoxLayout;
        buttonGroup1=new QButtonGroup(this);
        radioButton1=new QRadioButton("No Filtering");
        radioButton2=new QRadioButton("Bilinear Filtering");
        radioButton2->setChecked(true);
        buttonGroup1->addButton(radioButton1,0);
        buttonGroup1->addButton(radioButton2,1);
        hBoxLayout2->addWidget(radioButton1);
        hBoxLayout2->addWidget(radioButton2);
        hBoxLayout2->addStretch();



        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addLayout(hBoxLayout1);
        vBoxLayout1->addLayout(hBoxLayout2);
        vBoxLayout1->addStretch();
        setLayout(vBoxLayout1);
    }
    int getAngle(){
        return spinBox1->value();
    }
    Qt::TransformationMode filtering(){
        int id=buttonGroup1->checkedId();
        if(id==0) return Qt::FastTransformation;
        return Qt::SmoothTransformation;
    }
};

class ScaleNodePropertiesWindow:public PropertiesWindow{
private:
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1;
    QHBoxLayout*hBoxLayout2;
    QHBoxLayout*hBoxLayout3;
    QLabel*label1,*label2;
    QLineEdit*lineEdit1,*lineEdit2;
    QButtonGroup*buttonGroup1;
    QButtonGroup*buttonGroup2;
    QRadioButton*radioButton1,*radioButton2,*radioButton3;
    QRadioButton*radioButton4,*radioButton5;
public:
    ScaleNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        label1=new QLabel("Width(in px):");
        label2=new QLabel("Height(in px):");
        lineEdit1=new QLineEdit;
        lineEdit2=new QLineEdit;
        hBoxLayout1=new QHBoxLayout;
        hBoxLayout1->addWidget(label1);
        hBoxLayout1->addWidget(lineEdit1);
        hBoxLayout1->addWidget(label2);
        hBoxLayout1->addWidget(lineEdit2);
        hBoxLayout1->insertStretch(4);

        hBoxLayout2=new QHBoxLayout;
        buttonGroup1=new QButtonGroup(this);
        radioButton1=new QRadioButton("Ignore Aspect Ratio");
        radioButton1->setChecked(true);
        radioButton2=new QRadioButton("Keep Aspect Ratio");
        radioButton3=new QRadioButton("Keep Aspect Ratio By Expanding");
        buttonGroup1->addButton(radioButton1,0);
        buttonGroup1->addButton(radioButton2,1);
        buttonGroup1->addButton(radioButton3,2);
        hBoxLayout2->addWidget(radioButton1);
        hBoxLayout2->addWidget(radioButton2);
        hBoxLayout2->addWidget(radioButton3);
        hBoxLayout2->insertStretch(3);

        hBoxLayout3=new QHBoxLayout;
        buttonGroup2=new QButtonGroup(this);
        radioButton4=new QRadioButton("No Filtering");
        radioButton5=new QRadioButton("Bilinear Filtering");
        radioButton5->setChecked(true);
        buttonGroup2->addButton(radioButton4,0);
        buttonGroup2->addButton(radioButton5,1);
        hBoxLayout3->addWidget(radioButton4);
        hBoxLayout3->addWidget(radioButton5);
        hBoxLayout3->insertStretch(2);

        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addLayout(hBoxLayout1);
        vBoxLayout1->addLayout(hBoxLayout2);
        vBoxLayout1->addLayout(hBoxLayout3);
        vBoxLayout1->insertStretch(3);
        setLayout(vBoxLayout1);
    }
    Qt::AspectRatioMode aspectRatioMode(){
        if(buttonGroup1->checkedId()==0) return Qt::IgnoreAspectRatio;
        else if(buttonGroup1->checkedId()==1) return Qt::KeepAspectRatio;
        return Qt::KeepAspectRatioByExpanding;
    }
    Qt::TransformationMode filteringMode(){
        if(buttonGroup2->checkedId()==0) return Qt::FastTransformation;
        return Qt::SmoothTransformation;
    }
    QString width(){
        return lineEdit1->text();
    }
    QString height(){
        return lineEdit2->text();
    }
};

class WriteNodePropertiesWindow:public PropertiesWindow{
private:
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1;
    sliderInt*slider1;
public:
    QPushButton*pushButton1;
    WriteNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        pushButton1=new QPushButton("Save Image File",this);
        vBoxLayout1=new QVBoxLayout;
        slider1=new sliderInt;
        slider1->setText("Quality Factor:");
        slider1->setRange(0,100);
        slider1->setDefaultValue(100);
        hBoxLayout1=new QHBoxLayout;
        hBoxLayout1->addStretch();
        hBoxLayout1->addWidget(pushButton1);
        vBoxLayout1->addWidget(slider1);
        vBoxLayout1->addLayout(hBoxLayout1);
        vBoxLayout1->addStretch();
        setLayout(vBoxLayout1);
    }
    int getFactor(){
        return slider1->getValue();
    }
};

class CropNodePropertiesWindow:public PropertiesWindow{
private:
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1,*hBoxLayout2;
    QLabel*label1,*label2,*label3,*label4;
    QLineEdit*lineEdit1,*lineEdit2,*lineEdit3,*lineEdit4;
public:
    CropNodePropertiesWindow(QString title,QWidget*parent=NULL):PropertiesWindow(title,parent){
        vBoxLayout1=new QVBoxLayout;
        hBoxLayout1=new QHBoxLayout;
        hBoxLayout2=new QHBoxLayout;
        label1=new QLabel("x(in px):");
        label2=new QLabel("y(in px):");
        label3=new QLabel("Width(in px):");
        label4=new QLabel("Height(in px):");
        lineEdit1=new QLineEdit;
        lineEdit2=new QLineEdit;
        lineEdit3=new QLineEdit;
        lineEdit4=new QLineEdit;

        hBoxLayout1->addWidget(label1);
        hBoxLayout1->addWidget(lineEdit1);
        hBoxLayout1->addWidget(label2);
        hBoxLayout1->addWidget(lineEdit2);
        hBoxLayout1->addStretch();

        hBoxLayout2->addWidget(label3);
        hBoxLayout2->addWidget(lineEdit3);
        hBoxLayout2->addWidget(label4);
        hBoxLayout2->addWidget(lineEdit4);
        hBoxLayout2->addStretch();

        vBoxLayout1->addLayout(hBoxLayout1);
        vBoxLayout1->addLayout(hBoxLayout2);
        vBoxLayout1->addStretch();
        setLayout(vBoxLayout1);
    }
    int getX(){
        return lineEdit1->text().toInt();
    }
    int getY(){
        return lineEdit2->text().toInt();
    }
    int getWidth(){
        return lineEdit3->text().toInt();
    }
    int getHeight(){
        return lineEdit4->text().toInt();
    }
};

class node:public QGraphicsItem{
protected:
    QString name;
    int width,height;
    node*input;
    unordered_map<node*,QGraphicsPathItem*>output;
    QGraphicsPathItem*inputLine;
    QGraphicsScene*scene;
    bool pressed;
    PropertiesWindow*propW;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        PAINT_NODE(30,true,true)
    };
    QVariant itemChange(GraphicsItemChange change, const QVariant &value){
        if(change==ItemPositionChange){
            //p1 is output point
            //p2 is input point
            if(input!=NULL){
                QPointF p1(input->x()+input->width/2,input->y()+input->height);
                QPointF p2=value.toPointF();
                p2.setX(p2.x()+width/2);
                QPainterPath path;
                BEZIER_CURVE(path,p1,p2,25)
                inputLine->setPath(path);
            }
            for(auto it=output.begin();it!=output.end();it++){
                QPointF p1=value.toPointF();
                p1.setX(p1.x()+width/2);
                p1.setY(p1.y()+height);
                QPointF p2(it->first->x()+it->first->width/2,it->first->y());
                QPainterPath path;
                BEZIER_CURVE(path,p1,p2,25)
                it->second->setPath(path);
            }
        }
        return QGraphicsItem::itemChange(change,value);
    };
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        pressed=true;
        setZValue(1);
        update();
        return QGraphicsItem::mousePressEvent(event);
    };
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        pressed=false;
        setZValue(0);
        update();
        return QGraphicsItem::mouseReleaseEvent(event);
    };
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*connectOutput=menu.addAction("Connect Output");
        QAction*connectInput=menu.addAction("Connect Input");
        QAction*disconnectInput=menu.addAction("Disconnect Input");
        QAction*disconnectAllOutputs=menu.addAction("Disconnect All Outputs");
        QAction*deleteNode=menu.addAction("Delete Node");
        QAction*properties=menu.addAction("Properties");
        QAction*current=menu.exec(event->screenPos());
        if(current==connectOutput){
            if(inputScene!=NULL && inputScene!=this){
                if(inputScene->input!=NULL) inputScene->removeInput();
                setOutput(inputScene);
                inputScene=NULL;
            }
            else outputScene=this;
        }
        if(current==connectInput){
            if(outputScene!=NULL && outputScene!=this){
                if(input!=NULL) removeInput();
                setInput(outputScene);
                outputScene=NULL;
            }
            else inputScene=this;
        }
        if(current==disconnectInput){
            if(input!=NULL) removeInput();
            inputScene=NULL;
            outputScene=NULL;
        }
        if(current==disconnectAllOutputs){
            for(auto it=output.begin();it!=output.end();){
                (it++)->first->removeInput();
                //increment in for loop not occuring in linux bug
            }
        }
        if(current==deleteNode){
            if(input!=NULL) removeInput();
            for(auto it=output.begin();it!=output.end();){
                (it++)->first->removeInput();
                //increment in for loop not occuring in linux bug
            }
            scene->removeItem(this);
            inputScene=NULL;
            outputScene=NULL;
            if(propW!=NULL) propW->hide();
        }
        if(current==properties){
            if(!(propW->isVisible())) propW->show();
            else propW->activateWindow();
        }
    };
public:
    node(QGraphicsScene*scene,vector<node*>&destruc,QString name="node"+QString::number(lastIndex++),int width=200,int height=75)
        :scene(scene),name(name),width(width),height(height),
        input(NULL),output(NULL),inputLine(NULL)
    {
        destruc.push_back(this);
        setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemSendsScenePositionChanges);
        pressed=false;
    }
    ~node(){
        if(inputLine!=NULL){
            delete inputLine;
            input->output.erase(this);
        }
        for(auto it=output.begin();it!=output.end();it++){
            delete it->second;
            it->first->input=NULL;
            it->first->inputLine=NULL;
            output.erase(it);
        }
        if(propW!=NULL) delete propW;
    }
    QRectF boundingRect() const{
        return QRectF(0,0,width,height);
    };
    void setInput(node*input){
        this->input=input;
        QPen pen;
        pen.setWidth(2);
        pen.setColor(Qt::white);
        QPainterPath path;
        QPointF p1(input->x()+input->width/2,input->y()+input->height);
        QPointF p2(x()+width/2,y());
        BEZIER_CURVE(path,p1,p2,25)
        inputLine=new QGraphicsPathItem;
        inputLine->setPath(path);
        inputLine->setPen(pen);
        inputLine->setZValue(-1);
        scene->addItem(inputLine);
        input->output.insert({this,inputLine});
    }
    void removeInput(){
        scene->removeItem(inputLine);
        delete inputLine;
        inputLine=NULL;
        input->output.erase(this);
        input=NULL;
    }
    void setOutput(node*output){
        output->input=this;
        QPen pen;
        pen.setWidth(2);
        pen.setColor(Qt::white);
        QPointF p1(x()+width/2,+y()+height);
        QPointF p2(output->x()+output->width/2,output->y());
        QGraphicsPathItem*outputLine=new QGraphicsPathItem;
        QPainterPath path;
        BEZIER_CURVE(path,p1,p2,25)
        outputLine->setPath(path);
        outputLine->setPen(pen);
        outputLine->setZValue(-1);
        output->inputLine=outputLine;
        scene->addItem(outputLine);
        this->output.insert({output,outputLine});
    }
    void setName(QString name){
        this->name=name;
    }
    QString getName(){
        return name;
    }
    node*getInput(){
        return input;
    }
    int getWidth(){
        return width;
    }
    int getHeight(){
        return height;
    }
    virtual bool imageCalculate(QImage&image){
        return false;
    }
    static float clampF(float i){
        if(i>1) return 1;
        else if(i<0) return 0;
        else return i;
    }
    static int clamp(int i){
        if(i>255) return 255;
        else if(i<0) return 0;
        else return i;
    }
    static int lastIndex;
    static node*inputScene,*outputScene;
};

class readNode:public node{
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(220,220,220));
        grad.setColorAt(0.5,QColor(170,170,170));
        grad.setColorAt(1,QColor(220,220,220));
        painter->fillPath(path,grad);
        QPen pen;pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;font.setPixelSize(30);
        painter->setFont(font);
        if(!pressed) pen.setColor(Qt::black);
        else pen.setColor(QColor(140,140,140));
        painter->setPen(pen);
        painter->drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignHCenter|Qt::AlignBottom,"output");
    };
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*connectOutput=menu.addAction("Connect Output");
        QAction*disconnectAllOutputs=menu.addAction("Disconnect All Outputs");
        QAction*deleteNode=menu.addAction("Delete Node");
        QAction*properties=menu.addAction("Properties");
        QAction*current=menu.exec(event->screenPos());
        if(current==connectOutput){
            if(inputScene!=NULL && inputScene!=this){
                if(inputScene->getInput()!=NULL) inputScene->removeInput();
                setOutput(inputScene);
                inputScene=NULL;
            }
            else outputScene=this;
        }
        if(current==disconnectAllOutputs){
            for(auto it=output.begin();it!=output.end();){
                (it++)->first->removeInput();
                //increment in for loop not occuring in linux bug
            }
        }
        if(current==deleteNode){
            if(input!=NULL) removeInput();
            for(auto it=output.begin();it!=output.end();){
                (it++)->first->removeInput();
                //increment in for loop not occuring in linux bug
            }
            scene->removeItem(this);
            inputScene=NULL;
            outputScene=NULL;
            if(propW!=NULL) propW->hide();
        }
        if(current==properties){
            if(!(propW->isVisible())) propW->show();
            else propW->activateWindow();
        }
    };
public:
    readNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="readNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        propW=new ReadNodePropertiesWindow(name);
    }
    bool imageCalculate(QImage&image){
        return image.load(propW->getFileName());
    }
    static int lastIndex;
};

class viewerNode:public QObject,public node{
private:
    Q_OBJECT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(220,220,220));
        grad.setColorAt(0.5,QColor(170,170,170));
        grad.setColorAt(1,QColor(220,220,220));
        painter->fillPath(path,grad);
        QPen pen;pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;font.setPixelSize(30);
        painter->setFont(font);
        if(!pressed) pen.setColor(Qt::black);
        else pen.setColor(QColor(140,140,140));
        painter->setPen(pen);
        painter->drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignHCenter|Qt::AlignTop,"input");
    };
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*connectInput=menu.addAction("Connect Input");
        QAction*disconnectInput=menu.addAction("Disconnect Input");
        QAction*viewer=menu.addAction("Viewer");
        QAction*deleteNode=menu.addAction("Delete Node");
        QAction*current=menu.exec(event->screenPos());
        if(current==connectInput){
            if(outputScene!=NULL && outputScene!=this){
                if(input!=NULL) removeInput();
                setInput(outputScene);
                outputScene=NULL;
            }
            else inputScene=this;
        }
        if(current==disconnectInput){
            if(input!=NULL) removeInput();
            inputScene=NULL;
            outputScene=NULL;
        }
        if(current==viewer){
            if(!(propW->isVisible())) propW->show();
            else propW->activateWindow();
        }
        if(current==deleteNode){
            if(input!=NULL) removeInput();
            scene->removeItem(this);
            inputScene=NULL;
            outputScene=NULL;
            if(propW!=NULL) propW->hide();
        }
    };
    ViewerNodePropertiesWindow*win;
    QImage image;
    QTimer*timer;
private slots:
    void pushButton1Clicked(){
        win->scene->clear();
        if(getInput()==NULL) return;
        if(!(getInput()->imageCalculate(image))) return;
        QGraphicsPixmapItem*pix=win->scene->addPixmap(QPixmap::fromImage(image));
        pix->setPos(-(float)image.width()/2,-(float)image.height()/2);
        if(win->imageGraphicsView->overlay){
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
        }
        win->imageGraphicsView->centerOn(0,0);
    }
    void checkBox1Clicked(int state){
        if(state==Qt::Checked){
            win->pushButton1->setDisabled(true);
            win->spinBox1->setEnabled(true);
            timer->start(1000);
        }
        else{
            win->pushButton1->setEnabled(true);
            win->spinBox1->setDisabled(true);
            timer->stop();
        }
    }
public:
    viewerNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="viewerNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new ViewerNodePropertiesWindow(getName());
        propW=win;
        timer=new QTimer(this);
        connect(win->pushButton1,SIGNAL(clicked()),this,SLOT(pushButton1Clicked()));
        connect(win->checkBox1,SIGNAL(stateChanged(int)),this,SLOT(checkBox1Clicked(int)));
        connect(timer,SIGNAL(timeout()),this,SLOT(pushButton1Clicked()));
    }
    static int lastIndex;
};

class blurNode:public node{
private:
    void blur(QImage&img,int radius=1){
        int width = img.width(), height = img.height();
        QImage img1(width,height,img.format());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int r = 0, g = 0, b = 0;
                int num = 0;
                for (int di = -radius; di <= radius; di++) {
                    for (int dj = -radius; dj <= radius; dj++) {
                        if (i + di < 0 || i + di >= height) continue;
                        if (j + dj < 0 || j + dj >= width) continue;
                        QColor c=img.pixelColor(j+dj,i+di);
                        r += c.red();
                        g += c.green();
                        b += c.blue();
                        num++;
                    }
                }
                r /= num;
                g /= num;
                b /= num;
                img1.setPixelColor(j,i,QColor(r,g,b,img.pixelColor(j,i).alpha()));
            }
        }
        img=img1;
    }
    BlurNodePropertiesWindow*win;
public:
    blurNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="blurNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new BlurNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage&image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        blur(image,win->getValue());
        return true;
    }
    static int lastIndex;
};

class saturateNode:public node{
private:
    void saturate(QImage&img,float x){
        for(int i=0;i<img.height();i++){
            for(int j=0;j<img.width();j++){
                 QColor c=img.pixelColor(j,i);
                 float h=c.hueF();
                 float s=c.saturationF();
                 float v=c.valueF();
                 if(x>=0) s+=(1-s)*x;
                 else s*=1-abs(x);
                 c.setHsvF(h,s,v,c.alphaF());
                 img.setPixelColor(j,i,c);
            }
        }
    }
    SaturateNodePropertiesWindow*win;
public:
    saturateNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="saturateNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new SaturateNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage&image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        saturate(image,win->getValue());
        return true;
    }
    static int lastIndex;
};

class contrastNode:public node{
private:
    void contrast(QImage&img,int c){
        float f=(259*((float)c+255))/(255*(259-(float)c));
        for(int y=0;y<img.height();y++){
            for(int x=0;x<img.width();x++){
                QColor c=img.pixelColor(x,y);
                int r=c.red();
                int g=c.green();
                int b=c.blue();
                r=clamp(f*(r-128)+128);
                g=clamp(f*(g-128)+128);
                b=clamp(f*(b-128)+128);
                c.setRed(r);
                c.setGreen(g);
                c.setBlue(b);
                img.setPixelColor(x,y,c);
            }
        }
    }
    ContrastNodePropertiesWindow*win;
public:
    contrastNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="contrastNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new ContrastNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage&image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        contrast(image,win->getValue());
        return true;
    }
    static int lastIndex;
};

class gradeNode:public node{
private:
    void grade(QImage&image, float lift,float gain,float offset){
        for(int y=0;y<image.height();y++){
            for(int x=0;x<image.width();x++){
                QColor c=image.pixelColor(x,y);
                float r=c.redF();
                float g=c.greenF();
                float b=c.blueF();
                r=clampF((r*(gain-lift))+lift+offset);
                g=clampF((g*(gain-lift))+lift+offset);
                b=clampF((b*(gain-lift))+lift+offset);
                c.setRedF(r);
                c.setGreenF(g);
                c.setBlueF(b);
                image.setPixelColor(x,y,c);
            }
        }
    }
    GradeNodePropertiesWindow*win;
public:
    gradeNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="gradeNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new GradeNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage&image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        grade(image,win->getLift(),win->getGain(),win->getOffset());
        return true;
    }
    static int lastIndex;
};

class gammaNode:public node{
private:
    void gammaCorrect(QImage&image,float gamma){
        for(int y=0;y<image.height();y++){
            for(int x=0;x<image.width();x++){
                QColor c=image.pixelColor(x,y);
                c.setRed(255*pow((float)c.red()/255,1/gamma));
                c.setGreen(255*pow((float)c.green()/255,1/gamma));
                c.setBlue(255*pow((float)c.blue()/255,1/gamma));
                image.setPixelColor(x,y,c);
            }
        }
    }
    GammaNodePropertiesWindow*win;
public:
    gammaNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="gammaNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new GammaNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage &image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        gammaCorrect(image,win->getValue());
        return true;
    };
    static int lastIndex;
};

class mirrorNode:public node{
private:
    void mirror_imgH(QImage&img) {
        QImage temp(img.width(),img.height(),img.format());
        for(int y = 0; y < img.height(); y++) {
            for(int x = 0; x < img.width(); x++) {
                QColor c = img.pixelColor(x,y);
                temp.setPixelColor(img.width() - 1 - x, y, c);
            }
        }
        img = temp;
    }
    void mirror_imgV(QImage&img) {
        QImage temp(img.width(),img.height(),img.format());
        for(int y = 0; y < img.height(); y++) {
            for(int x = 0; x < img.width(); x++) {
                QColor c = img.pixelColor(x,y);
                temp.setPixelColor(x, img.height()- 1 - y, c);
            }
        }
        img = temp;
    }
    MirrorNodePropertiesWindow*win;
public:
    mirrorNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="mirrorNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new MirrorNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage &image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        if(win->mirrorH()) mirror_imgH(image);
        if(win->mirrorV()) mirror_imgV(image);
        return true;
    };
    static int lastIndex;
};

class rotateNode:public node{
private:
    RotateNodePropertiesWindow*win;
public:
    rotateNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="rotateNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new RotateNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage &image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        QTransform tr;
        tr.rotate(win->getAngle());
        image=image.transformed(tr,win->filtering());
        return true;
    };
    static int lastIndex;
};

class scaleNode:public node{
private:
    ScaleNodePropertiesWindow*win;
public:
    scaleNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="scaleNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new ScaleNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage &image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        int width=win->width().toInt();
        int height=win->height().toInt();
        if(width==0 || height==0) return true;
        image=image.scaled(width,height,win->aspectRatioMode(),win->filteringMode());
        return true;
    };
    static int lastIndex;
};

class writeNode:public QObject,public node{
private:
    Q_OBJECT
    WriteNodePropertiesWindow*win;
    QImage image;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(220,220,220));
        grad.setColorAt(0.5,QColor(170,170,170));
        grad.setColorAt(1,QColor(220,220,220));
        painter->fillPath(path,grad);
        QPen pen;pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;font.setPixelSize(30);
        painter->setFont(font);
        if(!pressed) pen.setColor(Qt::black);
        else pen.setColor(QColor(140,140,140));
        painter->setPen(pen);
        painter->drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignHCenter|Qt::AlignTop,"input");
    };
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        QMenu menu;
        QAction*connectInput=menu.addAction("Connect Input");
        QAction*disconnectInput=menu.addAction("Disconnect Input");
        QAction*deleteNode=menu.addAction("Delete Node");
        QAction*properties=menu.addAction("Properties");
        QAction*current=menu.exec(event->screenPos());
        if(current==connectInput){
            if(outputScene!=NULL && outputScene!=this){
                if(input!=NULL) removeInput();
                setInput(outputScene);
                outputScene=NULL;
            }
            else inputScene=this;
        }
        if(current==disconnectInput){
            if(input!=NULL) removeInput();
            inputScene=NULL;
            outputScene=NULL;
        }
        if(current==properties){
            if(!(propW->isVisible())) propW->show();
            else propW->activateWindow();
        }
        if(current==deleteNode){
            if(input!=NULL) removeInput();
            scene->removeItem(this);
            inputScene=NULL;
            outputScene=NULL;
            if(propW!=NULL) propW->hide();
        }
    };
private slots:
    void pushButton1Clicked(){
        if(getInput()==NULL) return;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return;
        QString fileName=QFileDialog::getSaveFileName(win,"Save Image File","","Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");
        QFileInfo info(fileName);
        if(info.suffix()!="") image.save(fileName,NULL,win->getFactor());
    }
public:
    writeNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="writeNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new WriteNodePropertiesWindow(getName());
        propW=win;
        connect(win->pushButton1,SIGNAL(clicked()),this,SLOT(pushButton1Clicked()));
    }
    static int lastIndex;
};

class cropNode:public node{
private:
    CropNodePropertiesWindow*win;
public:
    cropNode(QGraphicsScene*scene,vector<node*>&destruc,QString name="cropNode"+QString::number(lastIndex++)):node(scene,destruc,name){
        win=new CropNodePropertiesWindow(getName());
        propW=win;
    }
    bool imageCalculate(QImage &image){
        if(getInput()==NULL) return false;
        bool ans=getInput()->imageCalculate(image);
        if(!ans) return false;
        if(win->getHeight()<=0 || win->getWidth()<=0) return false;
        image=image.copy(win->getX(),win->getY(),win->getWidth(),win->getHeight());
        return true;
    };
    static int lastIndex;
};
