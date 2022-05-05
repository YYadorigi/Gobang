#include"linked_list.h"
#include"menu.h"

extern int ret;
extern int Map[15][15];
extern int Position;
extern int PX, PY;
extern char RecvBuffer[256];
extern char SendBufferX[256];
extern char SendBufferY[256];
extern BOOL GameOver;
extern BOOL ISLAUNCH;				//自己是否下子
extern Node* HEAD, * _END;
extern int rounds;
extern int Map[15][15];
extern std::vector<Point> SetChessOrder;

extern BOOL InitCheckerBoard();
extern BOOL AddNewNode(int x, int y, int p);
extern void InitGame();

const char* filePath1 = "./sl/SaveAndLoad1.txt";		//文件路径1，存取本地双人对战残局
const char* filePath2 = "./sl/SaveAndLoad2.txt";		//文件路径2，存取AI对弈残局
const int vertif = 0x1234abcd;		//vertif放在文件的开头和结尾用于简单判断是否正确读取数据 

//绘制透明背景png图片，并自动去除背景
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{

	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

//绘制图片，可调节透明度
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color, int alpha)		//0~255 255表示不透明
{
	DWORD* imgp = GetImageBuffer(img);
	DWORD* bgimgp = GetImageBuffer();
	int w, bw, h, i, j;
	w = img->getwidth();
	bw = getwidth();
	h = img->getheight();
	color += 0xff000000;
	if (alpha < 0)alpha = 0;
	else if (alpha > 255)alpha = 255;
	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			if (imgp[i * w + j] != color)
			{
				bgimgp[(i + dsty) * bw + j + dstx] = RGB(
					((int)(alpha / 255.0 * GetRValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetRValue(bgimgp[(i + dsty) * bw + j + dstx]))),
					((int)(alpha / 255.0 * GetGValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetGValue(bgimgp[(i + dsty) * bw + j + dstx]))),
					((int)(alpha / 255.0 * GetBValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetBValue(bgimgp[(i + dsty) * bw + j + dstx])))
				);
			}
		}
	}
}

void Menu::DrawBoardAndPiece()//绘画棋盘棋子
{
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
	for (int j = 0; j < 15; j++)
	{
		for (int k = 0; k < 15; k++)
		{
			if (Map[j][k] == -1)			// Position == -1 对应白棋
			{
				setfillcolor(WHITE);
				solidcircle(40 + 40 * k, 40 + 40 * j, PieceRadius);
			}
			else if (Map[j][k] == 1)		// position == 1 对应黑棋
			{
				setfillcolor(BLACK);
				solidcircle(40 + 40 * k, 40 + 40 * j, PieceRadius);
			}
		}
	}
}

