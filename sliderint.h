#ifndef SLIDERINT_H
#define SLIDERINT_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>

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
    void setDefaultValue(int value);
    QSlider* getSlider();
    QPushButton* getButton();
private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();
private:
    Ui::sliderInt *ui;
    int defaultValue;
};

#endif // SLIDERINT_H
