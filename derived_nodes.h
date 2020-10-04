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
        return ans;
    };
    static int lastIndex;
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
    QLineEdit*lineEdit1,*lineEdit2;
    QRadioButton*radioButton1,*radioButton2,*radioButton3;
    QRadioButton*radioButton4,*radioButton5;
    QButtonGroup*buttonGroup1;
    QButtonGroup*buttonGroup2;
    ScaleNodePropertiesWindow(QString title):PropertiesWindow(title){
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
};

class scaleNode:public node{
    Q_OBJECT
    ScaleNodePropertiesWindow*win;
    QImage image;
protected slots:
    void refresh(){
        if(input1==NULL) image=QImage();
        else if(!input1->imageCalculate(image)) image=QImage();
        win->lineEdit1->setText(QString::number(image.width()));
        win->lineEdit2->setText(QString::number(image.height()));
        refresh2();
    }
    void refresh2(){
        for(auto it=output.begin();it!=output.end();it++) it->first->refresh();
    }
public:
    scaleNode(QGraphicsScene*scene):node(scene,oneInput,oneOutput,"scaleNode"+QString::number(lastIndex++)){
        win=new ScaleNodePropertiesWindow(name);
        propW=win;
        connect(win->lineEdit1,SIGNAL(textEdited(QString)),this,SLOT(refresh2()));
        connect(win->lineEdit2,SIGNAL(textEdited(QString)),this,SLOT(refresh2()));
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
        image=this->image.scaled(win->lineEdit1->text().toInt(),win->lineEdit2->text().toInt(),arm,tm);
        return true;
    };
    static int lastIndex;
};
