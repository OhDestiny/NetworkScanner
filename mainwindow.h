#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <helpwindow.h>
#include <aboutauthor.h>
#include <aboutmachine.h>
#include <hostscanthread.h>
#include <portscanthread.h>
#include <someportscanthread.h>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<HostScanThread *> hostScanThreadList;                    // 扫描主机的线程数组
    QVector<PortScanThread *> portScanThreadList;                    // 扫描端口的线程数组
    QVector<SomePortScanThread *> somePortScanThreadList;            // 扫描特定端口的线程数组
    QVector<int> port = {1, 7, 19, 21, 22, 23, 25, 31, 42, 53, 67, 69, 79, 80, 99, 102, 109, 110, 113, 119, 135, 143, 161, 443, 456, 513, 548, 569, 666, 993, 1001, 1011};                  // 添加常用的端口
    int indexHost = 0;
    int indexPort = 0;
    int indexSomePort = 0;



private:
    Ui::MainWindow *ui;

    QVector<HostInfos *> hostList;             // 维护一个主机地址的vector 存放在线的主机，为后来的端口检测提供依据。
    QVector<portInfos *> portList;             // 维护一个在线的主机的开放端口
    QVector<portInfos *> somePortList;         // 维护一个主机开放的常用的端口


private slots:
    void on_click_helpWindow();                // 点击帮助文档事件的槽函数
    void on_click_aboutAuthor();               // 点击关于作者的槽函数
    void on_click_aboutMachine();              // 点击关于本机的槽函数
    void on_click_scan_host();                 // 主机扫描的槽函数
    void on_click_scan_port();                 // 端口扫描的槽函数
    void recvHost(HostInfos *host);            // 接收子线程传来的主机信息
    void recvPort(portInfos *port);            // 接收子线程传来的端口信息
    void recvSomePort(portInfos *port);        // 接收子线程传来的特定端口的信息
    void on_click_host_save();                 // 保存主机
    void on_click_port_save();                 // 保存端口
    void on_click_scan_some_port();            // 特定端口的扫描
    void on_click_save_some_port();            // 特定端口的保存

signals:
    void sendHostParas(ipParameters ipTransferParas);                    // 发送主机参数的信号
    void sendPortParas(portParameters portTransferParas);                // 发送端口参数的信号
};
#endif // MAINWINDOW_H
