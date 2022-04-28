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

#define WWidth					640	//窗口宽度
#define WHeight					750	//窗口高度
#define GHeight					640	//游戏区域高度
#define BWidth					140	//按钮区域宽度
#define PieceRadius				16	//棋子的半径
#define FlushCount				10
#define PORT						11451 //端口
#define err(errMsg)				printf("[line:%d]%s失败：%d！\n",__LINE__,errMsg,GetLastError())
#define suc(sucMsg)				printf("%s成功！\n",sucMsg)

