#include"node.h"
#include<QPushButton>
#include<QFileDialog>
#include<QVBoxLayout>
#include<QLineEdit>
#include<QLabel>
#include<QImageReader>
#include"sliderint.h"
#include"sliderfloat.h"
#include<math.h>
#include<QCheckBox>
#include<QButtonGroup>
#include<QSpinBox>
#include<QRadioButton>

class ReadNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*h[5];
    QLabel*label[4];
public:
    QString fileName;
    QPushButton*button1;
    QLineEdit*edit[5];
    ReadNodePropertiesWindow(QString title):PropertiesWindow(title){
        button1=new QPushButton("Read Image File",this);
        v1=new QVBoxLayout;
        for(int i=0;i<5;i++) h[i]=new QHBoxLayout;
        for(int i=0;i<5;i++){
            edit[i]=new QLineEdit;
            edit[i]->setDisabled(true);
        }
        for(int i=0;i<4;i++) label[i]=new QLabel;
        label[0]->setText("File Extension:");
        label[1]->setText("File Size(in kB):");
        label[2]->setText("Image Width(in px):");
        label[3]->setText("Image Height(in px):");

        h[0]->addWidget(edit[0]);
        h[0]->addWidget(button1);
        h[1]->addWidget(label[0]);
        h[1]->addWidget(edit[1]);
        h[2]->addWidget(label[1]);
        h[2]->addWidget(edit[2]);
        h[3]->addWidget(label[2]);
        h[3]->addWidget(edit[3]);
        h[4]->addWidget(label[3]);
        h[4]->addWidget(edit[4]);
        for(int i=0;i<5;i++) v1->addLayout(h[i]);
        v1->addStretch();
        setLayout(v1);
    }
};

class readNode:public node{
    Q_OBJECT
    ReadNodePropertiesWindow*win;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(220,220,220));
        grad.setColorAt(0.5,QColor(170,170,170));
        grad.setColorAt(1,QColor(220,220,220));
        painter->fillPath(path,grad);
        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        if(iType==oneInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/2-ci1->getWidth()/2-5,15);
            painter->drawText(rect1,Qt::AlignRight,"input");
        }else if(iType==twoInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/4-ci1->getWidth()/2-2,15);
            painter->drawText(rect1,Qt::AlignRight,"A");
            rect1.setWidth(3*width/4-ci2->getWidth()/2-2);
            painter->drawText(rect1,Qt::AlignRight,"B");
        }
        if(oType==oneOutput){
            QRectF rect1(0,0,width/2-co->getWidth()/2-5,height-co->getHeight()/2+2);
            painter->drawText(rect1,Qt::AlignRight|Qt::AlignBottom,"output");
        }
    };
private slots:
    void button1Clicked(){
        win->fileName=QFileDialog::getOpenFileName(win,"Read Image File","","Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");
        QFileInfo info(win->fileName);
        QImageReader info2(win->fileName);
        win->edit[0]->setText(win->fileName);
        if(info2.canRead()){
            win->edit[1]->setText(info.suffix());
            win->edit[2]->setText(QString::number((float)info.size()/1000));
            win->edit[3]->setText(QString::number(info2.size().width()));
            win->edit[4]->setText(QString::number(info2.size().height()));
        }else{
            for(int i=1;i<=4;i++) win->edit[i]->setText("");
        }
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
        image = image.convertToFormat(QImage::Format_RGBA8888);
        return ans;
    };
    static int lastIndex;
};

class ViewerNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*h1;
public:
    QRadioButton*b1,*b2,*b3,*b4,*b5;
    ImageGraphicsView*gw;
    QGraphicsScene*scene;
    ViewerNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        gw=new ImageGraphicsView;

        h1=new QHBoxLayout;
        b1=new QRadioButton("RGBA");
        b1->setChecked(true);
        b2=new QRadioButton("R");
        b3=new QRadioButton("G");
        b4=new QRadioButton("B");
        b5=new QRadioButton("A");
        h1->addStretch();
        h1->addWidget(b1);
        h1->addWidget(b2);
        h1->addWidget(b3);
        h1->addWidget(b4);
        h1->addWidget(b5);

        v1->addWidget(gw);
        v1->addLayout(h1);
        setLayout(v1);
        scene=new QGraphicsScene(this);
        gw->setScene(scene);
        resize(800,600);
    }
};

