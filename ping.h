//#ifndef PING_H
//#define PING_H

//#include <stdio.h>
//#include <time.h>
//#include <qdebug.h>
//#include <Winsock2.h>
//#include <Windows.h>
//#include <bitset>
//#include <tools.h>
//using namespace std;
//#pragma comment (lib, "ws2_32.lib")
//// 定义宏
//#define ICMP_HEADER_SIZE sizeof(icmpHeader)
//#define ICMP_ECHO_REQUEST 0x08
//#define ICMP_ECHO_REPLY 0x00

//// 实现ping操作

//// 构造icmp报文 发送给目标主机 获得返回报， 通过回包源地址判断是需要的回包，
//// 在线：通过是否收到指定的回包判断目标主机是否在线
//// 操作系统类型：解析回包的消息，获得该报文的ttl值，从而判断目标主机的操作系统类型

//// 1. 构建icmp报文
//// Type：占8位
//// Code：占8位
//// Checksum：占16位
//// Identifier：设置为ping 进程的进程ID。
//// Sequence Number ：每个发送出去的分组递增序列号。
//typedef struct{
//    unsigned char icmp_type;                            // 消息类型
//    unsigned char icmp_code;                            // 代码
//    unsigned short icmp_checksum;                       // 校验和
//    unsigned short icmp_id;                             // 用来惟一标识此请求的ID号，通常设置为进程ID
//    unsigned short icmp_sequence;                       // 序列号
//}icmpHeader;


//static int respNum = 0;
//static int minTime = 0,maxTime = 0,sumTime = 0;

//// ping函数 可以直接在子线程中调用 入参：目的地址，起始端口，结束端口  出参：主机
//inline void scanning(TransferParas transferParas, HostInfos &host){

//    // 处理参数 开始端口和结束端口
//    QByteArray tempIp = transferParas.desIp.toLatin1();
//    char *desIp = tempIp.data();
//    int intStartPort = stringPortToInt(transferParas.startPort);
//    int intEndPort = stringPortToInt(transferParas.endPort);

//    WSADATA wsaData;
//    int nTimeOut = 1000;                           // 设置超时时间
//    char szBuff[ICMP_HEADER_SIZE + 32] = {0};      // 初始化缓冲区
//    icmpHeader *pIcmp = (icmpHeader *)szBuff;    // 将缓冲区变成icmp头的类型
//    char icmp_data[32] = {0};                      // icmp数据报的数据部分

//    WSAStartup(MAKEWORD(2, 2), &wsaData);

//    // 创建需要发送数据的套接字对象
//    SOCKET s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

//    // 设置接收超时
//    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)&nTimeOut, sizeof(nTimeOut));

//    // 设置目的地址
//    sockaddr_in dest_addr;
//    dest_addr.sin_family = AF_INET;
//    dest_addr.sin_addr.S_un.S_addr = inet_addr(desIp);
//    dest_addr.sin_port = htons(0);

//    // 构造ICMP包
//    pIcmp->icmp_type = ICMP_ECHO_REQUEST;                            // 请求ICMP类型
//    pIcmp->icmp_code = 0;                                            // 初始化code
//    pIcmp->icmp_id = (USHORT)::GetCurrentProcessId();                // 初始化进程号
//    pIcmp->icmp_sequence = 0;                                        // 初始化序列号
//    pIcmp->icmp_checksum = 0;                                        // 初始化校验和

//    // 填充数据，可以任意
//    memcpy((szBuff + ICMP_HEADER_SIZE), "i am icmp request", 32);

//    pIcmp->icmp_checksum = chsum((icmpHeader*)szBuff, sizeof (szBuff));          // 计算icmp校验和，并且赋值到构造的icmp报文中

//    sockaddr_in from_addr;
//    char szRecvBuff[1024];                                           // 接收数据缓冲区
//    int nLen = sizeof(from_addr);                                    //
//    int ret,flag = 0;

//    DWORD  start = GetTickCount();
//    ret = sendto(s, szBuff, sizeof(szBuff), 0, (SOCKADDR *)&dest_addr, sizeof(SOCKADDR));

