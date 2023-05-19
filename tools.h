#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QStringList>
#include <stdio.h>
#include <time.h>
#include <qdebug.h>
#include <Winsock2.h>
#include <Windows.h>
#include <bitset>
#include <QTcpSocket>
#include <QFile>
#include <QTreeWidget>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

// 定义宏
#define ICMP_HEADER_SIZE sizeof(icmpHeader)
#define ICMP_ECHO_REQUEST 0x08
#define ICMP_ECHO_REPLY 0x00

// --------------------定义一些结构体 方便在线程之间传递参数， 比如参数设定，主机的相关信息----------------------- //

// 设定的参数
typedef struct Parameters{
    QString strStartIp;                           // 起始ip
    QString strEndIp;                             // 结束ip
    QString strStartPort;                         // 起始port
    QString strEndPort;                           // 结束port
    QString threadNum;                            // 给定的线程数量
}Parameters;


// 主机信息
typedef struct HostInfos{
    QString ipAddr;                               // 主机的ip地址
    QString isOn;                                 // 是否在线
    QString osInfo;                               // 操作系统的信息
    int ports[65535];                             // 端口数组
    QString services[65535];                      // 服务
    QString potentialBug[65535];                  // 潜在的漏洞
    QString threadAddr;                           // 扫描该主机的线程的地址
    int portNum = 0;
}HostInfos;


// 传递的参数
typedef struct TransferParas{
    QString desIp;                                // 目的主机的ip地址
    QString startPort;                            // 要扫描的目的主机的起始端口号
    QString endPort;                              // 要扫描的目的主机的结束端口号
}TransferParas;


typedef struct icmpHeader{
    unsigned char icmp_type;                            // 消息类型
    unsigned char icmp_code;                            // 代码
    unsigned short icmp_checksum;                       // 校验和
    unsigned short icmp_id;                             // 用来惟一标识此请求的ID号，通常设置为进程ID
    unsigned short icmp_sequence;                       // 序列号
}icmpHeader;

// -------------------------------------实现一些需要使用的工具函数------------------------------------------ //

// 在QString中，将数字转换为字符的操作
// 1. QString::number()
// int tmp = 100;
// QString str = QString::number(tmp);

// 2. QString("%1").arg(i)


// 在QString中，将字符串转换为数字
// 1. toInt()函数
// QString str("100");
// int tmp = str.toInt();         这里的字符串中一定需要保证都是数字格式的，否则的话，转换失败

// QString 转化成 char *
// 需要用到QByteArray temp = str.toLatinl();
// char * cha = temp.data();

// 将QString 的 ip 转换为 int   比如将111.111.111.111  分开存放在一个整形数组上
inline int* stringIpToInt(QString strIp){
    int static ipInt[4];
    QStringList ipList = strIp.split('.');                  // 按 '.' 分成四个字符串 放在QStringList里面
    for(int i=0 ;i<4 ;i++){                                 // 循环遍历这个StringList，将其中的转换为int 并且放入int数组中去
        ipInt[i] = ipList[i].toInt();
    }
    return ipInt;
}


// 将QString 的 port 转换为 int
inline int stringPortToInt(QString strPort){
    return strPort.toInt();
}

