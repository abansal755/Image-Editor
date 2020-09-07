#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void on_actionOpen_image_triggered();
    void on_actionSave_Image_triggered();
    void on_actionClose_Image_triggered();
    void on_actionExit_triggered();
    void on_horizontalSlider_3_valueChanged(int value);
    void on_pushButton_2_clicked();
    void on_actionAbout_triggered();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene*scene;
    QImage image,copy;
    QString fileExtensions="Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)";
};
#endif // MAINWINDOW_H
