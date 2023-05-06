#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <helpwindow.h>
#include <aboutauthor.h>
#include <aboutmachine.h>
#include <tools.h>
#include <hostscanthread.h>
#include <QThreadPool>                            // 线程池
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    HostScanThread* hostScanThread;
    QVector<HostInfos> hostList;

signals:
    void startHostScan(TransferParas transferParas);
private slots:
    void on_click_helpWindow();                // 点击帮助文档事件的槽函数
    void on_click_aboutAuthor();               // 点击关于作者的槽函数
    void on_click_aboutMachine();              // 点击关于本机的槽函数
    void on_click_scan();                      // 点击扫描的槽函数
    void on_click_cancel();                    // 点击取消的槽函数
    void on_click_save();                      // 点击保存的槽函数
    void recvHost(HostInfos host);                           // 接收子线程传来的主机信息
};
#endif // MAINWINDOW_H
