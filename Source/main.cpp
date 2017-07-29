#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QXmlStreamReader>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int res = a.exec();
    return res;
}
