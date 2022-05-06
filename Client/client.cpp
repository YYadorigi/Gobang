#include"linked_list.h"
#include"menu.h"
#include"AI.h"

int ret = 0;			//�����ж����ֻ��Ǻ��ֵ���ʱ����
int Map[15][15] = { 0 };	//���ڴ�������������Ϣ
int Position = 0;		//�ж����ֻ��Ǻ��֣���Ϊ1��Ϊ���֣���Ϊ-1��Ϊ����
int PX = 0, PY = 0;
char RecvBuffer[256] = { '0' };
char SendBufferX[256] = { '0' };
char SendBufferY[256] = { '0' };
BOOL GameOver = FALSE;				//�ж��Ƿ����˻�ʤ
BOOL ISLAUNCH = FALSE;				//�Լ��Ƿ�����
Node* HEAD = NULL, * _END = NULL;		//��¼ȫ������˳��
extern int rounds = 0;
Menu menu;							//ʵ��������Ϸ����
AI ai;
extern std::vector<Point> SetChessOrder = {};			//���ڴ洢���Ӽ�¼��STL����

extern const char* filePath1;		//�ļ�·��1����ȡAI���Ĳо�
extern const char* filePath2;		//�ļ�·��2����ȡ����˫�˶�ս�о�
extern const int vertif;		//vertif�����ļ��Ŀ�ͷ�ͽ�β���ڼ��ж��Ƿ���ȷ��ȡ���� 


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
	SetWindowTextA(hwnd, "������");
	mciSendString("open ./src/WalkInDust.mp3 alias bgm", NULL, 0, NULL);		//ѭ�����ű�������
	mciSendString("play bgm repeat", NULL, 0, NULL);
	InitGame();

	//��ʼ����Ϸ����
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
	sprintf(RecvBuffer, "�ֵ��ڷ�����");
	menu.DrawSystemInfo();

	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();	//��ȡһ�������Ϣ

		if (m.uMsg == WM_RBUTTONDOWN)		//�Ҽ������˵�
		{
			IMAGE img;	//��246����328
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
						//�ػ�
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)		//�رղ˵�
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
					if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 < m.y && m.y < 156 + 150)		//����
					{
						MessageBox(NULL, "����ɹ���", "����", MB_OK);
						for (int i = 0; i < 2; ++i)
						{
							//����һ������ͼ�񣬲���STL������ɾȥ���һ�����Ӽ�¼
							Point t = SetChessOrder[(int)SetChessOrder.size() - 1];
							clearcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							setfillcolor(RGB(105, 105, 105));
							fillcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							menu.DrawLineWhite(t.ShowX(), t.ShowY());
							Map[t.ShowX()][t.ShowY()] = 0;
							SetChessOrder.pop_back();
							rounds--;
						}
						//�ػ�
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
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 < m.y && m.y < 156 + 150 + 77)		//�浵
					{
						menu.SaveData(filePath1);
						//�ػ�
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
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 * 2 < m.y && m.y < 156 + 150 + 77 * 2)		//����
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
			sprintf(RecvBuffer, "�ֵ���������");
			menu.DrawSystemInfo();
			if (menu.JudgeWin())
			{
				if (rounds % 2 == 1)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "�ڷ���ʤ��");
					break;
				}
				else if (rounds % 2 == 0)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "�׷���ʤ��");
					break;
				}
			}
			ai.ActionByAI(x, y);
			memset(RecvBuffer, 0, sizeof(RecvBuffer));
			sprintf(RecvBuffer, "�ֵ��ڷ�����");
			menu.DrawSystemInfo();
			if (menu.JudgeWin())
			{
				if (rounds % 2 == 1)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "�ڷ���ʤ��");
					break;
				}
				else if (rounds % 2 == 0)
				{
					memset(RecvBuffer, 0, sizeof(RecvBuffer));
					sprintf(RecvBuffer, "�׷���ʤ��");
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
			menu.RePlayOffline();//�ط�
		else if (ret == -1)
			break;
		FlushBatchDraw();
	}
	exit(1);
}

