#ifndef SLIDERFLOAT_H
#define SLIDERFLOAT_H

#include <QWidget>

namespace Ui {
class sliderFloat;
}

class sliderFloat : public QWidget
{
    Q_OBJECT

public:
    explicit sliderFloat(QWidget *parent = nullptr);
    ~sliderFloat();
    void setText(QString text);
    void setRange(int min,int max,int divisor);
    float getValue();
    void setValue(float value);
private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::sliderFloat *ui;
    int divisor;
};

#endif // SLIDERFLOAT_H
