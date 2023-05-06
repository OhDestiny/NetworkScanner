#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<HostInfos>>("QVector<HostInfos>");                 // 需要注册自己定义的，不然不是有效的标识符，会报错
    MainWindow w;
    w.show();
    return a.exec();
}
