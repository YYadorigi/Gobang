#include"Gobang_Class.h"
#include<fstream>
using namespace MyGobang;

static const int vertif = 0x1234abcd;		//vertif放在文件的开头和结尾用于简单判断是否正确读取数据 

extern int x , y ;			//鼠标坐标
extern int a , b ;			//棋盘列行

extern void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);		//绘制透明背景png图片，并自动去除背景
extern void initUI();		//加载初始界面
extern void initGame();		//画出棋盘

//判断获胜的存在性
bool Judge::JudgeWin(int a, int b)
{
	int i, j;
	int t = 2 - rounds % 2;	//	1 判断玩家1是否赢	2 判断玩家2是否赢
	//横向
	for (i = a - 4, j = b; i <= a; i++)
	{
		if (i >= 0 && i < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i + 1, j) && t == gobangboard.ShowCell(i + 2, j) && t == gobangboard.ShowCell(i + 3, j) && t == gobangboard.ShowCell(i + 4, j))
		{
			return 1;
		}
	}
	//纵向
	for (i = a, j = b - 4; j <= b; j++)
	{
		if (j >= 0 && j < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i, j + 1) && t == gobangboard.ShowCell(i, j + 2) && t == gobangboard.ShowCell(i, j + 3) && t == gobangboard.ShowCell(i, j + 4))
		{
			return 1;
		}
	}
	//右下
	for (i = a - 4, j = b - 4; i <= a, j <= b; i++, j++)
	{
		if (i >= 0 && i < 12 && j >= 0 && j < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i + 1, j + 1) && t == gobangboard.ShowCell(i + 2, j + 2) && t == gobangboard.ShowCell(i + 3, j + 3) && t == gobangboard.ShowCell(i + 4, j + 4))
		{
			return 1;
		}
	}
	//左下
	for (i = a - 4, j = b + 4; i <= a, j >= b; i++, j--)
	{
		if (i >= 0 && i < 12 && j >= 4 && j < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i + 1, j - 1) && t == gobangboard.ShowCell(i + 2, j - 2) && t == gobangboard.ShowCell(i + 3, j - 3) && t == gobangboard.ShowCell(i + 4, j - 4))
		{
			return 1;
		}
	}
	return 0;
}

//判断悔棋的合法性
bool MyGobang::Judge::JudgeTakeback()
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

//在坐标(a,b)处绘制十字线
void GameOperator::drawLineRed(int a, int b)
{
	setlinecolor(LIGHTRED);
	line(a * 35, 0, a * 35, 560);
	line(0, b * 35, 525, b * 35);
	setlinecolor(BLACK);
}

void GameOperator::drawLineBlack(int a, int b)
{
	setlinecolor(BLACK);
	line(a * 35, 0, a * 35, 560);
	line(0, b * 35, 525, b * 35);
}

//落子于坐标(a,b)处
void GameOperator::dropChessBlack(int a, int b)
{
	if ((int)setchessorder.size() >= 1)
	{
		drawLineBlack(setchessorder[(int)setchessorder.size() - 1].ShowX(), setchessorder[(int)setchessorder.size() - 1].ShowY());
		for (int i = 0; i < N; ++i)
		{
			if (gobangboard.ShowCell(setchessorder[(int)setchessorder.size() - 1].ShowX(), i) != 0)
			{
				if (gobangboard.ShowCell(setchessorder[(int)setchessorder.size() - 1].ShowX(), i) == 1) { setfillcolor(BLACK); solidcircle(setchessorder[(int)setchessorder.size() - 1].ShowX() * 35, i * 35, 14); }
				else if (gobangboard.ShowCell(setchessorder[(int)setchessorder.size() - 1].ShowX(), i) == 2) { setfillcolor(WHITE); solidcircle(setchessorder[(int)setchessorder.size() - 1].ShowX() * 35, i * 35, 14); }
			}
		}
		for (int j = 0; j < N; ++j)
		{
			if (gobangboard.ShowCell(j, setchessorder[(int)setchessorder.size() - 1].ShowY()) != 0)
			{
				if (gobangboard.ShowCell(j, setchessorder[(int)setchessorder.size() - 1].ShowY()) == 1) { setfillcolor(BLACK); solidcircle(j * 35, setchessorder[(int)setchessorder.size() - 1].ShowY() * 35, 14); }
				else if (gobangboard.ShowCell(j, setchessorder[(int)setchessorder.size() - 1].ShowY()) == 2) { setfillcolor(WHITE); solidcircle(j * 35, setchessorder[(int)setchessorder.size() - 1].ShowY() * 35, 14); }
			}
		}
	}
	drawLineRed(a, b);
	for (int i = 0; i < N; ++i)
	{
		if (gobangboard.ShowCell(a, i) != 0)
		{
			if (gobangboard.ShowCell(a, i) == 1) { setfillcolor(BLACK); solidcircle(a * 35, i * 35, 14); }
			else if (gobangboard.ShowCell(a, i) == 2) { setfillcolor(WHITE); solidcircle(a * 35, i * 35, 14); }
		}
	}
	for (int j = 0; j < N; ++j)
	{
		if (gobangboard.ShowCell(j, b) != 0)
		{
			if (gobangboard.ShowCell(j, b) == 1) { setfillcolor(BLACK); solidcircle(j * 35, b * 35, 14); }
			else if (gobangboard.ShowCell(j, b) == 2) { setfillcolor(WHITE); solidcircle(j * 35, b * 35, 14); }
		}
	}
	gobangboard.SetCell(a, b, 1);
	setfillcolor(BLACK);
	solidcircle(a*35, b*35, 14);
	setchessorder.emplace_back(a, b, 1);
	rounds++;
}

