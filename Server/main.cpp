#pragma warning(disable:4996)

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>

#define PORT						11451 //端口
#define err(errMsg)				printf("[line:%d]%s失败：%d！\n",__LINE__,errMsg,GetLastError())
#define suc(sucMsg)				printf("%s成功！\n",sucMsg)

SOCKADDR_IN cAddr = { 0 };
int len = sizeof cAddr;
SOCKET clientSocket[2];

void func(int index)
{
	//7 通信
	char buf[BUFSIZ];
	int r;
	while (true)
	{
		r = recv(clientSocket[index], buf, BUFSIZ, NULL);
		if (r > 0)
		{
			buf[r] = 0;
			//printf("%s发来的数据:%s\n", inet_ntop(AF_INET,), buf);
			printf("%s发来的数据:%s\n", inet_ntoa(cAddr.sin_addr), buf);
		}
	}
}

int main() {
	//1 确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		err("确定协议版本");
		return -1;
	}
	else suc("确定协议版本");

	//2 创建socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket)
	{
		err("创建socket");
		//9 清理协议版本信息
		WSACleanup();
		return -1;
	}
	else suc("创建socket");

	//3 创建服务器协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//协议地址族
	addr.sin_addr.S_un.S_addr = inet_addr("101.5.172.59");//这里改成服务器IP地址
	addr.sin_port = htons(PORT);//10000左右    小端转大端

	//4 绑定
	int r = bind(serverSocket, (struct sockaddr*)&addr, sizeof addr);
	if (r == -1)
	{
		err("绑定");
		//8 关闭socket
		closesocket(serverSocket);
		//9 清理协议版本信息
		WSACleanup();
		return -1;
	}
	else suc("绑定");

	//5 监听
	r = listen(serverSocket, 10);
	if (r == -1)
	{
		err("监听");
		//8 关闭socket
		closesocket(serverSocket);
		//9 清理协议版本信息
		WSACleanup();
		return -1;
	}
	else suc("监听");

	//6 等待客户端连接
	while (1)
	{
		system("CLS");
		char SendBuffer[256];
		printf("等待客户端连接...\n");

		clientSocket[0] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		sprintf(SendBuffer, "%s", "1");
		send(clientSocket[0], SendBuffer, strlen(SendBuffer), NULL);//发送给客户端1
		sprintf(SendBuffer, "%s", "成功连接服务器！等待其它玩家加入");
		Sleep(200);
		send(clientSocket[0], SendBuffer, strlen(SendBuffer), NULL);//发送给客户端1
		printf("客户端1--%s--已连接\n", inet_ntoa(cAddr.sin_addr));

		clientSocket[1] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		sprintf(SendBuffer, "%s", "-1");
		send(clientSocket[1], SendBuffer, strlen(SendBuffer), NULL);//发送给客户端2
		Sleep(200);
		sprintf(SendBuffer, "%s", "成功连接服务器！游戏开始。对方先手");
		send(clientSocket[1], SendBuffer, strlen(SendBuffer), NULL);//发送给客户端2
		Sleep(200);
		sprintf(SendBuffer, "%s", "游戏开始。己方先手");
		send(clientSocket[0], SendBuffer, strlen(SendBuffer), NULL);//发送给客户端1
		printf("客户端2--%s--已连接\n", inet_ntoa(cAddr.sin_addr));

		while (1)
		{	//这里是收发白子信息
			r = recv(clientSocket[0], SendBuffer, 255, NULL);//获取白子传来的棋子X信息
			if (r == 0)
				break;
			SendBuffer[r] = '\0';
			send(clientSocket[1], SendBuffer, 255, NULL);//转发给黑子


			//这里是收发黑子信息
			r = recv(clientSocket[1], SendBuffer, 255, NULL);//获取白子传来的棋子X信息
			if (r == 0)
				break;
			SendBuffer[r] = '\0';
			send(clientSocket[0], SendBuffer, 255, NULL);//转发给黑子
		}
	}

	//8 关闭socket
	closesocket(serverSocket);
	//9 清理协议版本信息
	WSACleanup();
	printf("服务器关闭！\n");
	return 0;
}