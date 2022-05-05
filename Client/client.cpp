#include"linked_list.h"
#include"menu.h"
#include"AI.h"

int ret = 0;			//用于判断先手还是后手的临时变量
int Map[15][15] = { 0 };	//用于储存棋盘落子信息
int Position = 0;		//判断先手还是后手，若为1则为先手，若为-1则为后手
int PX = 0, PY = 0;
char RecvBuffer[256] = { '0' };
char SendBufferX[256] = { '0' };
char SendBufferY[256] = { '0' };
BOOL GameOver = FALSE;				//判断是否有人获胜
BOOL ISLAUNCH = FALSE;				//自己是否下子
Node* HEAD = NULL, * _END = NULL;		//记录全部落子顺序
extern int rounds = 0;
Menu menu;							//实例进行游戏操作
AI ai;
extern std::vector<Point> SetChessOrder = {};			//用于存储落子记录的STL容器

extern const char* filePath1;		//文件路径1，存取AI对弈残局
extern const char* filePath2;		//文件路径2，存取本地双人对战残局
extern const int vertif;		//vertif放在文件的开头和结尾用于简单判断是否正确读取数据 


extern void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color, int alpha);

void InitGame()
{
	IMAGE img1, img2, img3, img4, img5;
	clearrectangle(0, 0, WWidth, GHeight);
	setfillcolor(RGB(105, 105, 105));
	fillrectangle(0, 0, WWidth, GHeight);
	for (int i = 0; i < 15; i++)
	{
		line(40 + 40 * i, 40, 40 + 40 * i, GHeight - 40);
		line(40, 40 + 40 * i, WWidth - 40, 40 + 40 * i);
	}
	setfillcolor(WHITE);
	fillcircle(160, 160, 5);
	fillcircle(480, 160, 5);
	fillcircle(320, 320, 5);
	fillcircle(160, 480, 5);
	fillcircle(480, 480, 5);

}

