#ifndef HOSTSCANTHREAD_H
#define HOSTSCANTHREAD_H
#include <QThread>
#include <tools.h>
#include <QVector>
#include <QRunnable>
#include <QObject>


class HostScanThread : public QThread
{
    Q_OBJECT
public:
    explicit HostScanThread(QObject *parent = nullptr);
    // 接收参数数据
    void recvParameters(ipParameters ipTransferParas);

    void run() override;

    ipParameters m_ipTransferParas;                                   // 接收主线程的ip参数结构体
    ~HostScanThread();

signals:
    void sendHost(HostInfos *);
};

#endif // HOSTSCANTHREAD_H