// 检验ip地址是否合格     a.b.c.d  &&  a b c d 在区间 0--255
// ?:                     # 非获取匹配，只匹配但是不获取
// (?:1[0-9][0-9]\.)       # 100-199
// (?:2[0-4][0-9]\.)       # 200-249
// (?:25[0-5]\.)           # 250-255
// (?:[1-9][0-9]\.)        # 10-99
// (?:[0-9]\.)             # 0-9
inline bool checkIpForm(QString strIp){
    if(strIp == ""){
        return false;
    }

    // 正则表达式检测字符串
    QRegExp ipRe("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    if(ipRe.exactMatch(strIp))
        return true;
    else
        return false;
}


// 检验ip地址的范围是否准确  结束ip 需要比 开始ip 的地址更 “大”
inline bool checkIpRange(QString startIp, QString endIp){    
    // 先转换为int 再进行比较
    int *startIpInt = stringIpToInt(startIp);
    int *endIpInt = stringIpToInt(endIp);

    // 定义一个flag
     bool flag = true;
    // 循环比较 如果结束ip数组的每个值都大于或者等于开始ip 那么则合法 否则 return error 重新输入。
    for(int i=0; i<4; i++){
        // 如果有一个startIpInt的值比对应位置的endIpInt的值大 那么flag 变成 false
        if(endIpInt[i] < startIpInt[i] ){
            flag = false;
        }
    }

    return flag;
}

// 检验端口的大小是否合格
inline bool checkPort(QString port){
    if(port == ""){
        return false;
    }
    // 先变成int 再看是否在1-65535之间
    int portInt = stringPortToInt(port);
    if(portInt >=1 && portInt <= 65535)
        return true;
    else
        return false;
}


// 检验端口范围是否合格
inline bool checkPortRange(QString startPort, QString endPort){
    // QString --> int
    int startPortInt = stringPortToInt(startPort);
    int endPortInt = stringPortToInt(endPort);
    
    // 比较大小
    if(startPortInt <= endPortInt)
        return true;
    else
        return false;
}

// ------------------------------------------------------------------------------
// 比如说 我们用192.168.31.1来举例

// 为了方便之后的循环遍历IP地址扫描操作，决定将ip字符串 转换成一个整形数，而不是一个整形数组，进行循环遍历 然后在封装icmp包的时候再换成ip地址的原格式
inline int ipStrToNum(QString strIp){
    int *intIp = stringIpToInt(strIp);
    qDebug("%d,%d,%d,%d",intIp[0],intIp[1],intIp[2],intIp[3]);
    return intIp[0] << 24 | intIp[1] << 16 | intIp[2] << 8 | intIp[3];                    // 左移运算符          11000000 10101000 00011111 00000001
}

// 将这个整形数转换成QString
inline QString ipNumToStr(int intIp){
    // 先将这个整形存入 一个整形数组，然后将这个整形数组 转换成字符串，然后拼接起来
    int intIpArray[4];
    intIpArray[0] = (intIp & 0xff) ;                                                  // 00000001
    intIpArray[1] = (intIp & 0xff00) >> 8;                                                // 00011111
    intIpArray[2] = (intIp & 0xff0000) >> 16;                                              // 10101000
    intIpArray[3] = (intIp & 0xff000000) >> 24;                                            // 11000000

    qDebug("%d,%d,%d,%d",intIpArray[0],intIpArray[1],intIpArray[2],intIpArray[3]);
    QString strIp = QString("%1").arg(intIpArray[3]) + QString(".") + QString("%1").arg(intIpArray[2]) + QString(".") +
            QString("%1").arg(intIpArray[1]) + QString(".") + QString("%1").arg(intIpArray[0]);
    return strIp;
}

// 根据ttl判断主机的操作系统类型
// 1、WINDOWS NT/2000   TTL：128
// 2、WINDOWS 95/98     TTL：32
// 3、UNIX              TTL：255
// 4、LINUX             TTL：64
// 5、WIN7          TTL：64

inline QString whatOsIs(int ttl){
    QString os;
    os = "windows";
    if(ttl <= 0){
        os = "无法判断";
        return os;
    }
    else if(ttl <= 64){
        os = "linux";
        return os;
    }
    else if(ttl <= 128){
        os = "windows";
        return os;
    }else{
        os = "unix";
        return os;
    }
    return os;
}


// 计算icmp报文的校验和
inline unsigned short chsum(icmpHeader *picmp, int len){
    long sum = 0;
    unsigned short *pusicmp = (unsigned short *)picmp;

    while ( len > 1 )
    {
        sum += *(pusicmp++);
        if ( sum & 0x80000000 )
        {
            sum = (sum & 0xffff) + (sum >> 16);
        }
        len -= 2;
    }

    if ( len )
    {
        sum += (unsigned short)*(unsigned char *)pusicmp;
    }

    while ( sum >> 16 )
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (unsigned short)~sum;
}

// ----------------------------------下面实现扫描操作-----------------------------------------------------

// 单个端口是否开启
inline bool singlePortScan(QString desIp, int port){
    QTcpSocket socket(0);
    socket.abort();
    socket.connectToHost(desIp, port);
    if(socket.waitForConnected(1000)){
        return true;
    }else{
        return false;
    }
}

inline QString showservice(QString portnum)
{
    qDebug() << "进入";
    QString result="";
    QString a="";
    QString b="";
    int i=0;
    //QFile file("C://network scanner//network-scanner//nmap-services.txt");
    QFile file(":/new/prefix1/nmapservices.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
             qDebug() << "文件打开成功";
             while (!file.atEnd())
             {
                 //qDebug() << "开始";
                 QByteArray line = file.readLine();
                 QString str(line);
                 //qDebug() << str;
                 for(i=0;str[i]>='a'&&str[i]<='z';i++)
                 {
                    a=a+str[i];
                 }
                 while(!((str[i]>='0')&&(str[i]<='9')))
                 {
                       i++;
                 }
                 for(;str[i]>='0'&&str[i]<='9';i++)
                 {
                    b=b+str[i];
                 }
                 if(portnum==b)
                 {
                     result=a;
                     a="";
                     b="";
                     break;
                 }
                 a="";
                 b="";

             }
             file.close();

         }
    //qDebug() << result;
    return result;
}

// 根据端口判断
// 判断端口是否开启，直接使用TCP连接进行测试
inline void portScan(QString desIp, int startPort, int endPort, HostInfos *&host){
    // 给一个参数记录此时的端口数组位置
    int portPlace = 0;
    int servicePlace = 0;
    int bugPlace = 0;

    // 循环遍历
    for(int i=startPort;i<=endPort;i++){
        if(singlePortScan(desIp, i)){                             // 如果开放，将这个端口写入host的端口数组中，并且根据文档，匹配其可能对应的服务，及其可能存在的漏洞。
            host->portNum ++;
            qDebug("%d", i);
            host->ports[portPlace] = i;
            // 根据端口到对应的service文档里 匹配查找
            host->services[portPlace++] = showservice(QString("%1").arg(i));
            // 根据端口到对应的漏洞文档 匹配查找


        }
    }

}

// 构造icmp报文 发送给目标主机 获得返回报， 通过回包源地址判断是需要的回包，
// 在线：通过是否收到指定的回包判断目标主机是否在线
// 操作系统类型：解析回包的消息，获得该报文的ttl值，从而判断目标主机的操作系统类型

// 1. 构建icmp报文
// Type：占8位
// Code：占8位
// Checksum：占16位
// Identifier：设置为ping 进程的进程ID。
// Sequence Number ：每个发送出去的分组递增序列号。

static int respNum = 0;
static int minTime = 0,maxTime = 0,sumTime = 0;

// ping函数 可以直接在子线程中调用 入参：目的地址，起始端口，结束端口  出参：主机
inline void scanning(TransferParas transferParas, HostInfos *&host){

    // 处理参数 开始端口和结束端口
    QByteArray tempIp = transferParas.desIp.toLatin1();
    char *desIp = tempIp.data();
    int intStartPort = stringPortToInt(transferParas.startPort);
    int intEndPort = stringPortToInt(transferParas.endPort);

    WSADATA wsaData;
    int nTimeOut = 1000;                           // 设置超时时间
    char szBuff[ICMP_HEADER_SIZE + 32] = {0};      // 初始化缓冲区
    icmpHeader *pIcmp = (icmpHeader *)szBuff;    // 将缓冲区变成icmp头的类型
    char icmp_data[32] = {0};                      // icmp数据报的数据部分

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 创建需要发送数据的套接字对象
    SOCKET s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

    // 设置接收超时
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)&nTimeOut, sizeof(nTimeOut));

    // 设置目的地址
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.S_un.S_addr = inet_addr(desIp);
    dest_addr.sin_port = htons(0);

    // 构造ICMP包
    pIcmp->icmp_type = ICMP_ECHO_REQUEST;                            // 请求ICMP类型
    pIcmp->icmp_code = 0;                                            // 初始化code
    pIcmp->icmp_id = (USHORT)::GetCurrentProcessId();                // 初始化进程号
    pIcmp->icmp_sequence = 0;                                        // 初始化序列号
    pIcmp->icmp_checksum = 0;                                        // 初始化校验和

    // 填充数据，可以任意
    memcpy((szBuff + ICMP_HEADER_SIZE), "i am icmp request", 32);

    pIcmp->icmp_checksum = chsum((icmpHeader*)szBuff, sizeof (szBuff));          // 计算icmp校验和，并且赋值到构造的icmp报文中

    sockaddr_in from_addr;
    char szRecvBuff[1024];                                           // 接收数据缓冲区
    int nLen = sizeof(from_addr);                                    //
    int ret,flag = 0;

    DWORD  start = GetTickCount();
    ret = sendto(s, szBuff, sizeof(szBuff), 0, (SOCKADDR *)&dest_addr, sizeof(SOCKADDR));

    qDebug("已发送数据.....");
    int i = 0;
    //这里一定要用while循环，因为recvfrom 会接受到很多报文，包括 发送出去的报文也会被收到！ 不信你可以用 wireshark 抓包查看，这个问题纠结来了一晚上 才猜想出来！
    while(1){
        if(i++ > 5){// icmp报文 如果到不了目标主机，是不会返回报文，多尝试几次接受数据，如果都没收到 即请求失败
            flag = 1;
            break;
        }
        memset(szRecvBuff,0,1024);
        //printf("errorCode1:%d\n",WSAGetLastError() );
        int ret = recvfrom(s, szRecvBuff, MAXBYTE, 0, (SOCKADDR *)&from_addr, &nLen);
        //printf("errorCode2:%d\n",WSAGetLastError() );
        //printf("ret=%d,%s\n",ret,inet_ntoa(from_addr.sin_addr)) ;
        //接受到 目标ip的 报文
        if( strcmp(inet_ntoa(from_addr.sin_addr),desIp) == 0)  {
            respNum++;
            break;
        }
    }

    DWORD  end = GetTickCount();
    DWORD time = end -start;

    if(flag){
        printf("请求超时。\n");
        // 主机不在线 ,设置这个主机的在线参数为no
        qDebug("主机不在线....");
        host->isOn = "该主机不在线";
        return;
    }
    sumTime += time;
    if( minTime > time){
        minTime = time;
    }
    if( maxTime < time){
        maxTime = time;
    }

    // 开始接收

    // windows的原始套接字开发，系统没有去掉ip头，需要自己处理
    // ip头部的第一个字节），只有一个字节不涉及大小端的问题，前四位表示ip协议版本号，后四位表示ip头长度部分
    char ipInfo = szRecvBuff[0];

    // ip头部的第九个字节是ttl的值
    unsigned char ttl = (unsigned char) szRecvBuff[8];
    // 将char 转换成 int 然后判断是什么操作系统，并且将操作系统赋值给主机host
    int ttlInt = (int)ttl;
    QString hostOs = whatOsIs(ttlInt);                                    // 判断主机的操作系统
    host->osInfo = hostOs;                                                 // 赋值

    int ipVer = ipInfo >> 4;
    int ipHeadLen = ((char)( ipInfo << 4) >> 4) * 4;
    if( ipVer  == 4) {
        //ipv4
        //printf("ipv4 len = %d\n",ipHeadLen);
        // 跨过ip协议头，得到ICMP协议头的位置，不过是网络字节序。
        // 网络字节序 是大端模式 低地址 高位字节 高地址 低位字节。-> 转换为 本地字节序 小端模式 高地址高字节 低地址低字节
        icmpHeader* icmp_rep = (icmpHeader*)(szRecvBuff + ipHeadLen);
        //由于校验和是 2个字节 涉及大小端问题，需要转换字节序
        unsigned short checksum_host = ntohs(icmp_rep->icmp_checksum);// 转主机字节序 和wireshark 抓取的报文做比较

        //printf("type = %d ，checksum_host = %x\n",icmp_rep,checksum_host);

        if(icmp_rep->icmp_type == 0){ //回显应答报文
            // 说明主机在线， 设置主机的在线状态
            host->isOn = "主机在线";
        } else{
            // 主机不在线
            host->isOn = "主机不在线";
            printf("请求超时。type = %d\n",icmp_rep->icmp_type);
        }
    }else{
        // ipv6 icmpv6 和 icmpv4 不一样，要做对应的处理
        //printf("ipv6 len = %d\n",ipLen);
    }

    // ---------------------------------------上面是主机扫描程序，下面是端口扫描程序--------------------------------------------------


    // 端口扫描函数  入参：目的ip，起始端口，结束端口   出参：host

    // 检测是否实现在线功能
    qDebug("portScan....");

    portScan(transferParas.desIp, intStartPort, intEndPort, host);
}

//循环删除节点
inline void removeTree(QTreeWidgetItem *item)
{
    int count = item->childCount();;
    qDebug("%d", count);
    if(count==0)//没有子节点，直接删除
    {
        delete item;
        return;
    }

    for(int i=0; i<count; i++)
    {
        QTreeWidgetItem *childItem = item->child(i);//删除子节点
        removeTree(childItem);
        qDebug("正在删除结点");
    }
    delete item;//最后将自己删除
}
// 删除树结点
inline void removeTreeWidget(QTreeWidget *treeWidget){
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    qDebug("%d",items.size());
    for (int i = 0; i < items.size(); ++i) {
        removeTree(items[i]);
    }
}

#endif // TOOLS_H