//    int i = 0;
//    //这里一定要用while循环，因为recvfrom 会接受到很多报文，包括 发送出去的报文也会被收到！ 不信你可以用 wireshark 抓包查看，这个问题纠结来了一晚上 才猜想出来！
//    while(1){
//        if(i++ > 5){// icmp报文 如果到不了目标主机，是不会返回报文，多尝试几次接受数据，如果都没收到 即请求失败
//            flag = 1;
//            break;
//        }
//        memset(szRecvBuff,0,1024);
//        //printf("errorCode1:%d\n",WSAGetLastError() );
//        int ret = recvfrom(s, szRecvBuff, MAXBYTE, 0, (SOCKADDR *)&from_addr, &nLen);
//        //printf("errorCode2:%d\n",WSAGetLastError() );
//        //printf("ret=%d,%s\n",ret,inet_ntoa(from_addr.sin_addr)) ;
//        //接受到 目标ip的 报文
//        if( strcmp(inet_ntoa(from_addr.sin_addr),desIp) == 0)  {
//            respNum++;
//            break;
//        }
//    }

//    DWORD  end = GetTickCount();
//    DWORD time = end -start;

//    if(flag){
//        printf("请求超时。\n");
//        // 主机不在线 ,设置这个主机的在线参数为no
//        host.isOn = "该主机不在线";
//        return;
//    }
//    sumTime += time;
//    if( minTime > time){
//        minTime = time;
//    }
//    if( maxTime < time){
//        maxTime = time;
//    }

//    // 开始接收

//    // windows的原始套接字开发，系统没有去掉ip头，需要自己处理
//    // ip头部的第一个字节），只有一个字节不涉及大小端的问题，前四位表示ip协议版本号，后四位表示ip头长度部分
//    char ipInfo = szRecvBuff[0];

//    // ip头部的第九个字节是ttl的值
//    unsigned char ttl = (unsigned char) szRecvBuff[8];
//    // 将char 转换成 int 然后判断是什么操作系统，并且将操作系统赋值给主机host
//    int ttlInt = (int)ttl;
//    QString hostOs = whatOsIs(ttlInt);                                    // 判断主机的操作系统
//    host.osInfo = hostOs;                                                 // 赋值

//    int ipVer = ipInfo >> 4;
//    int ipHeadLen = ((char)( ipInfo << 4) >> 4) * 4;
//    if( ipVer  == 4) {
//        //ipv4
//        //printf("ipv4 len = %d\n",ipHeadLen);
//        // 跨过ip协议头，得到ICMP协议头的位置，不过是网络字节序。
//        // 网络字节序 是大端模式 低地址 高位字节 高地址 低位字节。-> 转换为 本地字节序 小端模式 高地址高字节 低地址低字节
//        icmpHeader* icmp_rep = (icmpHeader*)(szRecvBuff + ipHeadLen);
//        //由于校验和是 2个字节 涉及大小端问题，需要转换字节序
//        unsigned short checksum_host = ntohs(icmp_rep->icmp_checksum);// 转主机字节序 和wireshark 抓取的报文做比较

//        //printf("type = %d ，checksum_host = %x\n",icmp_rep,checksum_host);

//        if(icmp_rep->icmp_type == 0){ //回显应答报文
//            // 说明主机在线， 设置主机的在线状态
//            host.isOn = "主机在线";
//        } else{
//            // 主机不在线
//            host.isOn = "主机不在线";
//            printf("请求超时。type = %d\n",icmp_rep->icmp_type);
//        }
//    }else{
//        // ipv6 icmpv6 和 icmpv4 不一样，要做对应的处理
//        //printf("ipv6 len = %d\n",ipLen);
//    }

//    // ---------------------------------------上面是主机扫描程序，下面是端口扫描程序--------------------------------------------------


//    // 端口扫描函数  入参：目的ip，起始端口，结束端口   出参：host


//    // 将扫描完成的host 传回main函数， 展现出来


//}
//#endif // PING_H
