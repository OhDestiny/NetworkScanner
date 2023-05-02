#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <helpwindow.h>
#include <aboutauthor.h>
#include <tools.h>
#include <QFileDialog>
#include <hostscanthread.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置应用的名字
    setWindowTitle("network scanner");

    // 设置菜单的图标
    ui->actionhelp->setIcon(QIcon(":/n/res/helpWindow.png"));
    ui->actionabout->setIcon(QIcon(":/n/res/aboutAuthor.png"));
    ui->actionaboutMachine->setIcon(QIcon(":/n/res/aboutMachine.png"));

    // 设置tabWidget

    // 设置页面名字
    ui->tabWidget->setTabText(0,"主机");
    ui->tabWidget->setTabText(1,"操作系统");
    ui->tabWidget->setTabText(2,"端口");
    ui->tabWidget->setTabText(3,"服务");
    ui->tabWidget->setTabText(4,"潜在漏洞");

    // 设置页面的提示信息
    ui->tabWidget->setTabToolTip(0,"主机在线情况");
    ui->tabWidget->setTabToolTip(1,"主机操作系统");
    ui->tabWidget->setTabToolTip(2,"主机端口开放情况");
    ui->tabWidget->setTabToolTip(3,"主机端口开启的服务");
    ui->tabWidget->setTabToolTip(4,"主机潜在的漏洞威胁");

    // 设置页面激活
    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->tabWidget->setTabEnabled(3, true);
    ui->tabWidget->setTabEnabled(4, true);

    // 设置页面关闭按钮。
    // ui->tabWidget->setTabsClosable(true);

    // 设置页面的拖拽移动
    ui->tabWidget->setMovable(true);

    // 设置textEdit的清除按钮
    ui->startIpEditLine->setClearButtonEnabled(true);
    ui->endIpEditLine->setClearButtonEnabled(true);
    ui->startPortEditLine->setClearButtonEnabled(true);
    ui->endPortEditLine->setClearButtonEnabled(true);

    // 设置状态栏
    QLabel *label = new QLabel("created by llh", this);
    statusBar()->addPermanentWidget(label);

    // 设置widget边框颜色
    ui->widget_2->setStyleSheet(QString::fromUtf8("#widget_2{border:3px solid black}"));


    // 帮助文档链接
    connect(ui->actionhelp,SIGNAL(triggered()), this, SLOT(on_click_helpWindow()));

    // 关于作者
    connect(ui->actionabout,SIGNAL(triggered()), this, SLOT(on_click_aboutAuthor()));

    // 关于本机
    connect(ui->actionaboutMachine,SIGNAL(triggered()), this, SLOT(on_click_aboutMachine()));
    
    // 绑定点击扫描，触发槽函数
    // 创建线程进行主机在线扫描，以及根据ttl识别主机的操作系统，点击扫描后，直接开始线程
    // 需要给线程的参数，startIp， endIp， startPort， endPort
    hostScanThread = new HostScanThread;
    connect(ui->pushButtonScan, SIGNAL(clicked()), this, SLOT(on_click_scan()));

    // 绑定主线程向主机扫描子线程传递数据的信号和槽函数
    connect(this, &MainWindow::startHostScan, hostScanThread, &HostScanThread::recvParameters);

    connect(hostScanThread, &HostScanThread::sendHost, this, &MainWindow::recvHost);

    // 绑定点击取消，触发取消的槽函数
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(on_click_cancel()));

    // 绑定点击保存，触发保存的槽函数
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(on_click_save()));
}

// 帮助文档槽函数
void MainWindow::on_click_helpWindow(){
    helpWindow *helpwind = new helpWindow();
    helpwind->show();
}

// 关于作者槽函数
void MainWindow::on_click_aboutAuthor(){
    AboutAuthor *aboutAuthor = new AboutAuthor();
    aboutAuthor->show();
}

// 关于本机槽函数
void MainWindow::on_click_aboutMachine(){
    AboutMachine *aboutMachine = new AboutMachine();
    aboutMachine->getAndShowHostInfo();
    aboutMachine->show();
}

