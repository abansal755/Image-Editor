#ifndef QUALITYFACTORDIALOG_H
#define QUALITYFACTORDIALOG_H

#include <QDialog>

namespace Ui {
class qualityFactorDialog;
}

class qualityFactorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit qualityFactorDialog(QWidget *parent = nullptr);
    ~qualityFactorDialog();
    int getFactor();
private:
    Ui::qualityFactorDialog *ui;
};

#endif // QUALITYFACTORDIALOG_H