void WithAI()
{
	HWND hwnd = initgraph(WWidth, WHeight, 0);
	SetWindowTextA(hwnd, "五子棋");
	mciSendString("open ./src/WalkInDust.mp3 alias bgm", NULL, 0, NULL);		//循环播放背景音乐
	mciSendString("play bgm repeat", NULL, 0, NULL);
	InitGame();

	//初始化游戏数据
	SetChessOrder.clear();

	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			Map[i][j] = 0;
		}
	}

	MOUSEMSG m = { 0 };

	memset(RecvBuffer, 0, sizeof(RecvBuffer));
	sprintf(RecvBuffer, "轮到黑方落子");
	menu.DrawSystemInfo();

	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();	//获取一个鼠标消息

		if (m.uMsg == WM_RBUTTONDOWN)		//右键呼出菜单
		{
			IMAGE img;	//宽246，长328
			loadimage(&img, "./src/post_menu.png");
			putpicture(197, 156, &img, RGB(85, 85, 85), 179);
			while (1)
			{
				FlushMouseMsgBuffer();
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					if (197 >= m.x || 197 + 246 <= m.x || 156 >= m.y || 156 + 328 <= m.y)
					{
						//重绘
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)		//关闭菜单
						{
							Map[SetChessOrder[i].ShowX()][SetChessOrder[i].ShowY()] = SetChessOrder[i].ShowP();
							if (SetChessOrder[i].ShowP() == 1)
							{
								setfillcolor(BLACK);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
							else if (SetChessOrder[i].ShowP() == -1)
							{
								setfillcolor(WHITE);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
						}
						break;
					}
					if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 < m.y && m.y < 156 + 150)		//悔棋
					{
						MessageBox(NULL, "悔棋成功！", "悔棋", MB_OK);
						for (int i = 0; i < 2; ++i)
						{
							//撤销一个棋子图像，并从STL容器中删去最后一步落子记录
							Point t = SetChessOrder[(int)SetChessOrder.size() - 1];
							clearcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							setfillcolor(RGB(105, 105, 105));
							fillcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							menu.DrawLineWhite(t.ShowX(), t.ShowY());
							Map[t.ShowX()][t.ShowY()] = 0;
							SetChessOrder.pop_back();
							rounds--;
						}
						//重绘
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)
						{
							Map[SetChessOrder[i].ShowX()][SetChessOrder[i].ShowY()] = SetChessOrder[i].ShowP();
							if (SetChessOrder[i].ShowP() == 1)
							{
								setfillcolor(BLACK);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
							else if (SetChessOrder[i].ShowP() == -1)
							{
								setfillcolor(WHITE);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
						}
						break;
					}
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 < m.y && m.y < 156 + 150 + 77)		//存档
					{
						menu.SaveData(filePath1);
						//重绘
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)
						{
							Map[SetChessOrder[i].ShowX()][SetChessOrder[i].ShowY()] = SetChessOrder[i].ShowP();
							if (SetChessOrder[i].ShowP() == 1)
							{
								setfillcolor(BLACK);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
							else if (SetChessOrder[i].ShowP() == -1)
							{
								setfillcolor(WHITE);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
						}
						break;
					}
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 * 2 < m.y && m.y < 156 + 150 + 77 * 2)		//读档
					{
						menu.LoadData(filePath1);
						break;
					}
					else;
				}
			}
			continue;
		}


		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x < 20 || m.x > WWidth - 20 || m.y < 20 || m.y > GHeight - 20))
			{
				continue;
			}
			int x = (m.x - 20) / 40;
			int y = (m.y - 20) / 40;
			if (x < 0 || y < 0 || Map[x][y] != 0)
			{
				continue;
			}
			menu.DropChessBlack(x, y);
			memset(RecvBuffer, 0, sizeof(RecvBuffer));
			sprintf(RecvBuffer, "轮到电脑落子");
			menu.DrawSystemInfo();
			if (menu.JudgeWin())
			{
				if (rounds % 2 == 1)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "黑方获胜！");
					break;
				}
				else if (rounds % 2 == 0)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "白方获胜！");
					break;
				}
			}
			ai.ActionByAI(x, y);
			memset(RecvBuffer, 0, sizeof(RecvBuffer));
			sprintf(RecvBuffer, "轮到黑方落子");
			menu.DrawSystemInfo();
			if (menu.JudgeWin())
			{
				if (rounds % 2 == 1)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "黑方获胜！");
					break;
				}
				else if (rounds % 2 == 0)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "白方获胜！");
					break;
				}
			}
		}
	}
	while (1)
	{
		menu.DrawSystemInfo();
		m = GetMouseMsg();
		ret = menu.DrawButton(m.x, m.y, m.uMsg);
		if (ret == 1)
			menu.RePlayOffline();//回放
		else if (ret == -1)
			break;
		FlushBatchDraw();
	}
	exit(1);
}

