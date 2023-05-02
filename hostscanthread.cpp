#include "hostscanthread.h"
#include <QElapsedTimer>
#include <mainwindow.h>
#include <QDebug>

HostScanThread::HostScanThread(QThread *parent) : QThread(parent)
{

}

// 接收主线程的参数的槽函数
void HostScanThread::recvParameters(Parameters parameters)
{
    m_parameters = parameters;
    qDebug() << "结束ip:" << m_parameters.strEndIp;
    qDebug() << "结束port:" << m_parameters.strEndPort;
    qDebug() << "开始ip:" << m_parameters.strStartIp;
    qDebug() << "开始port:" << m_parameters.strStartPort;
}

// 这里的run()里面直接写子线程需要实现的逻辑
void HostScanThread::run()
{
    // QThread::currentThread();                         // 获取当前的线程的地址
    // 定义主机类型的vector
    QVector<HostInfos> hostList;

    // for循环 创建host 并且赋值，然后append到list  现在先用固定的测试效果
    HostInfos host;
    host.ipAddr = "1.1.1.1";
    host.isOn = "online";
    host.osInfo = "windows";
    host.ports[0] = "1";
    host.potentialBug[0] = "blue";
    host.services[0] = "hhh";


    // 将这个host加到hostList后面
    hostList.append(host);

    // 返回一个主机类型的vector
    emit sendHost(hostList);
}