class viewerNode:public node{
    Q_OBJECT
    ViewerNodePropertiesWindow*win;
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
        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        if(iType==oneInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/2-ci1->getWidth()/2-5,15);
            painter->drawText(rect1,Qt::AlignRight,"input");
        }else if(iType==twoInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/4-ci1->getWidth()/2-2,15);
            painter->drawText(rect1,Qt::AlignRight,"A");
            rect1.setWidth(3*width/4-ci2->getWidth()/2-2);
            painter->drawText(rect1,Qt::AlignRight,"B");
        }
        if(oType==oneOutput){
            QRectF rect1(0,0,width/2-co->getWidth()/2-5,height-co->getHeight()/2+2);
            painter->drawText(rect1,Qt::AlignRight|Qt::AlignBottom,"output");
        }
    };
protected slots:
    void refresh(){
        imageCalculate(image);
    }
    void updateViewer(){
        win->scene->clear();
        if(image.isNull()) return;
        QImage imageV=image;
        QGraphicsPixmapItem*pix;
        if(win->b2->isChecked()){
            for(int y=0;y<imageV.height();y++){
                for(int x=0;x<imageV.width();x++){
                    QColor c=imageV.pixelColor(x,y);
                    c.setGreen(c.red());
                    c.setBlue(c.red());
                    c.setAlpha(255);
                    imageV.setPixelColor(x,y,c);
                }
            }
        }else if(win->b3->isChecked()){
            for(int y=0;y<imageV.height();y++){
                for(int x=0;x<imageV.width();x++){
                    QColor c=imageV.pixelColor(x,y);
                    c.setRed(c.green());
                    c.setBlue(c.green());
                    c.setAlpha(255);
                    imageV.setPixelColor(x,y,c);
                }
            }
        }else if(win->b4->isChecked()){
            for(int y=0;y<imageV.height();y++){
                for(int x=0;x<imageV.width();x++){
                    QColor c=imageV.pixelColor(x,y);
                    c.setRed(c.blue());
                    c.setGreen(c.blue());
                    c.setAlpha(255);
                    imageV.setPixelColor(x,y,c);
                }
            }
        }else if(win->b5->isChecked()){
            for(int y=0;y<imageV.height();y++){
                for(int x=0;x<imageV.width();x++){
                    QColor c=imageV.pixelColor(x,y);
                    c.setRed(c.alpha());
                    c.setGreen(c.alpha());
                    c.setBlue(c.alpha());
                    c.setAlpha(255);
                    imageV.setPixelColor(x,y,c);
                }
            }
        }
        pix=win->scene->addPixmap(QPixmap::fromImage(imageV));
        pix->setPos(-(float)image.width()/2,-(float)image.height()/2);
        if(win->gw->overlay){
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
        win->gw->centerOn(0,0);
    }
public:
    viewerNode(QGraphicsScene*scene):node(scene,oneInput,noOutput,"viewerNode"+QString::number(lastIndex++)){
        win=new ViewerNodePropertiesWindow(name);
        propW=win;
        propertiesText="Viewer";
        connect(win->gw,SIGNAL(toggle()),this,SLOT(refresh()));
        connect(win->b1,SIGNAL(toggled(bool)),this,SLOT(updateViewer()));
        connect(win->b2,SIGNAL(toggled(bool)),this,SLOT(updateViewer()));
        connect(win->b3,SIGNAL(toggled(bool)),this,SLOT(updateViewer()));
        connect(win->b4,SIGNAL(toggled(bool)),this,SLOT(updateViewer()));
        connect(win->b5,SIGNAL(toggled(bool)),this,SLOT(updateViewer()));
    }
    bool imageCalculate(QImage &image){
        win->scene->clear();
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        updateViewer();
        return true;
    };
    static int lastIndex;
};

class BlurNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
public:
    sliderInt*slider1;
    BlurNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        slider1=new sliderInt;
        slider1->setText("Blur Radius(in px):");
        slider1->setRange(0,5);
        v1->addWidget(slider1);
        v1->addStretch();
        setLayout(v1);
    }
};

