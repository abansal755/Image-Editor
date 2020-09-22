#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include "node.h"
#include "canvas.h"
#include "graphicsview.h"
#include "imageviewer.h"

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
    void on_actionViewer_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene*scene;
    vector<node*> destruc;
    canvas*canv;
    viewerNode*viewNode;
    ImageViewer*imageViewer;
};
#endif // MAINWINDOW_H
