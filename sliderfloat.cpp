#include "sliderfloat.h"
#include "ui_sliderfloat.h"

sliderFloat::sliderFloat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sliderFloat)
{
    ui->setupUi(this);
    divisor=1;
}

sliderFloat::~sliderFloat()
{
    delete ui;
}

void sliderFloat::setText(QString text){
    ui->label->setText(text);
}

void sliderFloat::setRange(int min, int max,int divisor){
    this->divisor=divisor;
    ui->horizontalSlider->setRange(min,max);
    ui->doubleSpinBox->setRange((float)min/divisor,(float)max/divisor);
}

float sliderFloat::getValue(){
    return (float)(ui->horizontalSlider->value())/divisor;
}

void sliderFloat::setValue(float value){
    ui->horizontalSlider->setValue(value*divisor);
    ui->doubleSpinBox->setValue(value);
}

void sliderFloat::on_horizontalSlider_valueChanged(int value)
{
    ui->doubleSpinBox->setValue((float)value/divisor);
}