void Offline()
{
	HWND hwnd = initgraph(WWidth, WHeight, 0);
	SetWindowTextA(hwnd, "五子棋");
	mciSendString("open ./src/WalkInDust.mp3 alias bgm", NULL, 0, NULL);		//循环播放背景音乐
	mciSendString("play bgm repeat", NULL, 0, NULL);
	InitGame();

	//初始化游戏数据
	SetChessOrder.clear();

	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			Map[i][j] = 0;
		}
	}

	MOUSEMSG m = { 0 };

	memset(RecvBuffer, 0, sizeof(RecvBuffer));
	sprintf(RecvBuffer, "轮到黑方落子");
	menu.DrawSystemInfo();

	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();	//获取一个鼠标消息

		if (m.uMsg == WM_RBUTTONDOWN)		//右键呼出菜单
		{
			IMAGE img;	//宽246，长328
			loadimage(&img, "./src/post_menu.png");
			putpicture(197, 156, &img, RGB(85, 85, 85), 179);
			while (1)
			{
				FlushMouseMsgBuffer();
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					if (197 >= m.x || 197 + 246 <= m.x || 156 >= m.y || 156 + 328 <= m.y)
					{
						//重绘
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)		//关闭菜单
						{
							Map[SetChessOrder[i].ShowX()][SetChessOrder[i].ShowY()] = SetChessOrder[i].ShowP();
							if (SetChessOrder[i].ShowP() == 1)
							{
								setfillcolor(BLACK);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
							else if (SetChessOrder[i].ShowP() == -1)
							{
								setfillcolor(WHITE);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
						}
						break;
					}
					if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 < m.y && m.y < 156 + 150)		//悔棋
					{
						if (menu.JudgeTakeback())
						{
							//撤销一个棋子图像，并从STL容器中删去最后一步落子记录
							MessageBox(NULL, "悔棋成功！", "悔棋", MB_OK);
							Point t = SetChessOrder[(int)SetChessOrder.size() - 1];
							clearcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							setfillcolor(RGB(105, 105, 105));
							fillcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							menu.DrawLineWhite(t.ShowX(), t.ShowY());
							Map[t.ShowX()][t.ShowY()] = 0;
							SetChessOrder.pop_back();
							rounds--;
						}
						//重绘
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)
						{
							Map[SetChessOrder[i].ShowX()][SetChessOrder[i].ShowY()] = SetChessOrder[i].ShowP();
							if (SetChessOrder[i].ShowP() == 1)
							{
								setfillcolor(BLACK);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
							else if (SetChessOrder[i].ShowP() == -1)
							{
								setfillcolor(WHITE);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
						}
						break;
					}
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 < m.y && m.y < 156 + 150 + 77)		//存档
					{
						menu.SaveData(filePath2);
						//重绘
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)
						{
							Map[SetChessOrder[i].ShowX()][SetChessOrder[i].ShowY()] = SetChessOrder[i].ShowP();
							if (SetChessOrder[i].ShowP() == 1)
							{
								setfillcolor(BLACK);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
							else if (SetChessOrder[i].ShowP() == -1)
							{
								setfillcolor(WHITE);
								solidcircle(SetChessOrder[i].ShowX() * 40 + 40, SetChessOrder[i].ShowY() * 40 + 40, PieceRadius);
							}
						}
						break;
					}
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 * 2 < m.y && m.y < 156 + 150 + 77 * 2)		//读档
					{
						menu.LoadData(filePath2);
						break;
					}
					else;
				}
			}
			continue;
		}


		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((m.x < 20 || m.x > WWidth - 20 || m.y < 20 || m.y > GHeight - 20))
			{
				continue;
			}
			int x = (m.x - 20) / 40;
			int y = (m.y - 20) / 40;
			if (x < 0 || y < 0 || Map[x][y] != 0)
			{
				continue;
			}
			//判断黑白子
			if (rounds % 2 == 0)			//黑子
			{
				menu.DropChessBlack(x, y);
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "轮到白方落子");
			}
			else if (rounds % 2 == 1)		//白子
			{
				menu.DropChessWhite(x, y);
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "轮到黑方落子");
			}
			menu.DrawSystemInfo();
		}
		if (menu.JudgeWin())
		{
			if (rounds % 2 == 1)
			{
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "黑方获胜！");
				break;
			}
			else if (rounds % 2 == 0)
			{
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "白方获胜！");
				break;
			}
		}
	}
	while (1)
	{
		menu.DrawSystemInfo();
		m = GetMouseMsg();
		ret = menu.DrawButton(m.x, m.y, m.uMsg);
		if (ret == 1)
			menu.RePlayOffline();//回放
		else if (ret == -1)
			break;
		FlushBatchDraw();
	}
	exit(1);
}

