#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "graphwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow{
    Q_OBJECT
signals:
    void refreshSignal();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void on_actionOpen_image_triggered();
    void on_actionSave_Image_triggered();
    void on_actionClose_Image_triggered();
    void on_actionExit_triggered();
    void on_pushButton_3_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
    void on_horizontalSlider_7_valueChanged(int value);
    void on_horizontalSlider_8_valueChanged(int value);
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_actionGraph_triggered();
    void on_actionAbout_2_triggered();
    void refresh();
private:
    Ui::MainWindow *ui;
    QGraphicsScene*scene;
    QImage image,copy;
    QString fileExtensions="Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)";
    vector<QWidget*> statusChanging;
    void setDisabled();
    void setEnabled();
    GraphWindow*gw;
};
#endif // MAINWINDOW_H