class blurNode:public node{
    Q_OBJECT
    BlurNodePropertiesWindow*win;
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
public:
    blurNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"blurNode"+QString::number(lastIndex++)){
        win=new BlurNodePropertiesWindow(name);
        propW=win;
        connect(win->slider1->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        blur(image,win->slider1->getValue());
        return true;
    };
    static int lastIndex;
};

class SaturateNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
public:
    sliderFloat*slider1;
    SaturateNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        slider1=new sliderFloat;
        slider1->setText("Saturation:");
        slider1->setRange(-100,100,100);
        v1->addWidget(slider1);
        v1->addStretch();
        setLayout(v1);
    }
};

class saturateNode:public node{
    Q_OBJECT
    SaturateNodePropertiesWindow*win;
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
public:
    saturateNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"saturateNode"+QString::number(lastIndex++)){
        win=new SaturateNodePropertiesWindow(name);
        propW=win;
        connect(win->slider1->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        saturate(image,win->slider1->getValue());
        return true;
    };
    static int lastIndex;
};

class ContrastNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
public:
    sliderInt*slider1;
    ContrastNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        slider1=new sliderInt;
        slider1->setText("Saturation:");
        slider1->setRange(-255,255);
        v1->addWidget(slider1);
        v1->addStretch();
        setLayout(v1);
    }
};

class contrastNode:public node{
    Q_OBJECT
    ContrastNodePropertiesWindow*win;
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
public:
    contrastNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"contrastNode"+QString::number(lastIndex++)){
        win=new ContrastNodePropertiesWindow(name);
        propW=win;
        connect(win->slider1->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        contrast(image,win->slider1->getValue());
        return true;
    };
    static int lastIndex;
};

class GradeNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*vBoxLayout1;
public:
    sliderFloat*sliderLift;
    sliderFloat*sliderGain;
    sliderFloat*sliderOffset;
    GradeNodePropertiesWindow(QString title):PropertiesWindow(title){
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
};

class gradeNode:public node{
    Q_OBJECT
    GradeNodePropertiesWindow*win;
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
public:
    gradeNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"gradeNode"+QString::number(lastIndex++)){
        win=new GradeNodePropertiesWindow(name);
        propW=win;
        connect(win->sliderLift->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
        connect(win->sliderGain->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
        connect(win->sliderOffset->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        grade(image,win->sliderLift->getValue(),win->sliderGain->getValue(),win->sliderOffset->getValue());
        return true;
    };
    static int lastIndex;
};

class GammaNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*vBoxLayout1;
public:
    sliderFloat*slider1;
    GammaNodePropertiesWindow(QString title):PropertiesWindow(title){
        slider1=new sliderFloat;
        slider1->setText("Gamma:");
        slider1->setRange(0,500,100);
        slider1->setDefaultValue(1);
        vBoxLayout1=new QVBoxLayout;
        vBoxLayout1->addWidget(slider1);
        vBoxLayout1->insertStretch(1);
        setLayout(vBoxLayout1);
    }
};

class gammaNode:public node{
    Q_OBJECT
    GammaNodePropertiesWindow*win;
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
public:
    gammaNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"gammaNode"+QString::number(lastIndex++)){
        win=new GammaNodePropertiesWindow(name);
        propW=win;
        connect(win->slider1->getSlider(),SIGNAL(valueChanged(int)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        gammaCorrect(image,win->slider1->getValue());
        return true;
    };
    static int lastIndex;
};

class MirrorNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*h1;
public:
    QCheckBox*hbox,*vbox;
    MirrorNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        h1=new QHBoxLayout;
        hbox=new QCheckBox("Mirror Horizontally");
        vbox=new QCheckBox("Mirror Vertically");
        h1->addWidget(hbox);
        h1->addWidget(vbox);
        h1->addStretch();
        v1->addLayout(h1);
        v1->addStretch();
        setLayout(v1);
    }
};

