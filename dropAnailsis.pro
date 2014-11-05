#-------------------------------------------------
#
# Project created by QtCreator 2014-06-22T00:05:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphicTest
TEMPLATE = app


#QWT proFile addings
#LIBS += -L//usr/lib -lqwt
LIBS += -L//home/mishko/Qt5.3.2/Tools/qwt-6.1.0/lib -lqwt
INCLUDEPATH += /home/mishko/Qt5.3.2/Tools/qwt-6.1.0/src
DEPENDPATH += /home/mishko/Qt5.3.2/Tools/qwt-6.1.0/lib
#QWT proFile addings end

SOURCES += main.cpp\
        mainwindow.cpp \
    graphicplot.cpp

HEADERS  += mainwindow.h \
    interpolation.h \
    graphicplot.h

FORMS    += mainwindow.ui \
    graphicplot.ui

RESOURCES += \
    Img.qrc
