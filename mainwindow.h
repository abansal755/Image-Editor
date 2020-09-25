#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include "node.h"
#include "canvas.h"
#include "graphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
private:
    Ui::MainWindow *ui;
    QGraphicsScene*scene;
    vector<node*> destruc;
    canvas*canv;
};
#endif // MAINWINDOW_H