// 扫描槽函数
void MainWindow::on_click_scan(){
    ui->textBrowserLog->append("开始扫描....");

    QString strStartIp;
    QString strEndIp;
    QString strStartPort;
    QString strEndPort;

    // 获得起始ip 结束ip 起始port 结束port
    if(ui->startIpEditLine->text() == NULL){
        strStartIp = "";
    }
    else{
        strStartIp = ui->startIpEditLine->text();                               // 获取开始的ip地址
    }

    if(ui->endIpEditLine->text() == NULL){
        strEndIp = "";
    }
    else{
        strEndIp = ui->endIpEditLine->text();                                   // 获取结束的ip地址
    }

    if(ui->startPortEditLine->text() == NULL){
        strStartIp = "";
    }else{
        strStartPort = ui->startPortEditLine->text();                           // 获取开始的port
    }

    if(ui->startPortEditLine->text() == NULL){
        strStartIp = "";
    }else{
        strEndPort = ui->endPortEditLine->text();                               // 获取结束的port
    }

    // 判断起始ip是否合法
    // 定义一个ipFlag 判断是否需要判断范围
    int ipFlag = 0;
    if(!checkIpForm(strStartIp)){
        ipFlag = 1;
        ui->textBrowserLog->append(QString("起始ip格式错误，正确格式：xxx.xxx.xxx.xxx"));
    }

    // 判断结束ip是否合法
    if(!checkIpForm(strEndIp)){
        ipFlag = 1;
        ui->textBrowserLog->append(QString("结束ip格式错误，正确格式: xxx.xxx.xxx.xxx"));
    }

    // 判断ip的范围是否合法
    if(ipFlag == 0){
        if(!checkIpRange(strStartIp, strEndIp)){
            ui->textBrowserLog->append(QString("结束的ip地址需要比起始的ip地址大! 呆比!"));
        }
    }


    // 判断起始port是否合法
    // 定义一个portFlag 判断端口是否合法
    int portFlag = 0;
    if(!checkPort(strStartPort)){
        portFlag = 1;
        ui->textBrowserLog->append(QString("起始端口不正确奥，正确范围是：1-65535"));
    }

    // 判断结束port是否合法
    if(!checkPort(strEndPort)){
        portFlag = 1;
        ui->textBrowserLog->append(QString("结束端口不正确奥，正确的范围是:1-65535"));
    }

    // 判断port范围是否合法
    if(portFlag == 0){
        if(!checkPortRange(strStartPort, strEndPort)){
            ui->textBrowserLog->append(QString("结束的端口需要比起始的端口大啊！ 呆比！"));
        }
    }

    Parameters parameters;
    parameters.strStartIp = strStartIp;
    parameters.strEndIp = strEndIp;
    parameters.strStartPort = strStartPort;
    parameters.strEndPort = strEndPort;
    parameters.threadNum = "100";

    // 开启子线程
    // HostScanThread* hostScanThread = new HostScanThread;
    emit startHostScan(parameters);
    hostScanThread->start();
}



// ----------------------------------------------------------------------------------
// 注意：父类和父对象是不一样的
// 父类：是继承的类
// 父对象：而父对象与自身没有继承关系，qt本身一个项目会维护一个对象树，也就是说，在销毁回收工程的时候，如果父对象已经被销毁，那么子对象也会被销毁，防止内存的泄露。
// ----------------------------------------------------------------------------------

// tablewidget->setItem(0,0,new QTableWidgetItem("架构信息"));
// tablewidget->setItem(0,1,new QTableWidgetItem(QSysInfo::buildAbi()));


void MainWindow::recvHost(QVector<HostInfos> hostList){
    // 接收到主机消息 for循环展示在tabWidget上
    qDebug() << "接收数据";
    for(int i=0; i<hostList.size(); i++){
        HostInfos host;
        host = hostList.at(i);

        // 是否在线
        QTreeWidgetItem *itemIsOn = new QTreeWidgetItem(ui->treeWidgetIsOnline);
        itemIsOn->setText(0,host.ipAddr);
        itemIsOn->setText(1,host.isOn);

        // 操作系统
        QTreeWidgetItem *itemOs = new QTreeWidgetItem(ui->treeWidgetOs);
        itemOs->setText(0,host.ipAddr);
        itemOs->setText(1,host.osInfo);

        // 端口
        QTreeWidgetItem *itemPorts = new QTreeWidgetItem(ui->treeWidgetPorts);
        itemPorts->setText(0,host.ipAddr);
        itemPorts->setText(1,host.ports[0]);

        // 服务
        QTreeWidgetItem *itemServices = new QTreeWidgetItem(ui->treeWidgetServices);
        itemServices->setText(0,host.ipAddr);
        itemServices->setText(1,host.services[0]);

        // 漏洞
        QTreeWidgetItem *itemBugs = new QTreeWidgetItem(ui->treeWidgetBugs);
        itemBugs->setText(0,host.ipAddr);
        itemBugs->setText(1,host.potentialBug[0]);
    }
}




// 取消槽函数
void MainWindow::on_click_cancel(){
    // 将所有的输入的数据都清除
    ui->startIpEditLine->clear();
    ui->endIpEditLine->clear();
    ui->startPortEditLine->clear();
    ui->endPortEditLine->clear();

}

// 保存槽函数
void MainWindow::on_click_save(){
    // 打开文件对话框
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, "打开文件", "C:/Users/OhDestiny/Desktop");

    // 获取到文件需要存储的地址之后，就可以将扫描到的信息存储到文件中。 以下实现保存代码
    ui->textBrowserLog->append(QString("已将主机信息存储到--->" + fileName));
}

// 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

