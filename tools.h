#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QStringList>

// --------------------定义一些结构体 方便在线程之间传递参数， 比如参数设定，主机的相关信息----------------------- //

// 参数
typedef struct{
    QString strStartIp;                           // 起始ip
    QString strEndIp;                             // 结束ip
    QString strStartPort;                         // 起始port
    QString strEndPort;                           // 结束port
    QString threadNum;                            // 给定的线程数量
}Parameters;


// 主机信息
typedef struct{
    QString ipAddr;                               // 主机的ip地址
    QString isOn;                                 // 是否在线
    QString osInfo;                               // 操作系统的信息
    QString ports[65535];                               // 端口数组
    QString services[65535];                            // 服务
    QString potentialBug[65535];                        // 潜在的漏洞
}HostInfos;

// -------------------------------------实现一些需要使用的工具函数------------------------------------------ //

// 在QString中，将数字转换为字符的操作
// 1. QString::number()
// int tmp = 100;
// QString str = QString::number(tmp);

// 2. QString("1%").arg(i)


// 在QString中，将字符串转换为数字
// 1. toInt()函数
// QString str("100");
// int tmp = str.toInt();         这里的字符串中一定需要保证都是数字格式的，否则的话，转换失败

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


#endif // TOOLS_H
