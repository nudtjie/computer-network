#include <Winsock2.h>
#include <stdio.h>
#include<stdlib.h>

#define INVALID_VALUE32 0xFFFF
#pragma comment(lib, "ws2_32.lib")

int main()
{

	char* names;
	names="�����";
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
	/* ��ʼsocket�����岿��:����socket */
    SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0); /* AF_INET��ʾ����ʹ�õķ�Χinternetwork: UDP, TCP, etc,
	                                                     SOCK_STREAM��ʾʹ�õ���TCP
	                                                     SOCK_DGRAM ��ʾʹ�õ���UDP���� */
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family           = AF_INET;
	addrSrv.sin_port             = htons(7851); /* �˿����ֻҪ����ϵͳ��ͻ���� */

	/* ���濪ʼ��bind���� */
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); //�󶨶˿� sockSrvҪǿ��ת����SOCKADDR

	/* ���弸��������,Ҳ�ǽ��ܺͷ��͵�,���ó�100���� */
	char recvbuf[100];
	char sendbuf[100];
	char tmpbuf[100];

	SOCKADDR_IN addrClient;// �����ϵĿͻ���ip��ַ
	int len = sizeof(SOCKADDR);
    printf("\n************��ӭʹ��UDP��Sever*******\n");

	/* ���������ѭ�� */
	while (1)
	{
		recvfrom(sockSrv, recvbuf, 100, 0, (SOCKADDR *)&addrClient, &len);

		if ('Q' == recvbuf[0])
		{
			sendto(sockSrv, "Q", strlen("Q")+1, 0, (SOCKADDR *)&addrClient, len);
			printf("�������!\n");
			break;
		}

		/* �����յ������ݴ������ݻ��������Ա���ʾ */
		sprintf(tmpbuf,"%s say: %s ",names, recvbuf);
		printf("%s\n", tmpbuf);

		printf("��������Ҫ˵������:\n");
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
		sendto(sockSrv, sendbuf, strlen(sendbuf)+1, 0, (SOCKADDR *)&addrClient, len);

	}

	/* ���socket��� */
	closesocket(sockSrv);

	WSACleanup();

	return 0;
}