BOOL Menu::GetInput(int MX, int MY, UINT UMSG)
{
	if (UMSG == WM_LBUTTONDOWN)
	{
		if ((MX < 20 || MX > WWidth - 20 || MY < 20 || MY > GHeight - 20))
		{
			return FALSE;
		}
		else
		{
			int x = (MX - 20) / 40;
			int y = (MY - 20) / 40;
			if (x < 0 || y < 0)
			{
				return FALSE;
			}
			if (Map[y][x] == 0)
			{
				Map[y][x] = Position;
				PX = y;
				PY = x;
				sprintf(SendBufferX, "%d", y);
				sprintf(SendBufferY, "%d", x);
				AddNewNode(y, x, Position);
				ISLAUNCH = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL Menu::OpenPostMenu(UINT UMSG)
{
	if (UMSG == WM_RBUTTONDOWN)
	{
		return TRUE;
	}
	return FALSE;
}

void Menu::DrawSystemInfo()
{
	clearrectangle(BWidth + 1, GHeight + 1, WWidth, WHeight);
	settextstyle(20, 0, "楷体");
	outtextxy(BWidth + 10, GHeight + 45, RecvBuffer);
	return;
}

int Menu::DrawButton(int MX, int MY, UINT UMSG)//绘画按钮和系统通知。
{
	settextstyle(30, 0, "楷体");
	settextcolor(WHITE);
	outtextxy(30, GHeight + 20, "回放");
	outtextxy(30, GHeight + 60, "退出");
	if (MX > 30 && MX < 90 && MY > GHeight + 20 && MY < GHeight + 50)
	{
		settextcolor(RED);
		outtextxy(30, GHeight + 20, "回放");
		if (UMSG == WM_LBUTTONDOWN)
		{
			return 1;
		}
	}
	else if (MX > 30 && MX < 90 && MY > GHeight + 60 && GHeight + 90)
	{
		settextcolor(RED);
		outtextxy(30, GHeight + 60, "退出");
		if (UMSG == WM_LBUTTONDOWN)
		{
			return -1;
		}
	}
	return 0;
}

BOOL Menu::JudgeWin()
{
	for (int i = 2; i <= 12; i++)
		for (int j = 2; j <= 12; j++)
		{
			if (Map[i][j] != 0 && Map[i][j] == Map[i][j - 1] && Map[i][j] == Map[i][j - 2] && Map[i][j] == Map[i][j + 1] && Map[i][j] == Map[i][j + 2])
			{
				GameOver = TRUE;
				return TRUE;
			}
			if (Map[i][j] != 0 && Map[i][j] == Map[i - 1][j] && Map[i][j] == Map[i - 2][j] && Map[i][j] == Map[i + 1][j] && Map[i][j] == Map[i + 2][j])
			{
				GameOver = TRUE;
				return TRUE;
			}
			if (Map[i][j] != 0 && Map[i][j] == Map[i - 1][j - 1] && Map[i][j] == Map[i - 2][j - 2] && Map[i][j] == Map[i + 1][j + 1] && Map[i][j] == Map[i + 2][j + 2])
			{
				GameOver = TRUE;
				return TRUE;
			}
			if (Map[i][j] != 0 && Map[i][j] == Map[i + 1][j - 1] && Map[i][j] == Map[i + 2][j - 2] && Map[i][j] == Map[i - 1][j + 1] && Map[i][j] == Map[i - 2][j + 2])
			{
				GameOver = TRUE;
				return TRUE;
			}
		}
	for (int i = 2; i <= 12; i++)
	{
		if (Map[0][i] != 0 && Map[0][i] == Map[0][i - 1] && Map[0][i] == Map[0][i - 2] && Map[0][i] == Map[0][i + 1] && Map[0][i] == Map[0][i + 2])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[1][i] != 0 && Map[1][i] == Map[1][i - 1] && Map[1][i] == Map[1][i - 2] && Map[1][i] == Map[1][i + 1] && Map[1][i] == Map[1][i + 2])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[13][i] != 0 && Map[13][i] == Map[13][i - 1] && Map[13][i] == Map[13][i - 2] && Map[13][i] == Map[13][i + 1] && Map[13][i] == Map[13][i + 2])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[14][i] != 0 && Map[14][i] == Map[14][i - 1] && Map[14][i] == Map[14][i - 2] && Map[14][i] == Map[14][i + 1] && Map[14][i] == Map[14][i + 2])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[i][0] != 0 && Map[i][0] == Map[i - 1][0] && Map[i][0] == Map[i - 2][0] && Map[i][0] == Map[i + 1][0] && Map[i][0] == Map[i + 2][0])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[i][1] != 0 && Map[i][1] == Map[i - 1][1] && Map[i][1] == Map[i - 2][1] && Map[i][1] == Map[i + 1][1] && Map[i][1] == Map[i + 2][1])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[i][13] != 0 && Map[i][13] == Map[i - 1][13] && Map[i][13] == Map[i - 2][13] && Map[i][13] == Map[i + 1][13] && Map[i][13] == Map[i + 2][13])
		{
			GameOver = TRUE;
			return TRUE;
		}
		if (Map[i][14] != 0 && Map[i][14] == Map[i - 1][14] && Map[i][14] == Map[i - 2][14] && Map[i][14] == Map[i + 1][14] && Map[i][14] == Map[i + 2][14])
		{
			GameOver = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

//判断悔棋的合法性
BOOL Menu::JudgeTakeback()
{
	if (rounds % 2 == 0)
	{
		auto t = MessageBox(NULL, "玩家2申请悔棋，是否同意？", "悔棋", MB_YESNO);	//弹出一个提示框
		if (t == IDYES) return true;
		else return false;
	}
	else if (rounds % 2 == 1)
	{
		auto t = MessageBox(NULL, "玩家1申请悔棋，是否同意？", "悔棋", MB_YESNO);	//弹出一个提示框
		if (t == IDYES) return true;
		else return false;
	}
	else return false;
}


void Menu::RePlayOnline()
{
	clearrectangle(0, 0, WWidth, GHeight);
	setfillcolor(RGB(105, 105, 105));
	fillrectangle(0, 0, WWidth, GHeight);
	for (int i = 0; i < 15; i++)
	{
		line(40 + 40 * i, 40, 40 + 40 * i, GHeight - 40);
		line(40, 40 + 40 * i, WWidth - 40, 40 + 40 * i);
	}
	Node* N = HEAD->NEXT;
	while (N != _END)
	{
		if (N->P == 1)
		{
			setfillcolor(WHITE);
			solidcircle(40 + 40 * N->y, 40 + 40 * N->x, PieceRadius);
		}
		else
		{
			setfillcolor(BLACK);
			solidcircle(40 + 40 * N->y, 40 + 40 * N->x, PieceRadius);
		}
		N = N->NEXT;
		FlushBatchDraw();
		Sleep(800);
	}
}

void Menu::RePlayOffline()
{
	clearrectangle(0, 0, WWidth, GHeight);
	setfillcolor(RGB(105, 105, 105));
	fillrectangle(0, 0, WWidth, GHeight);
	for (int i = 0; i < 15; i++)
	{
		line(40 + 40 * i, 40, 40 + 40 * i, GHeight - 40);
		line(40, 40 + 40 * i, WWidth - 40, 40 + 40 * i);
	}
	for (int i = 0; i < (int)SetChessOrder.size(); ++i)
	{
		if (SetChessOrder[i].ShowP() == 1)
		{
			setfillcolor(BLACK);
			solidcircle(40 + 40 * SetChessOrder[i].ShowX(), 40 + 40 * SetChessOrder[i].ShowY(), PieceRadius);
		}
		else
		{
			setfillcolor(WHITE);
			solidcircle(40 + 40 * SetChessOrder[i].ShowX(), 40 + 40 * SetChessOrder[i].ShowY(), PieceRadius);
		}
		Sleep(800);
	}
}

void Menu::DrawLineRed(int a, int b)
{
	setlinecolor(LIGHTRED);
	line((a + 1) * 40, 40, (a + 1) * 40, GHeight - 40);
	line(40, (b + 1) * 40, WWidth - 40, (b + 1) * 40);
	setlinecolor(WHITE);
}

void Menu::DrawLineWhite(int a, int b)
{
	setlinecolor(WHITE);
	line((a + 1) * 40, 40, (a + 1) * 40, GHeight - 40);
	line(40, (b + 1) * 40, WWidth - 40, (b + 1) * 40);
}

//落子于坐标(a,b)处
void Menu::DropChessBlack(int a, int b)
{
	if ((int)SetChessOrder.size() >= 1)
	{
		DrawLineWhite(SetChessOrder[(int)SetChessOrder.size() - 1].ShowX(), SetChessOrder[(int)SetChessOrder.size() - 1].ShowY());
		for (int i = 0; i < 15; ++i)
		{
			if (Map[SetChessOrder[(int)SetChessOrder.size() - 1].ShowX()][i] != 0)
			{
				if (Map[SetChessOrder[(int)SetChessOrder.size() - 1].ShowX()][i] == 1) { setfillcolor(BLACK); solidcircle(SetChessOrder[(int)SetChessOrder.size() - 1].ShowX() * 40 + 40, (i + 1) * 40, PieceRadius); }
				else if (Map[SetChessOrder[(int)SetChessOrder.size() - 1].ShowX()][i] == -1) { setfillcolor(WHITE); solidcircle(SetChessOrder[(int)SetChessOrder.size() - 1].ShowX() * 40 + 40, (i + 1) * 40, PieceRadius); }
			}
		}
		for (int j = 0; j < 15; ++j)
		{
			if (Map[j][SetChessOrder[(int)SetChessOrder.size() - 1].ShowY()] != 0)
			{
				if (Map[j][SetChessOrder[(int)SetChessOrder.size() - 1].ShowY()] == 1) { setfillcolor(BLACK); solidcircle((j + 1) * 40, SetChessOrder[(int)SetChessOrder.size() - 1].ShowY() * 40 + 40, PieceRadius); }
				else if (Map[j][SetChessOrder[(int)SetChessOrder.size() - 1].ShowY()] == -1) { setfillcolor(WHITE); solidcircle((j + 1) * 40, SetChessOrder[(int)SetChessOrder.size() - 1].ShowY() * 40 + 40, PieceRadius); }
			}
		}
	}
	DrawLineRed(a, b);
	for (int i = 0; i < 15; ++i)
	{
		if (Map[a][i] != 0)
		{
			if (Map[a][i] == 1) { setfillcolor(BLACK); solidcircle((a + 1) * 40, (i + 1) * 40, PieceRadius); }
			else if (Map[a][i] == -1) { setfillcolor(WHITE); solidcircle((a + 1) * 40, (i + 1) * 40, PieceRadius); }
		}
	}
	for (int j = 0; j < 15; ++j)
	{
		if (Map[j][b] != 0)
		{
			if (Map[j][b] == 1) { setfillcolor(BLACK); solidcircle((j + 1) * 40, (b + 1) * 40, PieceRadius); }
			else if (Map[j][b] == -1) { setfillcolor(WHITE); solidcircle((j + 1) * 40, (b + 1) * 40, PieceRadius); }
		}
	}
	Map[a][b] = 1;
	setfillcolor(BLACK);
	solidcircle((a + 1) * 40, (b + 1) * 40, PieceRadius);
	SetChessOrder.emplace_back(a, b, 1);
	rounds++;
}

void Menu::DropChessWhite(int a, int b)
{
	if ((int)SetChessOrder.size() >= 1)
	{
		DrawLineWhite(SetChessOrder[(int)SetChessOrder.size() - 1].ShowX(), SetChessOrder[(int)SetChessOrder.size() - 1].ShowY());
		for (int i = 0; i < 15; ++i)
		{
			if (Map[SetChessOrder[(int)SetChessOrder.size() - 1].ShowX()][i] != 0)
			{
				if (Map[SetChessOrder[(int)SetChessOrder.size() - 1].ShowX()][i] == 1) { setfillcolor(BLACK); solidcircle(SetChessOrder[(int)SetChessOrder.size() - 1].ShowX() * 40 + 40, (i + 1) * 40, PieceRadius); }
				else if (Map[SetChessOrder[(int)SetChessOrder.size() - 1].ShowX()][i] == -1) { setfillcolor(WHITE); solidcircle(SetChessOrder[(int)SetChessOrder.size() - 1].ShowX() * 40 + 40, (i + 1) * 40, PieceRadius); }
			}
		}
		for (int j = 0; j < 15; ++j)
		{
			if (Map[j][SetChessOrder[(int)SetChessOrder.size() - 1].ShowY()] != 0)
			{
				if (Map[j][SetChessOrder[(int)SetChessOrder.size() - 1].ShowY()] == 1) { setfillcolor(BLACK); solidcircle((j + 1) * 40, SetChessOrder[(int)SetChessOrder.size() - 1].ShowY() * 40 + 40, PieceRadius); }
				else if (Map[j][SetChessOrder[(int)SetChessOrder.size() - 1].ShowY()] == -1) { setfillcolor(WHITE); solidcircle((j + 1) * 40, SetChessOrder[(int)SetChessOrder.size() - 1].ShowY() * 40 + 40, PieceRadius); }
			}
		}
	}
	DrawLineRed(a, b);
	for (int i = 0; i < 15; ++i)
	{
		if (Map[a][i] != 0)
		{
			if (Map[a][i] == 1) { setfillcolor(BLACK); solidcircle((a + 1) * 40, (i + 1) * 40, PieceRadius); }
			else if (Map[a][i] == -1) { setfillcolor(WHITE); solidcircle((a + 1) * 40, (i + 1) * 40, PieceRadius); }
		}
	}
	for (int j = 0; j < 15; ++j)
	{
		if (Map[j][b] != 0)
		{
			if (Map[j][b] == 1) { setfillcolor(BLACK); solidcircle((j + 1) * 40, (b + 1) * 40, PieceRadius); }
			else if (Map[j][b] == -1) { setfillcolor(WHITE); solidcircle((j + 1) * 40, (b + 1) * 40, PieceRadius); }
		}
	}
	Map[a][b] = -1;
	setfillcolor(WHITE);
	solidcircle((a + 1) * 40, (b + 1) * 40, PieceRadius);
	SetChessOrder.emplace_back(a, b, -1);
	rounds++;
}

//将vector保存到二进制文件
int Menu::SaveData(const char* filePath)
{
	using namespace std;
	auto t = MessageBox(NULL, "确认存档？", "存档", MB_YESNO);	//弹出一个提示框
	if (t == IDYES)
	{
		ofstream ofile(filePath, ios::binary);		//自动覆盖文本

		//将vector保存到文件,格式：4字节检验码+4字节数组长度+4字节数据长度+可变长度的数据+4字节尾部检验码
		if (!ofile.is_open())
		{
			MessageBox(NULL, "存档失败！", "存档", MB_OK);
			return -1;
		}
		ofile.write((char*)&vertif, sizeof(int));

		int length = (int)SetChessOrder.size();
		ofile.write((char*)&length, sizeof(int));

		int totalSize = (int)SetChessOrder.size() * sizeof(Point);
		ofile.write((char*)&totalSize, sizeof(int));

		ofile.write((char*)&SetChessOrder[0], totalSize);	//注意取址方式，不能用begin() 
		ofile.write((char*)&vertif, sizeof(int));

		ofile.close();

		return 0;
	}
	else return -1;
}

//从二进制中读取之前保存的数据并还原vector和棋盘数据
int Menu::LoadData(const char* filePath)
{
	using namespace std;
	auto t = MessageBox(NULL, "确认读取最近一次的存档？", "读档", MB_YESNO);
	if (t == IDYES)
	{
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

		ifstream ifile(filePath, ios::binary);

		int tmpVertif, length, totalSize;

		ifile.read((char*)&tmpVertif, sizeof(int));
		if (tmpVertif != vertif)
		{
			MessageBox(NULL, "读取存档失败！", "读档", MB_OK);
			return -1;
		}

		ifile.read((char*)&length, sizeof(int));
		ifile.read((char*)&totalSize, sizeof(int));

		vector<Point>setchessorder(length);	//需要往文件里面放长度的原因

		ifile.read((char*)&setchessorder[0], totalSize);
		ifile.read((char*)&tmpVertif, sizeof(int));

		if (tmpVertif != vertif)
		{
			MessageBox(NULL, "读取存档时发生错误！", "读档", MB_OK);
			return -1;
		}

		rounds = (int)setchessorder.size();		//载入回合数

		for (int i = 0; i < (int)setchessorder.size(); ++i)			//存档载入gobangboard二维数组，并打印图形
		{
			Map[setchessorder[i].ShowX()][setchessorder[i].ShowY()] = setchessorder[i].ShowP();
			if (setchessorder[i].ShowP() == 1)
			{
				setfillcolor(BLACK);
				solidcircle(setchessorder[i].ShowX() * 40 + 40, setchessorder[i].ShowY() * 40 + 40, PieceRadius);
			}
			else if (setchessorder[i].ShowP() == -1)
			{
				setfillcolor(WHITE);
				solidcircle(setchessorder[i].ShowX() * 40 + 40, setchessorder[i].ShowY() * 40 + 40, PieceRadius);
			}
		}

		ifile.close();

		return 0;
	}
	else return -1;
}
