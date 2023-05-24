#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <helpwindow.h>
#include <aboutauthor.h>
#include <tools.h>
#include <hostscanthread.h>
#include <portscanthread.h>
#include <QFileDialog>


////子线程在主函数的构造函数中实例化，connect，槽函数接收消息正常。
////子线程类在一个函数中实例化，connect，槽函数收不到消息。

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 先在QVector里面存主机扫描线程
    for(int i = 0; i<100; i++){
        HostScanThread *hostScanThread = new HostScanThread();
        hostScanThreadList.append(hostScanThread);
    }

    // 在QVector里面存放端口扫描线程
    for(int i = 0; i<100; i++){
        PortScanThread *portScanThread = new PortScanThread();
        portScanThreadList.append(portScanThread);
    }

    // 在QVector里面存放特定端口扫描线程
    for(int i = 0;i<100;i++){
        SomePortScanThread *somePortScanThread = new SomePortScanThread();
        somePortScanThreadList.append(somePortScanThread);
    }

    // 设置应用的名字
    setWindowTitle("network scanner");

    // 设置菜单的图标
    ui->actionhelp->setIcon(QIcon(":/n/res/helpWindow.png"));
    ui->actionabout->setIcon(QIcon(":/n/res/aboutAuthor.png"));
    ui->actionaboutMachine->setIcon(QIcon(":/n/res/aboutMachine.png"));

    // 设置tabWidget

    // 设置页面名字
    ui->tabWidget->setTabText(0,"主机在线以及操作系统识别");
    ui->tabWidget->setTabText(1,"主机端口以及服务识别");
    ui->tabWidget->setTabText(2,"常用端口扫描以及服务识别");

    // 设置widget边框颜色
    ui->widgetIpSet->setStyleSheet(QString::fromUtf8("#widget_2{border:3px solid black}"));
    ui->widgetPortSet->setStyleSheet(QString::fromUtf8("#widget_2{border:3px solid black}"));

    // 设置textEdit的清除按钮
    ui->lineEditStartIp->setClearButtonEnabled(true);
    ui->lineEditEndIp->setClearButtonEnabled(true);
    ui->lineEditStartPort->setClearButtonEnabled(true);
    ui->lineEditEndPort->setClearButtonEnabled(true);


    // 帮助文档链接
    connect(ui->actionhelp,SIGNAL(triggered()), this, SLOT(on_click_helpWindow()));

    // 关于作者
    connect(ui->actionabout,SIGNAL(triggered()), this, SLOT(on_click_aboutAuthor()));

    // 关于本机
    connect(ui->actionaboutMachine,SIGNAL(triggered()), this, SLOT(on_click_aboutMachine()));


    // -------------------------------------------------------------------------------------------------------------------------------------------------
    // 绑定主线程向主机扫描子线程传递数据的信号和槽函数
    for(int threadNum = 0;threadNum < 20; threadNum++){
        connect(this, &MainWindow::sendHostParas, this->hostScanThreadList.at(threadNum), &HostScanThread::recvParameters);
        connect(this->hostScanThreadList.at(threadNum), SIGNAL(sendHost(HostInfos *)), this, SLOT(recvHost(HostInfos *)));
        connect(this->portScanThreadList.at(threadNum), SIGNAL(sendPort(portInfos *)), this, SLOT(recvPort(portInfos *)));
        connect(this->somePortScanThreadList.at(threadNum), SIGNAL(sendPort(portInfos *)), this, SLOT(recvSomePort(portInfos *)));
    }


    // 绑定主机扫描按钮与扫描事件
    connect(ui->pushButtonHostScan, SIGNAL(clicked()), this, SLOT(on_click_scan_host()));
    // 绑定主机保存按钮点击与保存事件
    connect(ui->pushButtonHostSave, SIGNAL(clicked()), this, SLOT(on_click_host_save()));

    // --------------------------------------------------------------------------------------------------------------------------------------------------
    //绑定主线程向端口扫描子线程传递数据的信号和槽函数
    connect(ui->pushButtonPortScan, SIGNAL(clicked()), this, SLOT(on_click_scan_port()));
    // 绑定端口保存按钮点击与保存事件
    connect(ui->pushButtonPortSave, SIGNAL(clicked()), this, SLOT(on_click_port_save()));


    // --------------------------------------------------------------------------------------------------------------------------------------------------
    // 绑定指定端口的扫描按钮和事件
    connect(ui->pushButtonSomePortScan, SIGNAL(clicked()), this, SLOT(on_click_scan_some_port()));
    // 绑定指定端口的保存按钮和事件
    connect(ui->pushButtonSomePortSave, SIGNAL(clicked()), this, SLOT(on_click_save_some_port()));
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

