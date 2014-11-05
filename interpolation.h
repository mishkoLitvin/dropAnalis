#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <math.h>
#include <QPolygon>
#include <QDebug>

class QInterpolateClass
{
public:
    QInterpolateClass () {}
    QPolygonF inputPolygon, outputPolygon;

private:
    int r, i, j, k, l, n, count;
    double max, c, m, s,eps;
    double a[100][100];
    double x[1000], b[1000];
    double ro[1000], xi[1000], yi[1000], fi[1000];


public:
    long double sqr(long double z)
    {
        return z*z;
    }

    void doIt(int iterations)
    {
        for (i=0;i<inputPolygon.size();i++)
        {
            xi[i+1] = inputPolygon[i].x();
            yi[i+1] = inputPolygon[i].y();
        }
        count=inputPolygon.size()-1;
        eps=0.5;
        for (i=1; i<=count; i++)
        {
            ro[i]=0.0;
            for (j=1; j<=count; j++)
                ro[i]=ro[i]+sqr(eps);
            ro[i]=1.0/eps*sqrt(ro[i]/(n+1));
        }

        n = iterations;
        for (k=1; k<= n;k++)
        {
            for (l=1;l<=n;l++)
            {
                a[k][l]=0.0;
                b[k]=0.0;
                for (i=1;i<= count;i++)
                    a[k][l]=a[k][l]+ro[i]*pow(xi[i], k+l);
                for (i=1;i<= count;i++)
                    b[k]=b[k]+ro[i]*yi[i]*pow(xi[i], k);
            }
        }

        for (k =1; k<=n;k++)
        {
            max = fabs(a[k][k]);
            r = k ;
            for (i=k+1;i<=n;i++)
                if (fabs(a[i][k])>max)
                {
                    max = fabs ( a[i][k] ) ;
                    r = i ;
                }
            for (j =1;j<=n;j++)
            {
                c = a[k][j];
                a[k][j] = a[r][j] ;
                a[r][j] = c ;
            }
            c = b[k];
            b[k]= b[r] ;
            b[r]= c ;
            for (i = k+1;i<n;i++)
            {
                m = a[i][k]/a[k][k];
                for (j = k; j<=n;j++)
                    a[i][j]=a[i][j]-m*a[k][j] ;
                b [i] = b[i]-m*b[k] ;
            }
        }

        if(a[n][n]!=0)
        {
            for (i = n; i>=1;i--)
            {
                s= 0.0 ;
                for (j = i+1;j<=n;j++)
                {
                    s = s+a[i][j]*x[j];
                }
                x[i]=(b[i]-s)/a[i][i];
            }
            outputPolygon.clear();
            for(i=1;i<=n;i++)
                outputPolygon<<QPointF(i,x[i]);
        }
    }
};

#endif // INTERPOLATION_H
