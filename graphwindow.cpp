#include "graphwindow.h"
#include "ui_graphwindow.h"

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
    chart=new QChart;
    ui->graphicsView->setChart(chart);
    chart->legend()->hide();

    axisX=new QValueAxis(this);
    axisY=new QValueAxis(this);
    axisX->setRange(0,1);
    axisY->setRange(0,1);

    seriesR=new QScatterSeries(this);
    seriesG=new QScatterSeries(this);
    seriesB=new QScatterSeries(this);
    chart->addSeries(seriesR);
    chart->addSeries(seriesG);
    chart->addSeries(seriesB);

    QPen redPen(Qt::red);
    QBrush redBrush(Qt::red);
    seriesR->setPen(redPen);
    seriesR->setBrush(redBrush);
    seriesR->setMarkerSize(3);

    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);
    seriesG->setPen(greenPen);
    seriesG->setBrush(greenBrush);
    seriesG->setMarkerSize(3);

    QPen bluePen(Qt::blue);
    QBrush blueBrush(Qt::blue);
    seriesB->setPen(bluePen);
    seriesB->setBrush(blueBrush);
    seriesB->setMarkerSize(3);

    chart->setAxisX(axisX,seriesR);
    chart->setAxisY(axisY,seriesR);
    chart->setAxisX(axisX,seriesG);
    chart->setAxisY(axisY,seriesG);
    chart->setAxisX(axisX,seriesB);
    chart->setAxisY(axisY,seriesB);
}

GraphWindow::~GraphWindow()
{
    delete chart;
    delete ui;
}

void GraphWindow::on_pushButton_clicked()
{
    //Generate
    seriesR->clear();
    seriesG->clear();
    seriesB->clear();

    bool red=ui->checkBox->isChecked();
    bool green=ui->checkBox_3->isChecked();
    bool blue=ui->checkBox_2->isChecked();
    int width=copy->width();
    int height=copy->height();
    int factor=sqrt(((float)width*height)/ui->horizontalSlider->value());

    for(int y=0;y<copy->height();y+=factor){
        for(int x=0;x<copy->width();x+=factor){
            QColor c=copy->pixelColor(x,y);
            QColor i=image->pixelColor(x,y);
            if(red) seriesR->append(c.redF(),i.redF());
            if(green) seriesG->append(c.greenF(),i.greenF());
            if(blue) seriesB->append(c.blueF(),i.blueF());
        }
    }
}

void GraphWindow::on_horizontalSlider_valueChanged(int value)
{
    //Points per channel
    ui->spinBox->setValue(value);
}