class mirrorNode:public node{
    Q_OBJECT
    MirrorNodePropertiesWindow*win;
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
public:
    mirrorNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"mirrorNode"+QString::number(lastIndex++)){
        win=new MirrorNodePropertiesWindow(name);
        propW=win;
        connect(win->hbox,SIGNAL(stateChanged(int)),this,SLOT(refresh()));
        connect(win->vbox,SIGNAL(stateChanged(int)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        if(win->hbox->checkState()==Qt::Checked) mirror_imgH(image);
        if(win->vbox->checkState()==Qt::Checked) mirror_imgV(image);
        return true;
    };
    static int lastIndex;
};

class RotateNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1;
    QHBoxLayout*hBoxLayout2;
    QLabel*label1;
public:
    QSpinBox*spinBox1;
    QButtonGroup*buttonGroup1;
    QRadioButton*radioButton1;
    QRadioButton*radioButton2;
    RotateNodePropertiesWindow(QString title):PropertiesWindow(title){
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
};

class rotateNode:public node{
    Q_OBJECT
    RotateNodePropertiesWindow*win;
public:
    rotateNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"rotateNode"+QString::number(lastIndex++)){
        win=new RotateNodePropertiesWindow(name);
        propW=win;
        connect(win->spinBox1,SIGNAL(valueChanged(int)),this,SLOT(refresh()));
        connect(win->radioButton1,SIGNAL(clicked(bool)),this,SLOT(refresh()));
        connect(win->radioButton2,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        Qt::TransformationMode tm;
        if(win->buttonGroup1->checkedId()==0) tm=Qt::FastTransformation;
        else tm=Qt::SmoothTransformation;
        QTransform tr;
        tr.rotate(win->spinBox1->value());
        image=image.transformed(tr,tm);
        return true;
    };
    static int lastIndex;
};

class ScaleNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*vBoxLayout1;
    QHBoxLayout*hBoxLayout1;
    QHBoxLayout*hBoxLayout2;
    QHBoxLayout*hBoxLayout3;
    QLabel*label1,*label2;
public:
    QSpinBox*sb1,*sb2;
    QRadioButton*radioButton1,*radioButton2,*radioButton3;
    QRadioButton*radioButton4,*radioButton5;
    QButtonGroup*buttonGroup1;
    QButtonGroup*buttonGroup2;
    QCheckBox*box1;
    ScaleNodePropertiesWindow(QString title):PropertiesWindow(title){
        label1=new QLabel("Width(in px):");
        label2=new QLabel("Height(in px):");
        sb1=new QSpinBox;
        sb2=new QSpinBox;
        sb1->setRange(0,INT_MAX);
        sb2->setRange(0,INT_MAX);
        box1=new QCheckBox("Lock fields");

        hBoxLayout1=new QHBoxLayout;
        hBoxLayout1->addWidget(label1);
        hBoxLayout1->addWidget(sb1);
        hBoxLayout1->addWidget(label2);
        hBoxLayout1->addWidget(sb2);
        hBoxLayout1->addWidget(box1);
        hBoxLayout1->addStretch();

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
};

class scaleNode:public node{
    Q_OBJECT
    ScaleNodePropertiesWindow*win;
    QImage image;
protected slots:
    void refresh(){
        if(input1==NULL) image=QImage();
        else if(!input1->imageCalculate(image)) image=QImage();
        if(!win->box1->isChecked()){
            win->sb1->setValue(image.width());
            win->sb2->setValue(image.height());
        }
        refresh2();
    }
    void refresh2(){
        for(auto it=output.begin();it!=output.end();it++) it->first->refresh();
    }
    void onBox1Checked(bool checked){
        if(checked){
            win->sb1->setDisabled(true);
            win->sb2->setDisabled(true);
        }else{
            win->sb1->setEnabled(true);
            win->sb2->setEnabled(true);
        }
    }
public:
    scaleNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"scaleNode"+QString::number(lastIndex++)){
        win=new ScaleNodePropertiesWindow(name);
        propW=win;
        connect(win->box1,SIGNAL(toggled(bool)),this,SLOT(onBox1Checked(bool)));
        connect(win->sb1,SIGNAL(valueChanged(int)),this,SLOT(refresh2()));
        connect(win->sb2,SIGNAL(valueChanged(int)),this,SLOT(refresh2()));
        connect(win->radioButton1,SIGNAL(clicked(bool)),this,SLOT(refresh2()));
        connect(win->radioButton2,SIGNAL(clicked(bool)),this,SLOT(refresh2()));
        connect(win->radioButton3,SIGNAL(clicked(bool)),this,SLOT(refresh2()));
        connect(win->radioButton4,SIGNAL(clicked(bool)),this,SLOT(refresh2()));
        connect(win->radioButton5,SIGNAL(clicked(bool)),this,SLOT(refresh2()));
    }
    bool imageCalculate(QImage &image){
        if(this->image.isNull()) return false;
        Qt::AspectRatioMode arm;
        if(win->buttonGroup1->checkedId()==0) arm=Qt::IgnoreAspectRatio;
        else if(win->buttonGroup1->checkedId()==1) arm=Qt::KeepAspectRatio;
        else arm=Qt::KeepAspectRatioByExpanding;
        Qt::TransformationMode tm;
        if(win->buttonGroup2->checkedId()==0) tm=Qt::FastTransformation;
        else tm=Qt::SmoothTransformation;
        image=this->image.scaled(win->sb1->value(),win->sb2->value(),arm,tm);
        if(image.isNull()) return false;
        return true;
    };
    static int lastIndex;
};

class CropNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*h1,*h2,*h3;
    QLabel*l1,*l2,*l3,*l4;
public:
    QSpinBox*b1,*b2,*b3,*b4;
    QCheckBox*check1;
    CropNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        h1=new QHBoxLayout;
        h2=new QHBoxLayout;
        h3=new QHBoxLayout;
        l1=new QLabel("x1(in px):");
        l2=new QLabel("y1(in px):");
        l3=new QLabel("x2(in px):");
        l4=new QLabel("y2(in px):");
        b1=new QSpinBox;
        b1->setRange(INT_MIN,INT_MAX);
        b2=new QSpinBox;
        b2->setRange(INT_MIN,INT_MAX);
        b3=new QSpinBox;
        b3->setRange(INT_MIN,INT_MAX);
        b4=new QSpinBox;
        b4->setRange(INT_MIN,INT_MAX);
        check1=new QCheckBox("Lock Fields");

        h1->addWidget(l1);
        h1->addWidget(b1);
        h1->addWidget(l2);
        h1->addWidget(b2);
        h1->addStretch();

        h2->addWidget(l3);
        h2->addWidget(b3);
        h2->addWidget(l4);
        h2->addWidget(b4);
        h2->addStretch();

        h3->addWidget(check1);
        h3->addStretch();

        v1->addLayout(h1);
        v1->addLayout(h2);
        v1->addLayout(h3);
        v1->addStretch();
        setLayout(v1);
    }
};

class cropNode:public node{
    Q_OBJECT
    CropNodePropertiesWindow*win;
    QImage image;
protected slots:
    void refresh(){
        if(input1==NULL) image=QImage();
        else if(!input1->imageCalculate(image)) image=QImage();
        if(!win->check1->isChecked()){
            win->b1->setValue(0);
            win->b2->setValue(0);
            win->b3->setValue(image.width());
            win->b4->setValue(image.height());
        }
        refresh2();
    }
    void refresh2(){
        for(auto it=output.begin();it!=output.end();it++) it->first->refresh();
    }
    void onCheck1Clicked(bool checked){
        if(checked){
            win->b1->setDisabled(true);
            win->b2->setDisabled(true);
            win->b3->setDisabled(true);
            win->b4->setDisabled(true);
        }else{
            win->b1->setEnabled(true);
            win->b2->setEnabled(true);
            win->b3->setEnabled(true);
            win->b4->setEnabled(true);
        }
    }
public:
    cropNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"cropNode"+QString::number(lastIndex++)){
        win=new CropNodePropertiesWindow(name);
        propW=win;
        connect(win->b1,SIGNAL(valueChanged(int)),this,SLOT(refresh2()));
        connect(win->b2,SIGNAL(valueChanged(int)),this,SLOT(refresh2()));
        connect(win->b3,SIGNAL(valueChanged(int)),this,SLOT(refresh2()));
        connect(win->b4,SIGNAL(valueChanged(int)),this,SLOT(refresh2()));
        connect(win->check1,SIGNAL(toggled(bool)),this,SLOT(onCheck1Clicked(bool)));
    }
    bool imageCalculate(QImage &image){
        if(this->image.isNull()) return false;
        int x1=win->b1->value();
        int y1=win->b2->value();
        int x2=win->b3->value();
        int y2=win->b4->value();
        image=this->image.copy(x1,y1,x2-x1,y2-y1);
        if(image.isNull()) return false;
        return true;
    };
    static int lastIndex;
};

class WriteNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*h1,*h2;
public:
    sliderInt*slider1;
    QPushButton*b1;
    WriteNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        h1=new QHBoxLayout;
        h2=new QHBoxLayout;
        slider1=new sliderInt;
        slider1->setRange(0,100);
        slider1->setText("Quality Factor:");
        slider1->setDefaultValue(100);
        b1=new QPushButton("Save Image");
        h1->addWidget(slider1);
        h2->addStretch();
        h2->addWidget(b1);
        v1->addLayout(h1);
        v1->addLayout(h2);
        v1->addStretch();
        setLayout(v1);
    }
};

class writeNode:public node{
    Q_OBJECT
    WriteNodePropertiesWindow*win;
    QImage image;
protected slots:
    void onB1Clicked(){
        if(input1==NULL) return;
        input1->imageCalculate(image);
        if(image.isNull()) return;
        QString fileName=QFileDialog::getSaveFileName(win,"Save Image File","","Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");
        image.save(fileName,NULL,win->slider1->getValue());
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QRectF rect=boundingRect();
        QPainterPath path;
        path.addRoundedRect(rect,15,15);
        QLinearGradient grad(0,0,0,height);
        grad.setColorAt(0,QColor(220,220,220));
        grad.setColorAt(0.5,QColor(170,170,170));
        grad.setColorAt(1,QColor(220,220,220));
        painter->fillPath(path,grad);
        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPath(path);
        QFont font;
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(rect,Qt::AlignCenter,name);
        font.setPixelSize(10);
        painter->setFont(font);
        if(iType==oneInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/2-ci1->getWidth()/2-5,15);
            painter->drawText(rect1,Qt::AlignRight,"input");
        }else if(iType==twoInput){
            QRectF rect1(0,ci1->getHeight()/2-1,width/4-ci1->getWidth()/2-2,15);
            painter->drawText(rect1,Qt::AlignRight,"A");
            rect1.setWidth(3*width/4-ci2->getWidth()/2-2);
            painter->drawText(rect1,Qt::AlignRight,"B");
        }
        if(oType==oneOutput){
            QRectF rect1(0,0,width/2-co->getWidth()/2-5,height-co->getHeight()/2+2);
            painter->drawText(rect1,Qt::AlignRight|Qt::AlignBottom,"output");
        }
    };
public:
    writeNode(QGraphicsScene*scene):node(scene,oneInput,noOutput,"writeNode"+QString::number(lastIndex++)){
        win=new WriteNodePropertiesWindow(name);
        propW=win;
        connect(win->b1,SIGNAL(clicked()),this,SLOT(onB1Clicked()));
    }
    static int lastIndex;
};

class ShuffleNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*hbox[4];
    QVBoxLayout*vbox[6];
    QButtonGroup*group[4];
