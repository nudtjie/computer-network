#include <Winsock2.h>
#include <stdio.h>
#include<stdlib.h>


#define INVALID_VALUE32 0xFFFF
#pragma comment(lib, "ws2_32.lib")

int main()
{
	char* namec; //���÷���������
	namec="�ͻ���";
	int err = INVALID_VALUE32;
	WORD ver;
	WSADATA wsaData;

	//����汾��
	ver = MAKEWORD(2, 2);   /* �汾��Ϊ2.2�汾,1.1Ҳ���� */

    /* ��������ʼ�� */
	err = WSAStartup(ver, &wsaData);
	if (err != 0)    /* ���socket�Ƿ��ʼ���ɹ� */
	{
		return 0;
	}
	if ((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
	{
		WSACleanup();    /* �汾��������������DLL */
		return 0;
	}
	
	 SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0); /* AF_INET��ʾ����ʹ�õķ�Χinternetwork: UDP, TCP, etc,
	                                                     SOCK_STREAM��ʾʹ�õ���TCP
	                                                     SOCK_DGRAM ��ʾʹ�õ���UDP���� */

	SOCKADDR_IN addrSrv;                     //������addrSrv
	addrSrv.sin_addr.S_un.S_addr = inet_addr("169.254.175.93"); // ��������Ϊ��������IP��ַ,���������Լ��Ļ�������д��,����д�ػ���ַ��
	addrSrv.sin_family           = AF_INET;
	addrSrv.sin_port             = htons(7851); /* �˿����ֻҪ����ϵͳ��ͻ���� */

	/* ���弸��������,Ҳ�ǽ��ܺͷ��͵�,���ó�100���� */
	char recvbuf[100];
	char sendbuf[100];

	int len=sizeof(SOCKADDR);

	printf("\n************��ӭʹ��UDP��client*******\n");

	/* ���������ѭ�� */
	while (1)
	{
		printf("\n��Ҫ˵������������q:\n");
		gets(sendbuf);
		for(int i=0;sendbuf[i]!=0;i++)
		{
			if(sendbuf[i]>='a'&&sendbuf[i]<='z')
			{
				sendbuf[i]-=32;
			}
			else if(sendbuf[i]>='A'&&sendbuf[i]<='Z')
			{
				sendbuf[i]+=32;
			}
		} 

		sendto(sockClient, sendbuf, strlen(sendbuf)+1, 0, (SOCKADDR *)&addrSrv, len);
		recvfrom(sockClient, recvbuf, 100, 0, (SOCKADDR *)&addrSrv, &len);

		/* �����յ������ݴ������ݻ��������Ա���ʾ */
		printf("%s say: %s", namec, recvbuf);

	}

	/* ���socket��� */
	closesocket(sockClient);

	WSACleanup();

	return 0;
}
 
