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
    WSADATA wsaData;    //WSADATA�ṹ�������������AfxSocketInitȫ�ֺ������ص�Windows Sockets��ʼ����Ϣ��
    WORD wVersionRequested = MAKEWORD(2, 2);
    SOCKET sockClient;
    SOCKADDR_IN addrSrv;

    // ��ʼ��Windows Socket------
    ret = WSAStartup(wVersionRequested, &wsaData);
    if (ret != 0)
    {
        cout << "WSAStartup() failed:" << WSAGetLastError() << endl;
        return -1;
    }

    // ����Socket------
    sockClient = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == sockClient)
    {
        cout << "socket() failed:" << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    addrSrv.sin_addr.S_un.S_addr = inet_addr(SRV_IP_ADDR);  //sin_addr�洢IP��ַ��ʹ��in_addr������ݽṹ
    addrSrv.sin_family = AF_INET;  //sin_familyָ��Э���壬��socket�����ֻ����AF_INET
    addrSrv.sin_port = htons(PORT_NO);  //sin_port�洢�˿ںţ�ʹ�������ֽ�˳�򣩣��˿ںŵķ�Χ0~65535,ͬʱ0~1024��Χ�Ķ˿ں��Ѿ���ϵͳʹ�û�����
    // ����------
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

    // ��������------
    char sendBuf[MAX_PATH] = {0};
     char recvBuffer[MAX_PATH] = {0};

    while (1)
    {
		memset(recvBuffer, 0, sizeof(recvBuffer));
        ret = recv(sockClient, recvBuffer, sizeof(recvBuffer), 0);  //����client����
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

    // ������Դ-------
    closesocket(sockClient);
    WSACleanup();

    cout << "exit..." << endl;
    return 0;
}

