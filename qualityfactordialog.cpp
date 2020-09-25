#include "qualityfactordialog.h"
#include "ui_qualityfactordialog.h"

qualityFactorDialog::qualityFactorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qualityFactorDialog)
{
    ui->setupUi(this);
    ui->widget->setText("Qualit Factor:");
    ui->widget->setRange(0,100);
    ui->widget->setDefaultValue(100);
}

qualityFactorDialog::~qualityFactorDialog()
{
    delete ui;
}

int qualityFactorDialog::getFactor(){
    return ui->widget->getValue();
}
