#include <time.h>
#include <stdlib.h>
#include <memory.h>
#define PLAIN_FILE_OPEN_ERROR -1
#define KEY_FILE_OPEN_ERROR -2
#define CIPHER_FILE_OPEN_ERROR -3
#define OK 1;
#include <iostream>
#include <windows.h>
#include <process.h>
#include<stdio.h>
//#include "encrypt.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define PORT_NO 6000
#define BACKLOG 10

unsigned __stdcall clientThrd(void *lpParam)
{
    SOCKET sockClient = (SOCKET)lpParam;
    int ret = 0;
    char recvBuffer[MAX_PATH] = {0};
	char sendBuf[MAX_PATH] = {0};
    // 接收数据------
    while (1)
    {
    	cin.getline(sendBuf, sizeof(sendBuf));
        if (strcmp(sendBuf, "exit") == 0)
        {
            break;
        }
		FILE *f=fopen("1.txt","wr");
	    FILE *r;

		fputs(sendBuf,f);
		fclose(f);
		//DES_Encrypt("1.txt","key.txt","2.txt");
		r=fopen("2.txt","r");
		fscanf(r,"%s",sendBuf);
        ret = send(sockClient, sendBuf, strlen(sendBuf)+1, 0);
        if (SOCKET_ERROR == ret)
        {
            cout << "send() failed:" << WSAGetLastError() << endl;
            closesocket(sockClient);
            WSACleanup();
            return -1;
        }
    	
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
        FILE *p=fopen("3.txt","wr");
        FILE *q;

		fputs(recvBuffer,p);
		fclose(p);
		//DES_Decrypt("3.txt","key.txt","4.txt");
		q=fopen("4.txt","r");
		fscanf(q,"%s",recvBuffer);
        cout << "Receive Data from Sever:" << recvBuffer << endl;
    }

    closesocket(sockClient);

    return 0;
}

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int ret;
    WORD wVersionRequested = MAKEWORD(2, 2);
    SOCKET sockSrv, sockConn;
    SOCKADDR_IN addrSrv, addrClient;
    HANDLE hThread = NULL;

    // 初始化Windows Socket------
    ret = WSAStartup(wVersionRequested, &wsaData);
    if (ret != 0)
    {
        cout << "WSAStartup() failed:" << WSAGetLastError() << endl;
        return -1;
    }

    // 创建Socket------
    sockSrv = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == sockSrv)
    {
        cout << "socket() failed:" << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(PORT_NO);

    // Bind Socket------
    ret = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if (SOCKET_ERROR == ret)
    {
        cout << "bind() failed:" << WSAGetLastError() << endl;
        closesocket(sockSrv);
        WSACleanup();
        return -1;
    }

    // 监听------
    ret = listen(sockSrv, BACKLOG);
    if (SOCKET_ERROR == ret)
    {
        cout << "listen() failed:" << WSAGetLastError() << endl;
        closesocket(sockSrv);
        WSACleanup();
        return -1;
    }

    cout<< "Server started......" << endl;

    // Accept------
    int len=sizeof(SOCKADDR);
    while(1)
    {
        sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
        if (INVALID_SOCKET == sockConn)
        {
            cout << "accept() failed:" << WSAGetLastError() << endl;
            break;
        }
        hThread = (HANDLE)_beginthreadex(NULL, 0, clientThrd, (void*)sockConn, 0, NULL);
        if (hThread == 0)
        {
            cout << "_beginthreadex() failed." << endl;
            break;
        }
        CloseHandle(hThread);
    }

    // 清理资源------
    closesocket(sockSrv);
    WSACleanup();

    cout << "exit..." << endl;
    return 0;
}
