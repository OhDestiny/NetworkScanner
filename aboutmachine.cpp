#include "aboutmachine.h"
#include "ui_aboutmachine.h"
#include <QStringList>
#include "tools.h"

AboutMachine::AboutMachine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMachine)
{
    ui->setupUi(this);
    setWindowTitle("关于本机");
}

// 获取本主机ip地址
QString AboutMachine::getIpAddress()
{
  QString localIPAddress = "";
  QList<QHostAddress> listAddress = QNetworkInterface::allAddresses();
  for(int j = 0; j < listAddress.size(); j++){
      if(!listAddress.at(j).isNull()
          && listAddress.at(j).protocol() == QAbstractSocket::IPv4Protocol
          && listAddress.at(j) != QHostAddress::LocalHost){
              localIPAddress = listAddress.at(j).toString();
              return localIPAddress;
      }
  }
  return localIPAddress;
}

// 获取本主机的mac地址
QString AboutMachine::getMacAddress()
{
    QList<QNetworkInterface> NetList;//网卡链表
    int NetCount = 0;//网卡个数
    int Neti=0;
    QNetworkInterface thisNet;//所要使用的网卡

    NetList = QNetworkInterface::allInterfaces();//获取所有网卡信息
    NetCount = NetList.count();//统计网卡个数

    for(Neti = 0; Neti < NetCount; Neti++){//遍历所有网卡
        if( NetList[Neti].isValid() ){//判断该网卡是否是合法
            thisNet = NetList[Neti];//将该网卡置为当前网卡
            break;
        }
    }
    return(thisNet.hardwareAddress());//获取该网卡的MAC
}


// 获取并且展示本主机的信息
void AboutMachine::getAndShowHostInfo(){

    QTableWidget *tablewidget = ui->tableWidget;
    tablewidget->setColumnCount(2);
    tablewidget->setRowCount(10);


    tablewidget->verticalHeader()->setVisible(false);//不显示默认行号

    tablewidget->setItem(0,0,new QTableWidgetItem("架构信息"));
    tablewidget->setItem(0,1,new QTableWidgetItem(QSysInfo::buildAbi()));

    tablewidget->setItem(1,0,new QTableWidgetItem("当前cpu架构"));
    tablewidget->setItem(1,1,new QTableWidgetItem(QSysInfo::currentCpuArchitecture()));

    tablewidget->setItem(2,0,new QTableWidgetItem("内核类型"));
    tablewidget->setItem(2,1,new QTableWidgetItem(QSysInfo::kernelType()));

    tablewidget->setItem(3,0,new QTableWidgetItem("内核版本"));
    tablewidget->setItem(3,1,new QTableWidgetItem(QSysInfo::kernelVersion()));

    tablewidget->setItem(4,0,new QTableWidgetItem("机器主机名"));
    tablewidget->setItem(4,1,new QTableWidgetItem(QSysInfo::machineHostName()));


    // 由于qt只支持到win10 所以要判断系统是win11 需要使用操作系统的版本号进行判断  10.0.22000 当系统的版本号的第三个数大于22000的时候，是win11 所以在此处需要做出判断
    // 将版本号 按照 '.' 分割开，放到QStringList上，然后再转换成数组，再取出数组的第三个位置的数字，进行判断

    int *kernelVersion = stringKernelVersionToInt(QSysInfo::kernelVersion());
    int productVersion = stringPortToInt(QSysInfo::productVersion());
    if(productVersion == 10 && kernelVersion[2] >= 22000){

        tablewidget->setItem(5,0,new QTableWidgetItem("操作系统类型"));
        tablewidget->setItem(5,1,new QTableWidgetItem("windows"));

        tablewidget->setItem(6,0,new QTableWidgetItem("操作系统名称"));
        tablewidget->setItem(6,1,new QTableWidgetItem("windows 11"));

        tablewidget->setItem(7,0,new QTableWidgetItem("操作系统版本"));
        tablewidget->setItem(7,1,new QTableWidgetItem("11"));
    }
    else{

        tablewidget->setItem(5,0,new QTableWidgetItem("操作系统类型"));
        tablewidget->setItem(5,1,new QTableWidgetItem(QSysInfo::productType()));

        tablewidget->setItem(6,0,new QTableWidgetItem("操作系统名称"));
        tablewidget->setItem(6,1,new QTableWidgetItem(QSysInfo::prettyProductName()));

        tablewidget->setItem(7,0,new QTableWidgetItem("操作系统版本"));
        tablewidget->setItem(7,1,new QTableWidgetItem(QSysInfo::productVersion()));
    }

    // 主机的ip地址
    tablewidget->setItem(8,0,new QTableWidgetItem("主机IP地址"));
    tablewidget->setItem(8,1,new QTableWidgetItem(AboutMachine::getIpAddress()));

    // 主机的mac地址
    tablewidget->setItem(9,0,new QTableWidgetItem("主机mac地址"));
    tablewidget->setItem(9,1,new QTableWidgetItem(AboutMachine::getMacAddress()));

    tablewidget->resizeColumnsToContents();
}



AboutMachine::~AboutMachine()
{
    delete ui;
}

