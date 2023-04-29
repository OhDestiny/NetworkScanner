#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <helpwindow.h>
#include <aboutauthor.h>

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


MainWindow::~MainWindow()
{
    delete ui;
}

