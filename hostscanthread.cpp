#include "hostscanthread.h"
#include <QElapsedTimer>
#include <mainwindow.h>
#include <QDebug>
#include "ping.h"

HostScanThread::HostScanThread(QThread *parent) : QObject(parent), QRunnable()
{
    // setAutoDelete(true);
}

// 接收主线程的参数的槽函数


void HostScanThread::recvParameters(TransferParas transferParas)
{
    m_transferParas = transferParas;
//    qDebug() << "目标ip:" << transferParas.desIp;
//    qDebug() << "开始port:" << transferParas.startPort;
//    qDebug() << "结束port:" << transferParas.endPort;
    // qDebug() << QThread::currentThreadId();
}

// 接收到消息之后就可以进行子线程的扫描操作 ------> run()


// 这里的run()里面直接写子线程需要实现的逻辑
void HostScanThread::run()
{
    // QThread::currentThread();                         // 获取当前的线程的地址
    // 定义主机类型的vector
    HostInfos *host = new HostInfos;
    host->isOn = "NULL";
    host->osInfo = "NULL";
    host->ipAddr = m_transferParas.desIp;
    host->ports[0] = 1;
    host->services[0] = "blue";
    host->potentialBug[0] = "hhh";

    // 调用扫描函数
    qDebug("-------------------------------------");
    qDebug() << "目标ip:" << m_transferParas.desIp;
    qDebug() << QThread::currentThreadId();
    qDebug("-------------------------------------");

    qDebug("runing....");
    scanning(m_transferParas, host);

    qDebug("扫描结束...");

    // 打印host
    qDebug() << host->isOn.toStdString().data();
    qDebug() << host->osInfo.toStdString().data();
    // 返回一个主机   每一次运行完，都会向主线程发送该主机信息

    emit sendHost(host);

    qDebug("已发送给主线程....");
}

HostScanThread::~HostScanThread(){
    qDebug() << QThread::currentThreadId();
    qDebug("该线程已经析构");
}
