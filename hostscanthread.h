#ifndef HOSTSCANTHREAD_H
#define HOSTSCANTHREAD_H

#include <QThread>
#include <tools.h>
#include <QVector>

// 主机扫描线程 需要接收来自主线程的参数对象，然后返回一个主机信息类的数组
// 主线程和子线程的数据传递使用信号槽

class HostScanThread : public QThread
{
    Q_OBJECT
public:
    explicit HostScanThread(QThread *parent = nullptr);
    // 接收参数数据
    void recvParameters(Parameters parameters);

protected:
    void run() override;

signals:
    void sendHost(QVector<HostInfos> hostList);

private:
    Parameters m_parameters;                                   // 接收主线程的参数结构体
};

#endif // HOSTSCANTHREAD_H
