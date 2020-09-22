#include "sliderint.h"
#include "ui_sliderint.h"

sliderInt::sliderInt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sliderInt)
{
    ui->setupUi(this);
}

sliderInt::~sliderInt()
{
    delete ui;
}

void sliderInt::setText(QString text){
    ui->label->setText(text);
}

void sliderInt::setRange(int min, int max){
    ui->horizontalSlider->setRange(min,max);
    ui->spinBox->setRange(min,max);
}

int sliderInt::getValue(){
    return ui->horizontalSlider->value();
}

void sliderInt::setValue(int value){
    ui->horizontalSlider->setValue(value);
    ui->spinBox->setValue(value);
}

void sliderInt::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
}