// 主机扫描槽函数
void MainWindow::on_click_scan_host(){
    // 清除QWightTree的数据
    removeTreeWidget(ui->treeWidgetHost);
    // 清除hostList
    hostList.clear();

    QString strStartIp;
    QString strEndIp;

    bool isParasOk = true;                                // 判断所有的参数是否合法

    // 获得起始ip 结束ip 起始port 结束port
    if(ui->lineEditStartIp->text() == NULL){
        strStartIp = "";
    }
    else{
        strStartIp = ui->lineEditStartIp->text();                               // 获取开始的ip地址
    }

    if(ui->lineEditEndIp->text() == NULL){
        strEndIp = "";
    }
    else{
        strEndIp = ui->lineEditEndIp->text();                                   // 获取结束的ip地址
    }

    // 判断起始ip是否合法
    // 定义一个ipFlag 判断是否需要判断范围
    int ipFlag = 0;
    if(!checkIpForm(strStartIp)){
        ipFlag = 1;
        isParasOk = false;
        ui->textBrowserLog->append(QString("起始ip格式错误，正确格式：xxx.xxx.xxx.xxx"));
    }

    // 判断结束ip是否合法
    if(!checkIpForm(strEndIp)){
        ipFlag = 1;
        isParasOk = false;
        ui->textBrowserLog->append(QString("结束ip格式错误，正确格式: xxx.xxx.xxx.xxx"));
    }

    // 判断ip的范围是否合法
    if(ipFlag == 0){
        if(!checkIpRange(strStartIp, strEndIp)){
            isParasOk = false;
            ui->textBrowserLog->append(QString("结束的ip地址需要比起始的ip地址大! 呆比!"));
        }
    }

// 如果参数都是合法的，那么可以开始进行扫描，否则重新设定参数再次扫描 ----> 设定flag 判断是否所有的参数都已经合法

    if(isParasOk){
        ipParameters ipTransferParas;
        ipTransferParas.strStartIp = strStartIp;
        ipTransferParas.strEndIp = strEndIp;
        ui->textBrowserLog->append("主机参数设置成功，开始扫描....");

        qDebug("接收参数成功，开始扫描......");

    // 单纯的传递参数给子线程 传递相同的参数给不同的子线程，然后在不同的子线程上分段的对不同的主机进行扫描，
    // 创建子线程，最后返回结果。这里直接启动子线程即可
        emit sendHostParas(ipTransferParas);
        hostScanThreadList.at(indexHost++)->start();
    }
    else{
        ui->textBrowserLog->append(QString("参数设定存在不合理情况，请检查后，再次扫描......"));
    }
}

