#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->addWidget(ui->menuBar);
    this->setWindowTitle("Graph intergator");
    this->setWindowIcon(QIcon(":/new/img/icon1"));
    ui->spinBoxDiscret->setMaximum(255);
    ui->spinBoxDiscret->setValue(25);
    sceneOriginal = new QGraphicsScene(this);
    ui->graphicsViewOriginal->setScene(sceneOriginal);

    scaledScene = new QGraphicsScene(this);
    ui->graphicsViewScaled->setScene(scaledScene);

    plotterBorder = new GraphicPlot(this);
    plotterBorder->show();

    plotterDensity = new GraphicPlot(this);
    plotterDensity->show();

    scaledPixItem = scaledScene->addPixmap(QPixmap(QSize(300,300)));
    scaledScene->addLine(0,150,300,150,QPen(Qt::green,2));
    scaledScene->addLine(150,0,150,300,QPen(Qt::green,2));

    spectrXPolygonItem = scaledScene->addPolygon(spectrXPolygon,QPen(Qt::blue,1));
    spectrYPolygonItem = scaledScene->addPolygon(spectrYPolygon,QPen(Qt::blue,1));
    spectrYPolygonItem->moveBy(0,300);
    spectrYPolygonItem->setRotation(-90);

    borderScene = new QGraphicsScene(this);
    ui->graphicsViewBorder->setScene(borderScene);

    pointN = 0;

    connect(ui->actionOpen_image,SIGNAL(triggered()),this,SLOT(openImage()));
    connect(ui->actionMove_left,SIGNAL(triggered()),this,SLOT(moveLeft()));
    connect(ui->actionMove_right,SIGNAL(triggered()),this,SLOT(moveRight()));
    connect(ui->actionMove_down,SIGNAL(triggered()),this,SLOT(moveDown()));
    connect(ui->actionMove_up,SIGNAL(triggered()),this,SLOT(moveUp()));
    connect(ui->actionMoveF_left,SIGNAL(triggered()),this,SLOT(moveFLeft()));
    connect(ui->actionMoveF_right,SIGNAL(triggered()),this,SLOT(moveFRight()));
    connect(ui->actionMoveF_down,SIGNAL(triggered()),this,SLOT(moveFDown()));
    connect(ui->actionMoveF_up,SIGNAL(triggered()),this,SLOT(moveFUp()));
    connect(ui->actionOpen_file_list,SIGNAL(triggered()),this,SLOT(openFileListSlot()));
    connect(ui->actionTake_point,SIGNAL(triggered()),this,SLOT(takePoint()));
    connect(ui->actionInterpolate,SIGNAL(triggered()),this,SLOT(interpolate()));
