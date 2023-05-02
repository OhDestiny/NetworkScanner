#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<HostInfos>>("QVector<HostInfos>");
    MainWindow w;
    w.show();
    return a.exec();
}
