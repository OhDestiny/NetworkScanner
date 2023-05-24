#include "hostscanthread.h"

HostScanThread::HostScanThread(QObject *parent)
{

}

void HostScanThread::recvParameters(ipParameters ipTransferParas){
    m_ipTransferParas = ipTransferParas;
}

// 这里的run()里面直接写子线程需要实现的逻辑
void HostScanThread::run()
{
    // QThread::currentThread();                         // 获取当前的线程的地址
    // 定义主机类型的vector
    HostInfos *host = new HostInfos;
    host->isOn = "NULL";
    host->osInfo = "NULL";

    // for循环进行 扫描主机 初步打算开启三个线程 所以i++
    for(int startIpFor=ipStrToNum(m_ipTransferParas.strStartIp); startIpFor<=ipStrToNum(m_ipTransferParas.strEndIp); startIpFor++){
        // 定义主机类型的vector
        HostInfos *host = new HostInfos;
        host->isOn = "NULL";
        host->osInfo = "NULL";

        // 将ip地址转换为字符串
        QString desIp = ipNumToStr(startIpFor);

        // 测试是否将ip传入
        host->ipAddr = desIp;
        QByteArray tempIp = desIp.toLatin1();
        qDebug("开始扫描ip %s",tempIp.data());

        // 直接扫描，扫描完之后将host传入主线程   只要传入目标主机ip，返回目标主机的相关信息即可
        scanning(desIp, host);

        // 扫描结束，返回主机信息给主线程 呈现在界面上

        qDebug() << host->isOn.toStdString().data();
        qDebug() << host->osInfo.toStdString().data();
        emit sendHost(host);
        qDebug("已发送给主线程....");
    }
}

HostScanThread::~HostScanThread(){
    qDebug() << QThread::currentThreadId();
    qDebug("该线程已经析构");
}

