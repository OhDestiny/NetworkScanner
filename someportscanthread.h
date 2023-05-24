#ifndef SOMEPORTSCANTHREAD_H
#define SOMEPORTSCANTHREAD_H

#include <QThread>
#include <tools.h>
#include <QVector>
#include <QRunnable>
#include <QObject>

class SomePortScanThread : public QThread
{
    Q_OBJECT
public:
    SomePortScanThread();
    void run() override;
    ~SomePortScanThread();
    void setPortParas(SomePortParameters somePortTransferParas);
    SomePortParameters m_somePortParameters;
signals:
    void sendPort(portInfos *);                                    // 发送端口信息的信号
};

#endif // SOMEPORTSCANTHREAD_H
