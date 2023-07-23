#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    try{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    }
    catch(_exception e)
    {
        qDebug()<<"catch exception="<<&e<<Qt::endl;
    }
}