// 端口扫描槽函数
void MainWindow::on_click_scan_port(){
    // 删除树的数据
    removeTreeWidget(ui->treeWidgetPort);

    // 删除
    portList.clear();

    // 进行端口扫描的时候 先判断是否进行过主机扫描 也就是判断主线程的的在线主机列表是否是空的
    int hostOn = hostList.size();
    // 如果在线的主机数量为0的话 ---> 1. 真的没有主机在线   2. 没有扫描主机
    if(hostOn == 0){
        // 打印日志 请先扫描主机
        ui->textBrowserLog->append(QString("还未进行主机扫描或者扫描的主机都不在线！"));
    }
    else{
        QString strStartPort;
        QString strEndPort;

        if(ui->lineEditStartPort->text() == NULL){
            strStartPort = "";
        }else{
            strStartPort = ui->lineEditStartPort->text();                           // 获取开始的port
        }

        if(ui->lineEditEndPort->text() == NULL){
            strEndPort = "";
        }else{
            strEndPort = ui->lineEditEndPort->text();                               // 获取结束的port
        }

        // 判断起始port是否合法
        // 定义一个portFlag 判断端口是否合法
        int portFlag = 0;
        bool isParasOk = true;
        if(!checkPort(strStartPort)){
            portFlag = 1;
            isParasOk = false;
            ui->textBrowserLog->append(QString("起始端口不正确奥，正确范围是：1-65535"));
        }
        // 判断结束port是否合法
        if(!checkPort(strEndPort)){
            portFlag = 1;
            isParasOk = false;
            ui->textBrowserLog->append(QString("结束端口不正确奥，正确的范围是:1-65535"));
        }

        // 判断port范围是否合法
        if(portFlag == 0){
            if(!checkPortRange(strStartPort, strEndPort)){
                isParasOk = false;
                ui->textBrowserLog->append(QString("结束的端口需要比起始的端口大啊！ 呆比！"));
            }
        }
        // 如果设置的参数开启线程 进行多线程扫描
        if(isParasOk){
            portParameters portTransferParas;
            portTransferParas.strStartPort = strStartPort;
            portTransferParas.strEndPort = strEndPort;
            portTransferParas.hostList = hostList;
            ui->textBrowserLog->append("端口参数设置成功，开始扫描....");

            // 在子线程上设置参数
            portScanThreadList.at(indexPort)->setPortParas(portTransferParas);

            // 启动在子线程
            portScanThreadList.at(indexPort)->start();
        }else{
            ui->textBrowserLog->append(QString("端口参数设定存在不合理情况，请检查后，再次扫描......"));
        }
    }
}

//// ----------------------------------------------------------------------------------
//// 注意：父类和父对象是不一样的
//// 父类：是继承的类
//// 父对象：而父对象与自身没有继承关系，qt本身一个项目会维护一个对象树，也就是说，在销毁回收工程的时候，如果父对象已经被销毁，那么子对象也会被销毁，防止内存的泄露。
//// ----------------------------------------------------------------------------------

//// tablewidget->setItem(0,0,new QTableWidgetItem("架构信息"));
//// tablewidget->setItem(0,1,new QTableWidgetItem(QSysInfo::buildAbi()));

// 接收主机扫描的数据
void MainWindow::recvHost(HostInfos *host){

    // 打印扫描这个主机的线程
    // 接收到主机消息 for循环展示在tabWidget上

    qDebug("接收数据主机.....");

    // 引入指纹库，根据端口开放情况 --> 匹配对应的服务，以及服务存在的漏洞
    hostList.append(host);

    // 判断 如果主机不在线，如果不在线，后续的信息不需要呈现在界面上

    // 是否在线
    if(host->isOn == "主机在线"){
        QTreeWidgetItem *itemHost = new QTreeWidgetItem(ui->treeWidgetHost);
        itemHost->setText(0,host->ipAddr);
        itemHost->setText(1,host->isOn);
        itemHost->setText(2,host->osInfo);
        itemHost->setSelected(true);
    }
    else{
        QTreeWidgetItem *itemHost = new QTreeWidgetItem(ui->treeWidgetHost);
        itemHost->setText(0,host->ipAddr);
        itemHost->setText(1,host->isOn);
        itemHost->setText(2,host->osInfo);
        itemHost->setSelected(true);
    }
}

// 接收端口扫描的数据
void MainWindow::recvPort(portInfos *port){


    // 使用主线程的一个vector容器维护
    portList.append(port);

    qDebug("接收数据主机.....");

    // 展示数据
    for(int i=0;i<port->portNum;i++){
        QTreeWidgetItem *itemPorts = new QTreeWidgetItem(ui->treeWidgetPort);
        itemPorts->setText(0,port->ipAddr);
        itemPorts->setText(1,QString("%1").arg(port->ports[i]));                               // 端口
        itemPorts->setText(2,QString("%1").arg(port->services[i]));                            // 服务
        itemPorts->setSelected(true);
    }
}