void Offline()
{
	HWND hwnd = initgraph(WWidth, WHeight, 0);
	SetWindowTextA(hwnd, "������");
	mciSendString("open ./src/WalkInDust.mp3 alias bgm", NULL, 0, NULL);		//ѭ�����ű�������
	mciSendString("play bgm repeat", NULL, 0, NULL);
	InitGame();

	//��ʼ����Ϸ����
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
	sprintf(RecvBuffer, "�ֵ��ڷ�����");
	menu.DrawSystemInfo();

	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();	//��ȡһ�������Ϣ

		if (m.uMsg == WM_RBUTTONDOWN)		//�Ҽ������˵�
		{
			IMAGE img;	//��246����328
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
						//�ػ�
						InitGame();
						for (int i = 0; i < (int)SetChessOrder.size(); ++i)		//�رղ˵�
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
					if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 < m.y && m.y < 156 + 150)		//����
					{
						if (menu.JudgeTakeback())
						{
							//����һ������ͼ�񣬲���STL������ɾȥ���һ�����Ӽ�¼
							MessageBox(NULL, "����ɹ���", "����", MB_OK);
							Point t = SetChessOrder[(int)SetChessOrder.size() - 1];
							clearcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							setfillcolor(RGB(105, 105, 105));
							fillcircle(t.ShowX() * 40 + 40, t.ShowY() * 40 + 40, PieceRadius + 1);
							menu.DrawLineWhite(t.ShowX(), t.ShowY());
							Map[t.ShowX()][t.ShowY()] = 0;
							SetChessOrder.pop_back();
							rounds--;
						}
						//�ػ�
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
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 < m.y && m.y < 156 + 150 + 77)		//�浵
					{
						menu.SaveData(filePath2);
						//�ػ�
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
					else if (197 + 16 < m.x && m.x < 197 + 230 && 156 + 90 + 77 * 2 < m.y && m.y < 156 + 150 + 77 * 2)		//����
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
			//�жϺڰ���
			if (rounds % 2 == 0)			//����
			{
				menu.DropChessBlack(x, y);
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "�ֵ��׷�����");
			}
			else if (rounds % 2 == 1)		//����
			{
				menu.DropChessWhite(x, y);
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "�ֵ��ڷ�����");
			}
			menu.DrawSystemInfo();
		}
		if (menu.JudgeWin())
		{
			if (rounds % 2 == 1)
			{
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "�ڷ���ʤ��");
				break;
			}
			else if (rounds % 2 == 0)
			{
				memset(RecvBuffer, 0, sizeof(RecvBuffer));
				sprintf(RecvBuffer, "�׷���ʤ��");
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
			menu.RePlayOffline();//�ط�
		else if (ret == -1)
			break;
		FlushBatchDraw();
	}
	exit(1);
}

int Online()
{
	//1 ȷ��Э��汾
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		err("����Windows�첽Socket");
		return -1;
	}
	else suc("����Windows�첽Socket");
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

	//3 ��ȡ������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//Э���ַ��
	addr.sin_addr.S_un.S_addr = inet_addr("101.5.172.59");//������д������IP��ַ
	addr.sin_port = htons(PORT);//�������Ķ˿�

	//4 ���ӷ�����
	ret = connect(serverSocket, (SOCKADDR*)&addr, sizeof addr);
	if (ret == -1)
	{
		err("���ӷ�����");
		//6 �ر�socket
		closesocket(serverSocket);
		//7 ����Э��汾��Ϣ
		WSACleanup();

		return -1;
	}
	else suc("���ӷ�����");

	ret = recv(serverSocket, RecvBuffer, 255, NULL);//���ж��Ǻڷ����ǰ׷�
	if (ret == 0)
		return -1;
	RecvBuffer[ret] = '\0';

	Position = 1;//Ĭ���Լ�����
	ISLAUNCH = FALSE;//���־�û������
	if (strcmp(RecvBuffer, "-1") == 0)//�ж��Լ����ֻ��Ǻ���
	{
		Position = -1;
		ISLAUNCH = TRUE;
	}

	ret = recv(serverSocket, RecvBuffer, 255, NULL);
	if (ret == 0)
		return -1;
	RecvBuffer[ret] = '\0';
	BOOL ISPOST = TRUE;		//������ISPOST==TRUE
	if (Position == 1)
	{
		ISPOST = FALSE;		//������ISPOST==FALSE
	}
	if (!InitCheckerBoard())
	{
		return -1;
	}
	HWND hwnd = initgraph(WWidth, WHeight, 0);
	SetWindowTextA(hwnd, "������");
	mciSendString("open ./src/WalkInDust.mp3 alias bgm", NULL, 0, NULL);		//ѭ�����ű�������
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

		if (!ISPOST)	//�ж��Լ��Ƿ�������
		{
			ret = recv(serverSocket, RecvBuffer, 255, NULL);//��ȡ��������������Ϣ
			if (ret == 0)
				return -1;
			RecvBuffer[ret] = '\0';
			menu.DrawSystemInfo();
			FlushBatchDraw();
			ISPOST = TRUE;
		}
		if (!ISLAUNCH && menu.GetInput(m.x, m.y, m.uMsg))//���û�����Ӿͻ�ȡ���ӵ�
		{
			menu.DrawBoardAndPiece();
			FlushBatchDraw();
			if (menu.JudgeWin())
			{
				sprintf(RecvBuffer, "%d-%d-GameOver+", PX, PY);
				send(serverSocket, RecvBuffer, 255, NULL);
				sprintf(RecvBuffer, "������ʤ��");
			}
			else
			{
				sprintf(RecvBuffer, "%d-%d-Continue+", PX, PY);
				send(serverSocket, RecvBuffer, 255, NULL);
				sprintf(RecvBuffer, "�ֵ��Է�����");
			}
			menu.DrawSystemInfo();
			FlushBatchDraw();
		}
		if (ISLAUNCH && !GameOver)
		{
			ret = recv(serverSocket, RecvBuffer, 255, NULL);//���նԷ�����X
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
				sprintf(RecvBuffer, "��Ϸ�������Է���ʤ��");
			}
			else
			{
				sprintf(RecvBuffer, "�ֵ���������");
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
			menu.RePlayOnline();//�ط�
		else if (ret == -1)
			break;
		FlushBatchDraw();
	}

	//6 �ر�socket
	closesocket(serverSocket);
	//7 ����Э��汾��Ϣ
	WSACleanup();
	exit(0);
}