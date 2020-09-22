#ifndef SLIDERINT_H
#define SLIDERINT_H

#include <QWidget>

namespace Ui {
class sliderInt;
}

class sliderInt : public QWidget
{
    Q_OBJECT

public:
    explicit sliderInt(QWidget *parent = nullptr);
    ~sliderInt();
    void setText(QString text);
    void setRange(int min,int max);
    int getValue();
    void setValue(int value);
private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::sliderInt *ui;
};

#endif // SLIDERINT_H