int Online()
{
	//1 确定协议版本
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		err("创建Windows异步Socket");
		return -1;
	}
	else suc("创建Windows异步Socket");
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

	//3 获取服务器协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//协议地址族
	addr.sin_addr.S_un.S_addr = inet_addr("101.5.172.59");//这里填写服务器IP地址
	addr.sin_port = htons(PORT);//服务器的端口

	//4 连接服务器
	ret = connect(serverSocket, (SOCKADDR*)&addr, sizeof addr);
	if (ret == -1)
	{
		err("连接服务器");
		//6 关闭socket
		closesocket(serverSocket);
		//7 清理协议版本信息
		WSACleanup();

		return -1;
	}
	else suc("连接服务器");

	ret = recv(serverSocket, RecvBuffer, 255, NULL);//先判断是黑方还是白方
	if (ret == 0)
		return -1;
	RecvBuffer[ret] = '\0';

	Position = 1;//默认自己先手
	ISLAUNCH = FALSE;//先手就没有落子
	if (strcmp(RecvBuffer, "-1") == 0)//判断自己先手还是后手
	{
		Position = -1;
		ISLAUNCH = TRUE;
	}

	ret = recv(serverSocket, RecvBuffer, 255, NULL);
	if (ret == 0)
		return -1;
	RecvBuffer[ret] = '\0';
	BOOL ISPOST = TRUE;		//后手则ISPOST==TRUE
	if (Position == 1)
	{
		ISPOST = FALSE;		//先手则ISPOST==FALSE
	}
	if (!InitCheckerBoard())
	{
		return -1;
	}
	HWND hwnd = initgraph(WWidth, WHeight, 0);
	SetWindowTextA(hwnd, "五子棋");
	mciSendString("open ./src/WalkInDust.mp3 alias bgm", NULL, 0, NULL);		//循环播放背景音乐
	mciSendString("play bgm repeat", NULL, 0, NULL);
	setlinecolor(WHITE);
	MOUSEMSG m = { 0 };
	BeginBatchDraw();
	while (!GameOver)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();
		menu.DrawBoardAndPiece();
		menu.DrawSystemInfo();
		FlushBatchDraw();

		if (!ISPOST)	//判断自己是否先落子
		{
			ret = recv(serverSocket, RecvBuffer, 255, NULL);//获取服务器发来的信息
			if (ret == 0)
				return -1;
			RecvBuffer[ret] = '\0';
			menu.DrawSystemInfo();
			FlushBatchDraw();
			ISPOST = TRUE;
		}
		if (!ISLAUNCH && menu.GetInput(m.x, m.y, m.uMsg))//如果没有下子就获取落子点
		{
			menu.DrawBoardAndPiece();
			FlushBatchDraw();
			if (menu.JudgeWin())
			{
				sprintf(RecvBuffer, "%d-%d-GameOver+", PX, PY);
				send(serverSocket, RecvBuffer, 255, NULL);
				sprintf(RecvBuffer, "己方获胜！");
			}
			else
			{
				sprintf(RecvBuffer, "%d-%d-Continue+", PX, PY);
				send(serverSocket, RecvBuffer, 255, NULL);
				sprintf(RecvBuffer, "轮到对方落子");
			}
			menu.DrawSystemInfo();
			FlushBatchDraw();
		}
		if (ISLAUNCH && !GameOver)
		{
			ret = recv(serverSocket, RecvBuffer, 255, NULL);//接收对方落子X
			if (ret == 0)
				return -1;
			RecvBuffer[ret] = '\0';
			int num = 0;
			char result[10] = { "\0" };
			for (; num < ret; num++)
				if (RecvBuffer[num] != '-')
				{
					SendBufferX[num] = RecvBuffer[num];
				}
				else
				{
					SendBufferX[num] = '\0';
					break;
				}
			int numL = num + 1;
			for (num = num + 1; num < ret; num++)
				if (RecvBuffer[num] != '-')
				{
					SendBufferY[num - numL] = RecvBuffer[num];
				}
				else
				{
					SendBufferY[num - numL] = '\0';
					break;
				}
			numL = num + 1;
			for (num = num + 1; num < ret; num++)
				if (RecvBuffer[num] != '+')
				{
					result[num - numL] = RecvBuffer[num];
				}
				else
				{
					result[num - numL] = '\0';
					break;
				}
			int x = atoi(SendBufferX);
			int y = atoi(SendBufferY);
			Map[x][y] = (-1) * Position;
			AddNewNode(x, y, (-1) * Position);
			if (strcmp(result, "GameOver") == 0)
			{
				GameOver = TRUE;
				sprintf(RecvBuffer, "游戏结束。对方获胜！");
			}
			else
			{
				sprintf(RecvBuffer, "轮到己方落子");
			}
			menu.DrawBoardAndPiece();
			menu.DrawSystemInfo();
			FlushBatchDraw();
			ISLAUNCH = FALSE;
		}
		menu.DrawBoardAndPiece();
		menu.DrawSystemInfo();
		FlushBatchDraw();
	}
	while (GameOver)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();
		ret = menu.DrawButton(m.x, m.y, m.uMsg);
		if (ret == 1)
			menu.RePlayOnline();//回放
		else if (ret == -1)
			break;
		FlushBatchDraw();
	}

	//6 关闭socket
	closesocket(serverSocket);
	//7 清理协议版本信息
	WSACleanup();
	exit(0);
}