void GameOperator::dropChessWhite(int a, int b)
{
	if ((int)setchessorder.size() >= 1)
	{
		drawLineBlack(setchessorder[(int)setchessorder.size() - 1].ShowX(), setchessorder[(int)setchessorder.size() - 1].ShowY());
		for (int i = 0; i < N; ++i)
		{
			if (gobangboard.ShowCell(setchessorder[(int)setchessorder.size() - 1].ShowX(), i) != 0)
			{
				if (gobangboard.ShowCell(setchessorder[(int)setchessorder.size() - 1].ShowX(), i) == 1) { setfillcolor(BLACK); solidcircle(setchessorder[(int)setchessorder.size() - 1].ShowX() * 35, i * 35, 14); }
				else if (gobangboard.ShowCell(setchessorder[(int)setchessorder.size() - 1].ShowX(), i) == 2) { setfillcolor(WHITE); solidcircle(setchessorder[(int)setchessorder.size() - 1].ShowX() * 35, i * 35, 14); }
			}
		}
		for (int j = 0; j < N; ++j)
		{
			if (gobangboard.ShowCell(j, setchessorder[(int)setchessorder.size() - 1].ShowY()) != 0)
			{
				if (gobangboard.ShowCell(j, setchessorder[(int)setchessorder.size() - 1].ShowY()) == 1) { setfillcolor(BLACK); solidcircle(j * 35, setchessorder[(int)setchessorder.size() - 1].ShowY() * 35, 14); }
				else if (gobangboard.ShowCell(j, setchessorder[(int)setchessorder.size() - 1].ShowY()) == 2) { setfillcolor(WHITE); solidcircle(j * 35, setchessorder[(int)setchessorder.size() - 1].ShowY() * 35, 14); }
			}
		}
	}
	drawLineRed(a, b);
	for (int i = 0; i < N; ++i)
	{
		if (gobangboard.ShowCell(a, i) != 0)
		{
			if (gobangboard.ShowCell(a, i) == 1) { setfillcolor(BLACK); solidcircle(a * 35, i * 35, 14); }
			else if (gobangboard.ShowCell(a, i) == 2) { setfillcolor(WHITE); solidcircle(a * 35, i * 35, 14); }
		}
	}
	for (int j = 0; j < N; ++j)
	{
		if (gobangboard.ShowCell(j, b) != 0)
		{
			if (gobangboard.ShowCell(j, b) == 1) { setfillcolor(BLACK); solidcircle(j * 35, b * 35, 14); }
			else if (gobangboard.ShowCell(j, b) == 2) { setfillcolor(WHITE); solidcircle(j * 35, b * 35, 14); }
		}
	}
	gobangboard.SetCell(a, b, 2);
	setfillcolor(WHITE);
	solidcircle(a*35, b*35, 14);
	setchessorder.emplace_back(a, b, 2);
	rounds++;
}

//将vector保存到二进制文件
int GameOperator::SaveData(const char* filePath)
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

		int length = (int)setchessorder.size();
		ofile.write((char*)&length, sizeof(int));

		int totalSize = (int)setchessorder.size() * sizeof(chess);
		ofile.write((char*)&totalSize, sizeof(int));

		ofile.write((char*)&setchessorder[0], totalSize);	//注意取址方式，不能用begin() 
		ofile.write((char*)&vertif, sizeof(int));

		ofile.close();

		return 0;
	}
	else return -1;
}

//从二进制中读取之前保存的数据并还原vector和棋盘数据
int GameOperator::LoadData(const char* filePath)
{
	using namespace std;
	auto t = MessageBox(NULL, "确认读取最近一次的存档？", "读档", MB_YESNO);
	if (t == IDYES)
	{
		initGame();

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

		vector<chess>setchessorder(length);	//需要往文件里面放长度的原因

		ifile.read((char*)&setchessorder[0], totalSize);
		ifile.read((char*)&tmpVertif, sizeof(int));

		if (tmpVertif != vertif)
		{
			MessageBox(NULL, "读取存档时发生错误！", "读档", MB_OK);
			return -1;
		}

		rounds = (int)setchessorder.size();		//载入回合数

		for (size_t i = 0; i < setchessorder.size(); ++i)			//存档载入gobangboard二维数组，并打印图形
		{
			gobangboard.SetCell(setchessorder[i].ShowX(), setchessorder[i].ShowY(), setchessorder[i].ShowColor());
			if (setchessorder[i].ShowColor() == 1)
			{
				setfillcolor(BLACK);
				solidcircle(setchessorder[i].ShowX() * 35, setchessorder[i].ShowY() * 35, 14);
			}
			else if (setchessorder[i].ShowColor() == 2)
			{
				setfillcolor(WHITE);
				solidcircle(setchessorder[i].ShowX() * 35, setchessorder[i].ShowY() * 35, 14);
			}
		}

		ifile.close();

		return 0;
	}
	else return -1;
}


