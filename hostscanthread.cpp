#include "hostscanthread.h"
#include <QElapsedTimer>
#include <mainwindow.h>
#include <QDebug>
#include "ping.h"

HostScanThread::HostScanThread(QThread *parent) : QObject(parent), QRunnable()
{
    setAutoDelete(true);
}

// 接收主线程的参数的槽函数


void HostScanThread::recvParameters(TransferParas transferParas)
{
    m_transferParas = transferParas;
    qDebug() << "目标ip:" << transferParas.desIp;
    qDebug() << "开始port:" << transferParas.startPort;
    qDebug() << "结束port:" << transferParas.endPort;
}

// 接收到消息之后就可以进行子线程的扫描操作 ------> run()


// 这里的run()里面直接写子线程需要实现的逻辑
void HostScanThread::run()
{
    // QThread::currentThread();                         // 获取当前的线程的地址
    // 定义主机类型的vector
    HostInfos host;



    // 返回一个主机   每一次运行完，都会向主线程发送该主机信息
    emit sendHost(host);
}
