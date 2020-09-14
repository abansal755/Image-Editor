#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :QDialog(parent),ui(new Ui::AboutDialog){
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowTitleHint|Qt::WindowCloseButtonHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonBox_accepted()
{
    //Ok
    this->accept();
}
