#ifndef PORTSCANTHREAD_H
#define PORTSCANTHREAD_H
#include <QThread>
#include <tools.h>
#include <QVector>
#include <QRunnable>
#include <QObject>

class PortScanThread : public QThread
{
    Q_OBJECT
public:
    PortScanThread();                                              // 构造函数
    void run() override;                                           // 子线程需要运行的逻辑
    ~PortScanThread();                                             // 析构函数
    void setPortParas(portParameters portTransferParas);           // 直接在主线程获得端口参数之后，直接调用然后设置端口参数
    portParameters m_portTransferParas;
signals:
    void sendPort(portInfos *);                                    // 发送端口信息的信号
};



#endif // PORTSCANTHREAD_H
