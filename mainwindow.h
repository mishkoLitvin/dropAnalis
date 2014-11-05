#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QPolygonF>
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "interpolation.h"
#include "graphicplot.h"

//#define pi = 3.1415926;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStringList fileList;

    void openFileList();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *sceneOriginal, *scaledScene, *borderScene;
    QGraphicsLineItem *lineY, *lineX, *line1, *line2, *line3, *line4;
    QGraphicsPolygonItem *polygonBorderItem, *polygonBorderInterpolateItem,*polygonPillowItem;
    QGraphicsPolygonItem *spectrXPolygonItem, *spectrYPolygonItem;
    QGraphicsPixmapItem *scaledPixItem;

    QPixmap pixOriginal, pixScaled;
    QImage imgOriginal, imgScaled;
    QColor pointColor0, pointColor1, pointColor2;
    QPolygonF polygonPillow, spectrXPolygon, spectrYPolygon;
    QPolygonF polygonFull,polygonRight, polygonLeft, polygonBottom;

    QPointF pillowLeft, pillowRight, point0, pointTemp;
    int i, j, k, x1,x2, y1,y2,x0,y0,listCounter;
    int discret, xRightIndex, xLeftIndex, yBottomIndex, yMaxL, yMaxR;
    int forIntegralArray[3][1200], arrayIndex, arrayElementsCount, lastLX, lastRX, yPillow;
    float x,y, yMin, pointN;
    bool flag;
    float integral,weight,calibrateDiamPt,calibrateVolume,calibreteDiam;
    float dlIndex,drIndex,d,hIndex,h,r0,volume,density,surfTension,surfTension2,sTeta;
    QPointF lastPoint,polygonGraphPoint;
    QPolygonF polygonGraphPoints,polygonFullInterpolate,polygonFullEquation;
    QString namePix;
    QInterpolateClass interpolator;
    GraphicPlot *plotterBorder, *plotterDensity;

    QFile resultFile;

    float sqr(float x);
    float onePerH(float dPer2H);
    float fTeta(float teta);
    float dFTeta(float teta);
    float d2FTeta(float teta);


    float pi = 3.1415926;

private slots:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveFLeft();
    void moveFRight();
    void moveFUp();
    void moveFDown();
    void tabChanged(int number);
    void takePoint();
    void openImage();
    void openFileListSlot();
    void spectrUpdate();
    void interpolate();
    void mainCalculate();
    void saveToFile();
};

#endif // MAINWINDOW_H