public:
    QRadioButton* buttons[4][6];
    ShuffleNodePropertiesWindow(QString title):PropertiesWindow(title){
        QLabel*tempLabel;
        v1=new QVBoxLayout;
        for(int i=0;i<4;i++){
            hbox[i]=new QHBoxLayout;
            group[i]=new QButtonGroup(this);
            for(int j=0;j<6;j++){
                vbox[j]=new QVBoxLayout;
                buttons[i][j]=new QRadioButton;
            }
        }
        tempLabel=new QLabel("R");
        tempLabel->setAlignment(Qt::AlignCenter);
        vbox[0]->addWidget(tempLabel);
        tempLabel=new QLabel("G");
        tempLabel->setAlignment(Qt::AlignCenter);
        vbox[1]->addWidget(tempLabel);
        tempLabel=new QLabel("B");
        tempLabel->setAlignment(Qt::AlignCenter);
        vbox[2]->addWidget(tempLabel);
        tempLabel=new QLabel("A");
        tempLabel->setAlignment(Qt::AlignCenter);
        vbox[3]->addWidget(tempLabel);
        tempLabel=new QLabel("0");
        tempLabel->setAlignment(Qt::AlignCenter);
        vbox[4]->addWidget(tempLabel);
        tempLabel=new QLabel("1");
        tempLabel->setAlignment(Qt::AlignCenter);
        vbox[5]->addWidget(tempLabel);

        for(int i=0;i<6;i++){
            vbox[i]->addWidget(buttons[0][i]);
            hbox[0]->addLayout(vbox[i]);
            group[0]->addButton(buttons[0][i]);
        }
        for(int i=1;i<4;i++){
            for(int j=0;j<6;j++){
                hbox[i]->addWidget(buttons[i][j]);
                group[i]->addButton(buttons[i][j]);
            }
        }
        for(int i=0;i<4;i++) v1->addLayout(hbox[i]);
        v1->addStretch();
        tempLabel=new QLabel("R");
        tempLabel->setAlignment(Qt::AlignBottom);
        hbox[0]->addWidget(tempLabel);
        hbox[1]->addWidget(new QLabel("G"));
        hbox[2]->addWidget(new QLabel("B"));
        hbox[3]->addWidget(new QLabel("A"));
        for(int i=0;i<4;i++){
            hbox[i]->insertSpacing(4,25);
            hbox[i]->addStretch();
        }
        setLayout(v1);
        for(int i=0;i<4;i++) buttons[i][i]->setChecked(true);
    }
};

class shuffleNode:public node{
    Q_OBJECT
    ShuffleNodePropertiesWindow*win;
public:
    shuffleNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"shuffleNode"+QString::number(lastIndex++)){
        win=new ShuffleNodePropertiesWindow(name);
        propW=win;
        for(int i=0;i<4;i++){
            for(int j=0;j<6;j++) connect(win->buttons[i][j],SIGNAL(toggled(bool)),this,SLOT(refresh()));
        }
    }
    bool imageCalculate(QImage &image){
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        QImage imageCopy=image;
        for(int y=0;y<image.height();y++){
            for(int x=0;x<image.width();x++){
                QColor co=imageCopy.pixelColor(x,y);
                QColor c;
                if(win->buttons[0][0]->isChecked()) c.setRed(co.red());
                if(win->buttons[0][1]->isChecked()) c.setRed(co.green());
                if(win->buttons[0][2]->isChecked()) c.setRed(co.blue());
                if(win->buttons[0][3]->isChecked()) c.setRed(co.alpha());
                if(win->buttons[0][4]->isChecked()) c.setRed(0);
                if(win->buttons[0][5]->isChecked()) c.setRed(255);

                if(win->buttons[1][0]->isChecked()) c.setGreen(co.red());
                if(win->buttons[1][1]->isChecked()) c.setGreen(co.green());
                if(win->buttons[1][2]->isChecked()) c.setGreen(co.blue());
                if(win->buttons[1][3]->isChecked()) c.setGreen(co.alpha());
                if(win->buttons[1][4]->isChecked()) c.setGreen(0);
                if(win->buttons[1][5]->isChecked()) c.setGreen(255);

                if(win->buttons[2][0]->isChecked()) c.setBlue(co.red());
                if(win->buttons[2][1]->isChecked()) c.setBlue(co.green());
                if(win->buttons[2][2]->isChecked()) c.setBlue(co.blue());
                if(win->buttons[2][3]->isChecked()) c.setBlue(co.alpha());
                if(win->buttons[2][4]->isChecked()) c.setBlue(0);
                if(win->buttons[2][5]->isChecked()) c.setBlue(255);

                if(win->buttons[3][0]->isChecked()) c.setAlpha(co.red());
                if(win->buttons[3][1]->isChecked()) c.setAlpha(co.green());
                if(win->buttons[3][2]->isChecked()) c.setAlpha(co.blue());
                if(win->buttons[3][3]->isChecked()) c.setAlpha(co.alpha());
                if(win->buttons[3][4]->isChecked()) c.setAlpha(0);
                if(win->buttons[3][5]->isChecked()) c.setAlpha(255);
                image.setPixelColor(x,y,c);
            }
        }
        return true;
    }
    static int lastIndex;
};