// 接收特定端口的信息
void MainWindow::recvSomePort(portInfos *port){
    somePortList.append(port);

    // 展示数据
    for(int i=0;i<port->portNum;i++){
        QTreeWidgetItem *itemPorts = new QTreeWidgetItem(ui->treeWidgetSomePort);
        itemPorts->setText(0,port->ipAddr);
        itemPorts->setText(1,QString("%1").arg(port->ports[i]));                               // 端口
        itemPorts->setText(2,QString("%1").arg(port->services[i]));                            // 服务
        itemPorts->setSelected(true);
    }
}

// 保存主机数据
void MainWindow::on_click_host_save(){
    // 打开文件对话框
    QString fileNameHost;
    fileNameHost = QFileDialog::getOpenFileName(this, "打开文件", "C:/Users/OhDestiny/Desktop");

    // 将主线程维护的hostList数据存到磁盘里
    QFile file(fileNameHost);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // 循环输入hostList的数据
    for(int i=0; i<hostList.size();i++){
        out << hostList.at(i)->ipAddr  << "    " <<hostList.at(i)->isOn << "    " << hostList.at(i)->osInfo << "\n";
    }
}

// 保存主机端口数据
void MainWindow::on_click_port_save(){
    // 打开文件对话框
    QString fileNamePort;
    fileNamePort = QFileDialog::getOpenFileName(this, "打开文件", "C:/Users/OhDestiny/Desktop");

    // 将主线程维护的hostList数据存到磁盘里
    QFile file(fileNamePort);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // 循环输入hostList的数据
    for(int i=0; i<portList.size();i++){
        for(int j=0;j<portList.at(i)->portNum;j++){
            out <<  portList.at(i)->ipAddr << "      " << portList.at(i)->ports[j] << "      " << portList.at(i)->services[j] << "\n";
        }
    }
}


// 特定端口的扫描
void MainWindow::on_click_scan_some_port(){
    // 删除树的数据
    removeTreeWidget(ui->treeWidgetSomePort);

    // 删除
    somePortList.clear();

    // 进行端口扫描的时候 先判断是否进行过主机扫描 也就是判断主线程的的在线主机列表是否是空的
    int hostOn = hostList.size();
    // 如果在线的主机数量为0的话 ---> 1. 真的没有主机在线   2. 没有扫描主机
    if(hostOn == 0){
        // 打印日志 请先扫描主机
        ui->textBrowserLog->append(QString("还未进行主机扫描或者扫描的主机都不在线！"));
    }else{
        // 向子线程传递参数 直接在子线程上设置一个函数来设置参数
        SomePortParameters somePortTransferParas;
        somePortTransferParas.hostList = hostList;
        somePortTransferParas.portList = port;
        ui->textBrowserLog->append("特定端口参数设置成功，开始扫描....");

        // 在子线程上设置参数
        somePortScanThreadList.at(indexSomePort)->setPortParas(somePortTransferParas);

        // 启动在子线程
        somePortScanThreadList.at(indexSomePort)->start();
    }
}

// 特定端口的保存
void MainWindow::on_click_save_some_port(){
    // 打开文件对话框
    QString fileNameSomePort;
    fileNameSomePort = QFileDialog::getOpenFileName(this, "打开文件", "C:/Users/OhDestiny/Desktop");

    // 将主线程维护的hostList数据存到磁盘里
    QFile file(fileNameSomePort);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // 循环输入hostList的数据
    for(int i=0; i<somePortList.size();i++){
        for(int j=0;j<somePortList.at(i)->portNum;j++){
            out <<  somePortList.at(i)->ipAddr << "      " << somePortList.at(i)->ports[j] << "      " << somePortList.at(i)->services[j] << "\n";
        }
    }
}

// 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

