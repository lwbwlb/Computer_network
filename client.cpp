#if 0
#include <iostream>
#include "winsock.h"
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main()
{
    //开启网路库
    WORD wdVersion = MAKEWORD(2, 2);
    WSADATA wdata;

    int wres = WSAStartup(wdVersion, &wdata);

    if (0 != wres) {
        cout << "客户端开启网络库失败：" << wres << endl;
        return 0;
    }

    //检测版本号
    if (2 != LOBYTE(wdata.wVersion) || 2 != HIBYTE(wdata.wVersion)) {
        cout << "版本号不支持" << endl;

        WSACleanup();
        return 0;
    }

    //设置服务器ip信息

    SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == sockServer) {
        cout << "客户端：创建socket失败 " << endl;
        WSACleanup();
        return 0;
    }


    //connect
    SOCKADDR_IN sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(12345);
    sAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //Winsock2.h头文件不支持上述函数
    /*sAddr.sin_addr.S_un.S_un_b.s_b1 = 127;
    sAddr.sin_addr.S_un.S_un_b.s_b2 = 0;
    sAddr.sin_addr.S_un.S_un_b.s_b3 = 0;
    sAddr.sin_addr.S_un.S_un_b.s_b4 = 1;*/

    int cres = connect(sockServer, (const SOCKADDR*)&sAddr, sizeof(sAddr));
    if (0 != cres) {
        int a = WSAGetLastError();

        switch (a) {
        case WSAECONNREFUSED:
            cout << "客户端无法连接：未开机" << endl;
            break;
        default:
            cout << "客户端连接出错：" << a << endl;
            break;
        }

        closesocket(sockServer);
        WSACleanup();
        return 0;

    }
    //发送消息

    while (1) {

        string str;        getline(cin, str);

        if (str[0] == '0') break;
        //输入0让客户端正常退出
        int sres = send(sockServer, str.c_str(), str.size(), 0);
        if (SOCKET_ERROR == sres) {
            int a = WSAGetLastError();
            cout << "客户端：发送失败" << a << endl;
            closesocket(sockServer);
            WSACleanup();
            return 0;
        }

        char buf[1024] = { 0 };
        int rres = recv(sockServer, buf, sizeof(buf) - 1, 0);  // 0表示默认读写，不必等满。
        if (SOCKET_ERROR == rres) {
            int a = WSAGetLastError();
            cout << " 客户端：接收失败 " << a << endl;

            closesocket(sockServer);
            WSACleanup();
            return 0;
        }
        else
        {
            cout << "客户端收到（" << rres << "）" << buf << endl;
        }

    }

    closesocket(sockServer);
    WSACleanup();
    return 0;
}




#endif




















