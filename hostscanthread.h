#ifndef HOSTSCANTHREAD_H
#define HOSTSCANTHREAD_H

#include <QThread>
#include <tools.h>
#include <QVector>
#include <QRunnable>
#include <QObject>

// 这里是扫描主机是否在线和操作系统类型的QRunnable
// 主机扫描线程 需要接收来自主线程的参数对象，然后返回一个主机信息类的数组
// 主线程和子线程的数据传递使用信号槽

class HostScanThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit HostScanThread(QThread *parent = nullptr);
    // 接收参数数据
    void recvParameters(TransferParas transferParas);

    void run() override;

    TransferParas m_transferParas;                                   // 接收主线程的参数结构体
    ~HostScanThread();


signals:
    void sendHost(HostInfos *);


};

#endif // HOSTSCANTHREAD_H
