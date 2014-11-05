#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int i;
    if(argc>1)
    {
        for(i=1;i<argc;i++)
            w.fileList<<argv[i];
        w.openFileList();
    }
    w.show();

    return a.exec();
}
