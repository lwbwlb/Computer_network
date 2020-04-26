//#include <stdlib.h>
#include <Winsock.h>
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
using namespace std;


int main()
{
    //打开网络库
    WORD wdVersion = MAKEWORD(2, 2);
    WSADATA wdSockMsg;

    int wres = WSAStartup(wdVersion, &wdSockMsg);

    if (0 != wres) {
        switch (wres)
        {
        case WSASYSNOTREADY:
            cout << "系统配置问题，请检查ws2_32.lib是否在工作目录" << endl;
            break;
        case WSAVERNOTSUPPORTED:
            cout << "网络库版本号不支持" << endl;
            break;
        case WSAEINPROGRESS:
            cout << "服务器忙" << endl;
            break;
        case WSAEPROCLIM:
            cout << "Windows套接字实现所支持的任务数量已达到限制。请关闭不必要软件" << endl;
            break;
        case WSAEFAULT:
            cout << "网络库启动第二个参数错了" << endl;
            break;
        default:
            break;
        }

        return 0;
    }
    //检测版本
    if (2 != LOBYTE(wdSockMsg.wVersion) || 2 != HIBYTE(wdSockMsg.wVersion)) {

        WSACleanup();
        return 0;
    }

    //创建socket
    SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == sockServer) {
        int a = WSAGetLastError();  //0--success
        cout << "服务器端创建socket失败：" << a << endl;

        WSACleanup();
        return 0;
    }

    //绑定服务器socket

    SOCKADDR_IN sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(12345);
    sAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    /*sAddr.sin_addr.S_un.S_un_b.s_b1 = 127;
    sAddr.sin_addr.S_un.S_un_b.s_b2 = 0;
    sAddr.sin_addr.S_un.S_un_b.s_b3 = 0;
    sAddr.sin_addr.S_un.S_un_b.s_b4 = 1;*/
    //inet_pton()
    int bres = bind(sockServer, (const SOCKADDR*)&sAddr, sizeof(sAddr));
    if (SOCKET_ERROR == bres) {
        int a = WSAGetLastError();

        cout << "服务器端bind失败：" << a << endl;

        closesocket(sockServer);
        WSACleanup();
        return 0;
    }

    //监听

    int lres = listen(sockServer, SOMAXCONN);

    if (SOCKET_ERROR == lres) {
        int a = WSAGetLastError();

        cout << "服务器端listen失败(无法获得用户连接)：" << a << endl;

        closesocket(sockServer);
        WSACleanup();
        return 0;
    }
    cout << "服务器端已启动..." << endl;

    //接受连接，创建一个socket客户端，会阻塞
    SOCKADDR_IN cAddr;
    int cAddrlen = sizeof(cAddr);
    SOCKET sockClient = accept(sockServer, (SOCKADDR*)&cAddr, &cAddrlen);

    if (INVALID_SOCKET == sockClient) {
        int a = WSAGetLastError();

        cout << "创建sockClient失败：" << a << endl;

        closesocket(sockServer);
        WSACleanup();
        return 0;
    }

    //与客户端通信:收,会阻塞

    char buf[1500];
    int rres = recv(sockClient, buf, sizeof(buf) - 1, 0);

    if (SOCKET_ERROR == rres) {

        int a = WSAGetLastError();
        cout << "服务器recv错误：" << a << endl;
    }
    else if (0 == rres)
    {
        cout << "客户端掉线" << endl;
    }
    else {
        cout << rres << ":" << buf << endl;
    }

    //与客户端通信：发
    char sbuf[] = "已收到数据.";

    int sres = send(sockClient, sbuf, sizeof(sbuf), 0);
    if (SOCKET_ERROR == sres) {

        int a = WSAGetLastError();
        cout << "服务器send错误:" << a << endl;

    }

    cin.get();

    closesocket(sockClient);
    closesocket(sockServer);
    WSACleanup();
    return 0;
}
