#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QtCharts>

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();
    QImage*image,*copy;
private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::GraphWindow *ui;
    QScatterSeries *seriesR,*seriesG,*seriesB;
    QChart *chart;
    QValueAxis *axisX,*axisY;
};

#endif // GRAPHWINDOW_H
