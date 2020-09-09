#include "qualityfactordialog.h"
#include "ui_qualityfactordialog.h"

QualityFactorDialog::QualityFactorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QualityFactorDialog)
{
    ui->setupUi(this);
    qualityFactor=100;
}

QualityFactorDialog::~QualityFactorDialog()
{
    delete ui;
}

void QualityFactorDialog::on_horizontalSlider_valueChanged(int value)
{
    //Quality factor slider
    ui->spinBox->setValue(value);
    qualityFactor=value;
}

void QualityFactorDialog::on_buttonBox_accepted()
{
    //Accept
    accept();
}

void QualityFactorDialog::on_buttonBox_rejected()
{
    //Cancel
    reject();
}
