#include "portscanthread.h"

void PortScanThread::setPortParas(portParameters portTransferParas){
    m_portTransferParas = portTransferParas;
}

// 这里的run()里面直接写子线程需要实现的逻辑
PortScanThread::PortScanThread()
{

}

void PortScanThread::run()
{
    // QThread::currentThread();                         // 获取当前的线程的地址

    // for 循环遍历 在线的主机
    for(int i=0; i< m_portTransferParas.hostList.size(); i++){
        if(m_portTransferParas.hostList.at(i)->isOn == "主机在线"){
            // 创建端口信息对象 并且初始化
            portInfos *port = new portInfos;
            port->ipAddr = m_portTransferParas.hostList.at(i)->ipAddr;                // 端口所在的主机
            port->ports[0] = 1;
            port->services[0] = "services";
            port->potentialBug[0] = "bug";

            // 设置参数  目标地址， 起始端口， 结束端口， 端口信息对象
            int startPort = stringPortToInt(m_portTransferParas.strStartPort);
            int endPort = stringPortToInt(m_portTransferParas.strEndPort);

            // 开始扫描
            qDebug("%d %d", startPort, endPort);
            portScan(port->ipAddr, startPort, endPort, port);

            // 扫描结束 向主线程 发送信号 接收端口数据
            emit sendPort(port);
        }
    }

}

PortScanThread::~PortScanThread(){
    qDebug() << QThread::currentThreadId();
    qDebug("该线程已经析构");
}
