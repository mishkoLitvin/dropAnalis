#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <math.h>
#include <QPolygon>
#include <QDebug>

class QInterpolateClass
{
public:
    QInterpolateClass () {}
    QPolygonF inputPolygon;

private:
    int i, j,size;
    double xi[100],yi[100];


public:
    long double sqr(long double z)
    {
        return z*z;
    }

   double doIt(double x)
    {
        double lagrange_pol = 0;
        double basics_pol;

        for (i=0;i<inputPolygon.size();i++)
        {
            xi[i+1] = inputPolygon[i].x();
            yi[i+1] = inputPolygon[i].y();
//            qDebug()<<xi[i+1]<<yi[i+1];
        }
        size=inputPolygon.size();


        for (i = 0; i < size; i++)
        {
            basics_pol = 1;
            for (j = 0; j < size; j++)
            {
                if (j == i) continue;
                basics_pol *= (x - xi[j])/(xi[i] - xi[j]);
            }
            lagrange_pol += basics_pol*yi[i];
        }
        return lagrange_pol;
    }
};

#endif // INTERPOLATION_H
