#ifndef ABOUTMACHINE_H
#define ABOUTMACHINE_H

#include <QDialog>
#include <QTableWidget>
#include <QSysInfo>
#include <QDebug>
#include <QHeaderView>
#include <QtNetwork>

namespace Ui {
class AboutMachine;
}

class AboutMachine : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMachine(QWidget *parent = nullptr);
    ~AboutMachine();

    // 获取本机IP地址
    QString getIpAddress();

    // 获取本机的mac地址
    QString getMacAddress();

    // 获取并且在tableWidget上展示本主机的相关信息
    void getAndShowHostInfo();
private:
    Ui::AboutMachine *ui;
};

#endif // ABOUTMACHINE_H
