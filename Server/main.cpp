#pragma warning(disable:4996)

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>

#define PORT						11451 //�˿�
#define err(errMsg)				printf("[line:%d]%sʧ�ܣ�%d��\n",__LINE__,errMsg,GetLastError())
#define suc(sucMsg)				printf("%s�ɹ���\n",sucMsg)

SOCKADDR_IN cAddr = { 0 };
int len = sizeof cAddr;
SOCKET clientSocket[2];

void func(int index)
{
	//7 ͨ��
	char buf[BUFSIZ];
	int r;
	while (true)
	{
		r = recv(clientSocket[index], buf, BUFSIZ, NULL);
		if (r > 0)
		{
			buf[r] = 0;
			//printf("%s����������:%s\n", inet_ntop(AF_INET,), buf);
			printf("%s����������:%s\n", inet_ntoa(cAddr.sin_addr), buf);
		}
	}
}

int main() {
	//1 ȷ��Э��汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		err("ȷ��Э��汾");
		return -1;
	}
	else suc("ȷ��Э��汾");

	//2 ����socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket)
	{
		err("����socket");
		//9 ����Э��汾��Ϣ
		WSACleanup();
		return -1;
	}
	else suc("����socket");

	//3 ����������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//Э���ַ��
	addr.sin_addr.S_un.S_addr = inet_addr("101.5.172.59");//����ĳɷ�����IP��ַ
	addr.sin_port = htons(PORT);//10000����    С��ת���

	//4 ��
	int r = bind(serverSocket, (struct sockaddr*)&addr, sizeof addr);
	if (r == -1)
	{
		err("��");
		//8 �ر�socket
		closesocket(serverSocket);
		//9 ����Э��汾��Ϣ
		WSACleanup();
		return -1;
	}
	else suc("��");

	//5 ����
	r = listen(serverSocket, 10);
	if (r == -1)
	{
		err("����");
		//8 �ر�socket
		closesocket(serverSocket);
		//9 ����Э��汾��Ϣ
		WSACleanup();
		return -1;
	}
	else suc("����");

	//6 �ȴ��ͻ�������
	while (1)
	{
		system("CLS");
		char SendBuffer[256];
		printf("�ȴ��ͻ�������...\n");

		clientSocket[0] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		sprintf(SendBuffer, "%s", "1");
		send(clientSocket[0], SendBuffer, strlen(SendBuffer), NULL);//���͸��ͻ���1
		sprintf(SendBuffer, "%s", "�ɹ����ӷ��������ȴ�������Ҽ���");
		Sleep(200);
		send(clientSocket[0], SendBuffer, strlen(SendBuffer), NULL);//���͸��ͻ���1
		printf("�ͻ���1--%s--������\n", inet_ntoa(cAddr.sin_addr));

		clientSocket[1] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		sprintf(SendBuffer, "%s", "-1");
		send(clientSocket[1], SendBuffer, strlen(SendBuffer), NULL);//���͸��ͻ���2
		Sleep(200);
		sprintf(SendBuffer, "%s", "�ɹ����ӷ���������Ϸ��ʼ���Է�����");
		send(clientSocket[1], SendBuffer, strlen(SendBuffer), NULL);//���͸��ͻ���2
		Sleep(200);
		sprintf(SendBuffer, "%s", "��Ϸ��ʼ����������");
		send(clientSocket[0], SendBuffer, strlen(SendBuffer), NULL);//���͸��ͻ���1
		printf("�ͻ���2--%s--������\n", inet_ntoa(cAddr.sin_addr));

		while (1)
		{	//�������շ�������Ϣ
			r = recv(clientSocket[0], SendBuffer, 255, NULL);//��ȡ���Ӵ���������X��Ϣ
			if (r == 0)
				break;
			SendBuffer[r] = '\0';
			send(clientSocket[1], SendBuffer, 255, NULL);//ת��������


			//�������շ�������Ϣ
			r = recv(clientSocket[1], SendBuffer, 255, NULL);//��ȡ���Ӵ���������X��Ϣ
			if (r == 0)
				break;
			SendBuffer[r] = '\0';
			send(clientSocket[0], SendBuffer, 255, NULL);//ת��������
		}
	}

	//8 �ر�socket
	closesocket(serverSocket);
	//9 ����Э��汾��Ϣ
	WSACleanup();
	printf("�������رգ�\n");
	return 0;
}