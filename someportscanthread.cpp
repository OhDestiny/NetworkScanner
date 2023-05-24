#include "someportscanthread.h"

SomePortScanThread::SomePortScanThread()
{

}
void SomePortScanThread::setPortParas(SomePortParameters somePortTransferParas){
    m_somePortParameters = somePortTransferParas;
}



void SomePortScanThread::run()
{
    // QThread::currentThread();                         // 获取当前的线程的地址

    // 两层for循环遍历    for：循环遍历在线的主机  for：遍历端口
    for(int i=0; i<m_somePortParameters.hostList.size(); i++){

        portInfos *port = new portInfos;

        if(m_somePortParameters.hostList.at(i)->isOn == "主机在线"){

            // 初始化端口信息
            port->ipAddr = m_somePortParameters.hostList.at(i)->ipAddr;
            port->ports[0] = 0;
            port->services[0] = "sevice";
            int portPlace = 0;
            for(int j=0; j<m_somePortParameters.portList.size(); j++){
                if(singlePortScan(m_somePortParameters.hostList.at(i)->ipAddr, m_somePortParameters.portList.at(j))){                             // 如果开放，将这个端口写入host的端口数组中，并且根据文档，匹配其可能对应的服务，及其可能存在的漏洞。
                    port->portNum ++;
                    qDebug("%d", m_somePortParameters.portList.at(j));
                    port->ports[portPlace] = m_somePortParameters.portList.at(j);
                    // 根据端口到对应的service文档里 匹配查找
                    port->services[portPlace++] = showservice(QString("%1").arg(m_somePortParameters.portList.at(j)));
                    // 根据端口到对应的漏洞文档 匹配查找
                }
            }
        }
        emit sendPort(port);
    }
}

SomePortScanThread::~SomePortScanThread(){
    qDebug() << QThread::currentThreadId();
    qDebug("该线程已经析构");
}
