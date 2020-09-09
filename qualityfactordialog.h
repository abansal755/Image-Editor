#ifndef QUALITYFACTORDIALOG_H
#define QUALITYFACTORDIALOG_H

#include <QDialog>

namespace Ui {
class QualityFactorDialog;
}

class QualityFactorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QualityFactorDialog(QWidget *parent = nullptr);
    ~QualityFactorDialog();
    int qualityFactor;
private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    Ui::QualityFactorDialog *ui;
};

#endif // QUALITYFACTORDIALOG_H
