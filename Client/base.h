#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "winmm.lib") // link with Windows MultiMedia lib
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include<algorithm>
#include<vector>
#include<fstream>
#include<Graphics.h>
#include<mmsystem.h>

#define WWidth					640	//���ڿ��
#define WHeight					750	//���ڸ߶�
#define GHeight					640	//��Ϸ����߶�
#define BWidth					140	//��ť������
#define PieceRadius				16	//���ӵİ뾶
#define FlushCount				10
#define PORT						11451 //�˿�
#define err(errMsg)				printf("[line:%d]%sʧ�ܣ�%d��\n",__LINE__,errMsg,GetLastError())
#define suc(sucMsg)				printf("%s�ɹ���\n",sucMsg)