//    connect(ui->actionCalculate,SIGNAL(triggered()),this,SLOT(mainCalculate()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
    connect(ui->pushButtonSave,SIGNAL(clicked()),this,SLOT(saveToFile()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::sqr(float x)
{
    return x*x;
}

float MainWindow::onePerH(float dPer2H)
{
    return 7502.04982-42177.33457*dPer2H+105228.63675*pow(dPer2H,2)-152848.47508*pow(dPer2H,3)
            +142408.17317*pow(dPer2H,4)-88239.26116*pow(dPer2H,5)+36356.19147*pow(dPer2H,6)
            -9603.82359*pow(dPer2H,7)+1475.80437*pow(dPer2H,8)-100.51028*pow(dPer2H,9);
}

float MainWindow::fTeta(float teta)
{
    y = yMin;
    for(i=0;i<polygonFullEquation.size();i++)
        y = y + polygonFullEquation[i].y()
                *pow(teta,polygonFullEquation[i].x());
    return y;
}

float MainWindow::dFTeta(float teta)
{
    y = 0;
    for(i=0;i<polygonFullEquation.size();i++)
        y = y + polygonFullEquation[i].y()*polygonFullEquation[i].x()
                *pow(teta,polygonFullEquation[i].x()-1);
    return y;
}

float MainWindow::d2FTeta(float teta)
{
    y = 0;
    for(i=1;i<polygonFullEquation.size();i++)
        y = y + polygonFullEquation[i].y()*polygonFullEquation[i].x()*(polygonFullEquation[i].x()-1)*
                pow(teta,polygonFullEquation[i].x()-2);
    return y;
}

void MainWindow::moveLeft()
{
    lineY->moveBy(-1,0);
    lineX->moveBy(-1,0);
    line1->moveBy(-1,0);
    line2->moveBy(-1,0);
    line3->moveBy(-1,0);
    line4->moveBy(-1,0);
    this->spectrUpdate();
}

void MainWindow::moveRight()
{
    lineY->moveBy(1,0);
    lineX->moveBy(1,0);
    line1->moveBy(1,0);
    line2->moveBy(1,0);
    line3->moveBy(1,0);
    line4->moveBy(1,0);
    this->spectrUpdate();
}

void MainWindow::moveUp()
{
    lineY->moveBy(0,-1);
    lineX->moveBy(0,-1);
    line1->moveBy(0,-1);
    line2->moveBy(0,-1);
    line3->moveBy(0,-1);
    line4->moveBy(0,-1);
    this->spectrUpdate();
}

void MainWindow::moveDown()
{
    lineY->moveBy(0,1);
    lineX->moveBy(0,1);
    line1->moveBy(0,1);
    line2->moveBy(0,1);
    line3->moveBy(0,1);
    line4->moveBy(0,1);
    this->spectrUpdate();
}

void MainWindow::moveFLeft()
{
    lineY->moveBy(-20,0);
    lineX->moveBy(-20,0);
    line1->moveBy(-20,0);
    line2->moveBy(-20,0);
    line3->moveBy(-20,0);
    line4->moveBy(-20,0);
    this->spectrUpdate();
}

void MainWindow::moveFRight()
{
    lineY->moveBy(20,0);
    lineX->moveBy(20,0);
    line1->moveBy(20,0);
    line2->moveBy(20,0);
    line3->moveBy(20,0);
    line4->moveBy(20,0);
    this->spectrUpdate();
}

void MainWindow::moveFUp()
{
    lineY->moveBy(0,-20);
    lineX->moveBy(0,-20);
    line1->moveBy(0,-20);
    line2->moveBy(0,-20);
    line3->moveBy(0,-20);
    line4->moveBy(0,-20);
    this->spectrUpdate();
}

void MainWindow::moveFDown()
{
    lineY->moveBy(0,20);
    lineX->moveBy(0,20);
    line1->moveBy(0,20);
    line2->moveBy(0,20);
    line3->moveBy(0,20);
    line4->moveBy(0,20);
    this->spectrUpdate();
}

void MainWindow::tabChanged(int number)
{
    if(number==2)
    {
        ui->graphicsViewScaled->setHidden(true);
        ui->spinBoxDiscret->setHidden(true);
        ui->textEditOut->setHidden(true);
        ui->label->setHidden(true);
        ui->checkBoxInvert->setHidden(true);
    }
    else
    {
        ui->graphicsViewScaled->setHidden(false);
        ui->spinBoxDiscret->setHidden(false);
        ui->textEditOut->setHidden(false);
        ui->label->setHidden(false);
        ui->checkBoxInvert->setHidden(false);
    }
}

void MainWindow::takePoint()
{
    ui->textEditOut->append(QString::number(lineX->pos().x())+"; "+QString::number(lineX->pos().y()));
//    polygonBottom<<QPoint(lineX->pos().x(),lineY->pos().y());
//    polygonBorderItem->setPolygon(polygonBottom);
}

void MainWindow::openImage()
{
    sceneOriginal->clear();
    borderScene->clear();
    polygonLeft.clear();
    polygonRight.clear();
    polygonBottom.clear();
    polygonPillow.clear();
    polygonFull.clear();

    namePix = QFileDialog::getOpenFileName(this,"pix","/home/nekomata/Dropbox/Experiment/Experiment");
    pixOriginal.load(namePix);
    sceneOriginal->addPixmap(pixOriginal);
    borderScene->addPixmap(pixOriginal);
    QPen pen(Qt::blue);
    pen.setWidth(1);
    lineY = sceneOriginal->addLine(0,-1000,0,1000,pen);
    lineX = sceneOriginal->addLine(-1000,0,1000,0,pen);
    line1 = sceneOriginal->addLine(-30,-30,30,-30,pen);
    line2 = sceneOriginal->addLine(-30,-30,-30,30,pen);
    line3 = sceneOriginal->addLine(30,-30,30,30,pen);
    line4 = sceneOriginal->addLine(-30,30,30,30,pen);

    polygonBorderItem = borderScene->addPolygon(polygonBottom,pen);
    polygonPillowItem = borderScene->addPolygon(polygonPillow,QPen(Qt::red,1));

    pen.setColor(Qt::red);
    pen.setWidth(1);
}

void MainWindow::openFileListSlot()
{
    fileList = QFileDialog::getOpenFileNames(this,"Open images...","/home/nekomata/Dropbox/Experiment/Experiment");
    this->openFileList();
}

void MainWindow::openFileList()
{
    polygonGraphPoints.clear();
    QPen pen(Qt::blue);
    plotterDensity->clean();
    for(listCounter = 0; listCounter<fileList.size();listCounter++)
    {
        sceneOriginal->clear();
        borderScene->clear();
        polygonLeft.clear();
        polygonRight.clear();
        polygonBottom.clear();
        polygonPillow.clear();
        polygonFull.clear();
        namePix = fileList[listCounter];
        pixOriginal.load(namePix);
        sceneOriginal->addPixmap(pixOriginal);
        borderScene->addPixmap(pixOriginal);
        pen.setWidth(1);
        lineY = sceneOriginal->addLine(0,-1000,0,1000,pen);
        lineX = sceneOriginal->addLine(-1000,0,1000,0,pen);
        line1 = sceneOriginal->addLine(-30,-30,30,-30,pen);
        line2 = sceneOriginal->addLine(-30,-30,-30,30,pen);
        line3 = sceneOriginal->addLine(30,-30,30,30,pen);
        line4 = sceneOriginal->addLine(-30,30,30,30,pen);

        polygonBorderItem = borderScene->addPolygon(polygonBottom,pen);
        polygonPillowItem = borderScene->addPolygon(polygonPillow,QPen(Qt::red,1));

        this->interpolate();
    }

}

void MainWindow::spectrUpdate()
{
    ui->statusBar->showMessage(QString::number(lineX->pos().x())+"; "+QString::number(lineY->pos().y()),3000);

    if((lineX->pos().x()>0)
            &&(lineX->pos().x()<pixOriginal.width())
            &&(lineY->pos().y()>0)
            &&(lineY->pos().y()<pixOriginal.height()))
    {
        pixScaled = pixOriginal.copy(lineX->pos().x()-30,lineY->pos().y()-30,60,60);
        scaledPixItem->setPixmap(pixScaled.scaled(300,300));

        imgScaled = pixScaled.toImage();

        spectrXPolygon.clear();
        spectrXPolygon<<QPoint(0,300);
        for(i = 2;i<imgScaled.width();i++)
        {
            pointColor0 = imgScaled.pixel(i,30);
            pointColor1 = imgScaled.pixel(i-2,30);
            spectrXPolygon<<QPoint(i*5, 300-fabs(pointColor0.red()-pointColor1.red()));
        }
        spectrXPolygon<<QPoint(300,300);
        spectrXPolygonItem->setPolygon(spectrXPolygon);

        spectrYPolygon.clear();
        spectrYPolygon<<QPoint(0,0);
        for(j = 2;j<imgScaled.height();j++)
        {
            pointColor0 = imgScaled.pixel(30,j);
            pointColor1 = imgScaled.pixel(30,j-2);
            spectrYPolygon<<QPoint(j*5, fabs(pointColor0.red()-pointColor1.red()));
        }
        spectrYPolygon<<QPoint(300,0);
        spectrYPolygonItem->setPolygon(spectrYPolygon);
    }
}

void MainWindow::interpolate()
{
    polygonFull.clear();
    polygonFullInterpolate.clear();
    polygonPillow.clear();
    polygonBottom.clear();
    polygonLeft.clear();
    polygonRight.clear();

    imgOriginal = pixOriginal.toImage();
    discret = ui->spinBoxDiscret->value();
    y1 = 80;
    y2 = 80;
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Pillow
    for(i = 1; i<20;i++)
    {
        for(j =imgOriginal.height()-3;j>y1;j--)
        {
            if(!ui->checkBoxInvert->isChecked())
            {
                pointColor0 = imgOriginal.pixel(i,j);
                pointColor1 = imgOriginal.pixel(i,j+2);
                if(((pointColor0.red()-pointColor1.red())>30)||((pointColor0.blue()-pointColor1.blue())>30)||((pointColor0.green()-pointColor1.green())>30))
                {
                    polygonPillow<<QPointF(i,j);
                    break;
                }
            }
            else
            {
                pointColor0 = imgOriginal.pixel(i,j);
                pointColor1 = imgOriginal.pixel(i,j-2);
                if(((pointColor0.red()-pointColor1.red())>12)||((pointColor0.blue()-pointColor1.blue())>12)||((pointColor0.green()-pointColor1.green())>12))
                {
                    polygonPillow<<QPointF(i,j);
                    break;
                }
            }

        }
    }
    polygonPillowItem->setPolygon(polygonPillow);
    yPillow = polygonPillow[polygonPillow.size()-1].y();
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!left
    for(j=y1;j<yPillow;j++)
    {
        for(i=imgOriginal.width()-3; i>2;i--)
        {
            if(!ui->checkBoxInvert->isChecked())
            {
                pointColor0 = imgOriginal.pixel(i,j);
                pointColor2 = imgOriginal.pixel(i+2,j);
                if(((pointColor0.red()-pointColor2.red())>discret)||((pointColor0.blue()-pointColor2.blue())>discret)||((pointColor0.green()-pointColor2.green())>discret))
                {
                    polygonFull<<QPoint(i,j);
                    polygonLeft<<QPoint(i,j);
                    break;
                }
            }
            else
            {
                pointColor0 = imgOriginal.pixel(i,j);
                pointColor2 = imgOriginal.pixel(i-2,j);
                if(((pointColor0.red()-pointColor2.red())>discret)||((pointColor0.blue()-pointColor2.blue())>discret)||((pointColor0.green()-pointColor2.green())>discret))
                {
                    polygonFull<<QPoint(i-2,j);
                    polygonLeft<<QPoint(i-2,j);
                    break;
                }
            }

        }
    }

//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!right
    for(j=y2;j<yPillow;j++)
    {
        for(i=3; i<imgOriginal.width()-3;i++)
        {
            if(!ui->checkBoxInvert->isChecked())
            {
                pointColor0 = imgOriginal.pixel(i,j);
                pointColor2 = imgOriginal.pixel(i-2,j);
                if(((pointColor0.red()-pointColor2.red())>discret)||((pointColor0.blue()-pointColor2.blue())>discret)||((pointColor0.green()-pointColor2.green())>discret))
                {
                    polygonFull<<QPoint(i,j);
                    polygonRight<<QPoint(i,j);
                    break;
                }
            }
            else
            {
                pointColor0 = imgOriginal.pixel(i,j);
                pointColor2 = imgOriginal.pixel(i+2,j);
                if(((pointColor0.red()-pointColor2.red())>discret)||((pointColor0.blue()-pointColor2.blue())>discret)||((pointColor0.green()-pointColor2.green())>discret))
                {
                    polygonFull<<QPoint(i+2,j);
                    polygonRight<<QPoint(i+2,j);
                    break;
                }
            }
        }
    }

//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!bottom
    {
        x1 = polygonLeft[0].x();
        y1 = polygonLeft[0].y();
        x2 = polygonRight[0].x();
        y2 = polygonRight[0].y();
        for(i=x1; i<x2;i+=1)
        {
            for(j=y1;j>2;j--)
            {
                if(!ui->checkBoxInvert->isChecked())
                {
                    pointColor0 = imgOriginal.pixel(i,j);
                    pointColor1 = imgOriginal.pixel(i,j+2);
                    if(((pointColor0.red()-pointColor1.red())>discret)||((pointColor0.blue()-pointColor1.blue())>discret)||((pointColor0.green()-pointColor1.green())>discret))
                    {
                        polygonBottom<<QPoint(i,j);
                        polygonFull<<QPoint(i,j);
                        break;
                    }
                }
                else
                {
                    pointColor0 = imgOriginal.pixel(i,j);
                    pointColor1 = imgOriginal.pixel(i,j-2);
                    if(((pointColor0.red()-pointColor1.red())>discret)||((pointColor0.blue()-pointColor1.blue())>discret)||((pointColor0.green()-pointColor1.green())>discret))
                    {
                        polygonBottom<<QPoint(i,j-2);
                        polygonFull<<QPoint(i,j-2);
                        break;
                    }
                }
            }
        }
    }
    xLeftIndex = 0;
    for(i = 0; i<polygonLeft.size();i++)
        if(polygonLeft[i].x()<polygonLeft[xLeftIndex].x())
            xLeftIndex = i;
    xRightIndex = 0;
    for(i = 0; i<polygonRight.size();i++)
        if(polygonRight[i].x()>polygonRight[xRightIndex].x())
            xRightIndex = i;
    yBottomIndex = 0;
    for(i = 0;i<polygonBottom.size();i++)
        if(polygonBottom[i].y()<polygonBottom[yBottomIndex].y())
            yBottomIndex = i;

    y = yPillow;
    for(i = 0;i<1200;i++)
    {
        forIntegralArray[0][i] = -1;
        forIntegralArray[1][i] = -1;
        forIntegralArray[2][i] = -1;
    }
    arrayIndex = 0;

    for(j=polygonBottom[yBottomIndex].y();j<=y;j++)
    {
        forIntegralArray[0][arrayIndex] = j;
        for(i = 0;i<polygonFull.size();i++)
        {
            if(polygonFull[i].y() == j)
            {
                if(polygonFull[i].x()<(polygonRight[xRightIndex].x()+polygonLeft[xLeftIndex].x())/2)
                {
                    forIntegralArray[1][arrayIndex] = polygonFull[i].x();
                }
                else
                {
                    forIntegralArray[2][arrayIndex] = polygonFull[i].x();
                }
            }
        }
            arrayIndex++;
    }
    arrayElementsCount = arrayIndex-1;
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Integrate
    integral = 0;
    for(i = 0;i<arrayElementsCount;i++)
    {
        if((forIntegralArray[1][i]!=-1)&&(forIntegralArray[2][i]!=-1))
        {
            integral+=sqr(forIntegralArray[2][i]-forIntegralArray[1][i])/4.0*pi;
            lastLX = forIntegralArray[1][i];
            lastRX = forIntegralArray[2][i];
        }
        if((forIntegralArray[1][i]==-1)&&(forIntegralArray[2][i]!=-1))
        {
            integral+=sqr(forIntegralArray[2][i]-polygonBottom[yBottomIndex].x())*pi;
            lastRX = forIntegralArray[2][i];
        }
        if((forIntegralArray[1][i]!=-1)&&(forIntegralArray[2][i]==-1))
        {
            integral+=sqr(polygonBottom[yBottomIndex].x()-forIntegralArray[1][i])*pi;
            lastLX = forIntegralArray[1][i];
        }
        if((forIntegralArray[1][i]==-1)&&(forIntegralArray[2][i]==-1))
        {
            integral+=sqr(lastRX-lastLX)/4.0*pi;
        }
    }

    for(i = namePix.length();i>0;i--)
    {
        if((namePix.mid(i,1)=="\\")||(namePix.mid(i,1)=="/"))
        {
            ui->textEditOut->append(namePix.mid(i+1,namePix.length()-i-5)+" "+QString::number(integral));
            lastPoint = QPointF(namePix.mid(i+1,namePix.length()-i-5).toFloat(),integral);
            polygonGraphPoint = lastPoint;
            break;
        }
    }
    polygonBorderItem->setPolygon(polygonFull);
    this->mainCalculate();
}


void MainWindow::mainCalculate()
{
    weight = ui->lineEditWeight->text().toFloat();
    calibrateDiamPt = ui->lineEditVolume->text().toFloat();
    calibrateVolume = pi/6.0*pow(calibrateDiamPt,3);
    calibreteDiam = ui->lineEditDiametr->text().toFloat();

    volume = polygonGraphPoint.y()/calibrateVolume*pi/6.0*powf(calibreteDiam,3);
    density = weight/volume*1E6;
    x0 = (polygonRight[xRightIndex].x()-polygonLeft[xLeftIndex].x())/2.0+polygonLeft[xLeftIndex].x();
    y0 = yPillow;
//    qDebug()<<x0<<y0;
    polygonFullInterpolate.clear();
    plotterBorder->clean();
    for(i = 0;i<polygonFull.size()-1;i++)
    {
        point0 = QPointF(polygonFull[i].x()-x0,y0-polygonFull[i].y());
        point0.setX(point0.x()/calibrateDiamPt*calibreteDiam/1000.0);
        point0.setY(point0.y()/calibrateDiamPt*calibreteDiam/1000.0);
        plotterBorder->appendPoint(QPointF(point0.x()*1000.0,point0.y()*1000.0));
//        qDebug()<<point0.x()<<point0.y();
        if(point0.x()!=0)
        {
            polygonFullInterpolate<<QPointF(atan2(point0.x(),point0.y()),sqrt(point0.x()*point0.x()+point0.y()*point0.y()));
        }
        if(point0.x() ==0)
        {
            if(point0.y()>0)
                polygonFullInterpolate<<QPointF(0.0,sqrt(point0.y()*point0.y()));
            if(point0.y()<0)
                polygonFullInterpolate<<QPointF(-pi,sqrt(point0.y()*point0.y()));
        }
    }
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Sorting
    pointTemp = polygonFullInterpolate[0];
    for(;;)
    {
        flag = true;
        for(i = 0; i<polygonFullInterpolate.size()-1; i++)
        {
            if(polygonFullInterpolate[i].x()>polygonFullInterpolate[i+1].x())
            {
                pointTemp = polygonFullInterpolate[i];
                polygonFullInterpolate[i] = polygonFullInterpolate[i+1];
                polygonFullInterpolate[i+1] = pointTemp;
                flag = false;
            }
        }
        if(flag)
            break;
    }

//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Move circle graph
    for(i = 0;i<polygonFullInterpolate.size();i++)
    {
        if(fabs(polygonFullInterpolate[i].x())<0.01)
            yMin = polygonFullInterpolate[i].y();
    }

    for(i = 0;i<polygonFullInterpolate.size();i++)
    {
        polygonFullInterpolate[i].setY(polygonFullInterpolate[i].y()-yMin);
    }
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Interpolation
    interpolator.inputPolygon.clear();
    for(i = 00; i<polygonFullInterpolate.size();i++)
    {
        interpolator.inputPolygon <<polygonFullInterpolate[i];
//        qDebug()<<polygonFullInterpolate[i].x()<<polygonFullInterpolate[i].y();
    }
    interpolator.doIt(15);
    polygonFullEquation.clear();
    polygonFullEquation = interpolator.outputPolygon;
//    for(i=0;i<polygonFullEquation.size();i++)
//        qDebug()<<polygonFullEquation[i].x()<<polygonFullEquation[i].y();
    polygonFullInterpolate.clear();
    for(x=-pi/2.0;x<pi/2.0;x+=0.01)
    {
        polygonFullInterpolate<<QPointF(fTeta(x)*cos(x+pi/2.0),fTeta(x)*sin(x+pi/2.0));
        plotterBorder->appendPoint(QPointF(fTeta(x)*cos(x+pi/2.0)*1000.0,fTeta(x)*sin(x+pi/2.0)*1000.0));
//        plotterDensity->appendPoint(QPointF(x,fTeta(x)));
    }
    dlIndex = 0;
    drIndex = 0;
    hIndex = 0;
    sTeta = pi
            +(fTeta(pi/2.0)*cos(x+pi/2.0)-fTeta(pi/2.0-0.01)*cos(x-0.01+pi/2.0))
            /(fTeta(pi/2.0)*sin(x+pi/2.0)-fTeta(pi/2.0-0.01)*sin(x-0.01+pi/2.0));
    for(i = 0;i<polygonFullInterpolate.size();i++)
    {
        if(polygonFullInterpolate[i].x()<polygonFullInterpolate[dlIndex].x())
            dlIndex = i;
        if(polygonFullInterpolate[i].x()>polygonFullInterpolate[drIndex].x())
            drIndex = i;
        if(polygonFullInterpolate[i].y()>polygonFullInterpolate[hIndex].y())
            hIndex = i;
    }
    d = polygonFullInterpolate[drIndex].x()-polygonFullInterpolate[dlIndex].x();
    h = polygonFullInterpolate[hIndex].y()-((polygonFullInterpolate[drIndex].y()+polygonFullInterpolate[dlIndex].y())/2.0);
//    qDebug()<<sTeta*180/pi;
    qDebug()<<polygonGraphPoint.x()<<"\t"<<d<<"\t"<<h;
    surfTension2 = 9.81*density*sqr(polygonFullInterpolate[hIndex].y())/(2.0*(1-cos(sTeta)));
//    qDebug()<<polygonGraphPoint.x()<<sTeta*180/pi<<surfTension;
    plotterDensity->appendPoint(QPointF(polygonGraphPoint.x(),density));
//    qDebug()<<polygonGraphPoint.x()<<d<<h;
//    qDebug()<<polygonFullInterpolate[drIndex]<<polygonFullInterpolate[dlIndex]<<polygonFullInterpolate[hIndex];
    surfTension = sqr(d)*density*9.81*onePerH(d/(2.0*h));
    ui->textEditOut2->append(QString::number(polygonGraphPoint.x())+"\t"
                             +QString::number(sTeta*180.0/3.1415926)+"\t"
                             +QString::number(volume)+"\t"
                             +QString::number(density/1000.0)+"\t"
                             +QString::number(surfTension)+"\t"
                             +QString::number(surfTension2));
//    plotterDensity->appendPoint(QPointF(polygonGraphPoint.x(),density));
//    plotterDensity->appendPoint(QPointF(polygonGraphPoint.x(),sTeta));
//    plotterBorder->show();
//    plotterDensity->show();
}

void MainWindow::saveToFile()
{
    resultFile.setFileName(QFileDialog::getSaveFileName(this,"Save data file...","/home/nekomata/Dropbox/Experiment/Experiment"));
    resultFile.open(QIODevice::WriteOnly);
    QTextStream out(&resultFile);
    out<<ui->textEditOut2->toPlainText();
    resultFile.close();
}
