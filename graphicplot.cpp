#include "graphicplot.h"
#include "ui_graphicplot.h"

GraphicPlot::GraphicPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphicPlot)
{
    ui->setupUi(this);
    qwtPlot = new QwtPlot();
    ui->qwtLay->addWidget(qwtPlot);
    plot = new QwtPlotCurve("gr1");
    // настройка функций
//    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,qwtPlot->canvas());
    picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
                QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
                QwtPicker::ActiveOnly, // включение/выключение
                qwtPlot->canvas() ); // ассоциация с полем

    // Цвет перпендикулярных линий
    picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    picker->setStateMachine( new QwtPickerDragPointMachine() );

    connect(ui->actionSave_image,SIGNAL(triggered()),this,SLOT(printGraph()));
    connect(ui->actionSave_data,SIGNAL(triggered()),this,SLOT(saveDataSlot()));
}

GraphicPlot::~GraphicPlot()
{
    delete ui;
}

void GraphicPlot::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}

void GraphicPlot::setPolygon(QPolygonF inputPolygon)
{
    pointsCount = inputPolygon.size();
    for(i=0;i<pointsCount;i++)
        vectorGraph.append(QPointF(inputPolygon[i].x(),inputPolygon[i].y()));
    plot->setSamples(vectorGraph);
    plot->attach(qwtPlot);
    qwtPlot->replot();
}

void GraphicPlot::appendPoint(QPointF inputPoint)
{
    vectorGraph.append(QPointF(inputPoint.x(),inputPoint.y()));
    plot->setSamples(vectorGraph);
    plot->attach(qwtPlot);
    qwtPlot->replot();
}

void GraphicPlot::printGraph()
{
    graphPixmap = qwtPlot->grab(QRect(QPoint(0,0),qwtPlot->size()));
    graphPixmap.save("img.png");
}

void GraphicPlot::saveDataSlot()
{
    outFile.setFileName(QFileDialog::getSaveFileName(this,"Save data...","/home/nekomata/Dropbox/Experiment"));
    outFile.open(QIODevice::WriteOnly);
    out.setDevice(&outFile);
    for(i=0;i<vectorGraph.length();i++)
        out<<vectorGraph[i].x()<<"\t"<<vectorGraph[i].y()<<"\n";
    outFile.close();
}

void GraphicPlot::clean()
{
    vectorGraph.clear();
}
