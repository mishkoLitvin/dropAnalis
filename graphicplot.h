#ifndef GRAPHICPLOT_H
#define GRAPHICPLOT_H

#include <QMainWindow>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <QPolygonF>
#include <QCloseEvent>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

namespace Ui {
class GraphicPlot;
}

class GraphicPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphicPlot(QWidget *parent = 0);
    ~GraphicPlot();


private:
    Ui::GraphicPlot *ui;
    QPolygonF polygonGraph;
    int pointsCount,i;
    QVector<QPointF> vectorGraph;
    QwtPlot *qwtPlot;
    QwtPlotCurve *plot;
    QwtPlotPicker *picker;
    QPixmap graphPixmap;
    QTextStream out;
    QFile outFile;


    void closeEvent(QCloseEvent *e);

public slots:
    void setPolygon(QPolygonF inputPolygon);
    void appendPoint(QPointF inputPoint);
    void printGraph();
    void saveDataSlot();
    void clean();


};

#endif // GRAPHICPLOT_H
