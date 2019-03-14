#include <iostream>
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
//#include "decode.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define PORT_NO 6000
#define SRV_IP_ADDR "192.168.8.101"

int main(int argc, char* argv[])
{
    int ret;
    WSADATA wsaData;    //WSADATA结构被用来储存调用AfxSocketInit全局函数返回的Windows Sockets初始化信息。
    WORD wVersionRequested = MAKEWORD(2, 2);
    SOCKET sockClient;
    SOCKADDR_IN addrSrv;

    // 初始化Windows Socket------
    ret = WSAStartup(wVersionRequested, &wsaData);
    if (ret != 0)
    {
        cout << "WSAStartup() failed:" << WSAGetLastError() << endl;
        return -1;
    }

    // 创建Socket------
    sockClient = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == sockClient)
    {
        cout << "socket() failed:" << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    addrSrv.sin_addr.S_un.S_addr = inet_addr(SRV_IP_ADDR);  //sin_addr存储IP地址，使用in_addr这个数据结构
    addrSrv.sin_family = AF_INET;  //sin_family指代协议族，在socket编程中只能是AF_INET
    addrSrv.sin_port = htons(PORT_NO);  //sin_port存储端口号（使用网络字节顺序），端口号的范围0~65535,同时0~1024范围的端口号已经被系统使用或保留。
    // 连接------
    ret = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if (SOCKET_ERROR == ret)
    {
        cout << "connect() failed:" << WSAGetLastError() << endl;
        closesocket(sockClient);
        WSACleanup();
        return -1;
    }
    else
    {
        cout << "connect() successfully." << endl;
    }

    // 发送数据------
    char sendBuf[MAX_PATH] = {0};
     char recvBuffer[MAX_PATH] = {0};

    while (1)
    {
		memset(recvBuffer, 0, sizeof(recvBuffer));
        ret = recv(sockClient, recvBuffer, sizeof(recvBuffer), 0);  //接受client数据
        if (ret == 0)  //
        {
            cout << "Connection has been gracefully closed." << endl;
            break;
        }
        else if (ret == SOCKET_ERROR)
        {
            cout << "Connection has been closed ungracefully." << endl;
            break;
        }
        FILE *f1=fopen("3.txt","wr");
        FILE *r1;

		fputs(recvBuffer,f1);
		fclose(f1);
		//DES_Decrypt("3.txt","key.txt","4.txt");
		r1=fopen("4.txt","r");
		fscanf(r1,"%s",recvBuffer);
        cout << "Receive Data from Client:" << recvBuffer << endl;
    	
        cin.getline(sendBuf, sizeof(sendBuf));
        if (strcmp(sendBuf, "exit") == 0)
        {
            break;
        }
		FILE *f2=fopen("1.txt","wr");
	    FILE *r2;

		fputs(sendBuf,f2);
		fclose(f2);
		//DES_Encrypt("1.txt","key.txt","2.txt");
		r2=fopen("2.txt","r");
		fscanf(r2,"%s",sendBuf);
        ret = send(sockClient, sendBuf, strlen(sendBuf)+1, 0);
        if (SOCKET_ERROR == ret)
        {
            cout << "send() failed:" << WSAGetLastError() << endl;
            closesocket(sockClient);
            WSACleanup();
            return -1;
        }
    }

    // 清理资源-------
    closesocket(sockClient);
    WSACleanup();

    cout << "exit..." << endl;
    return 0;
}

