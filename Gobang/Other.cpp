#include"Gobang_Class.h"
using namespace MyGobang;

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
		if (t == IDNO) return false;
		else if (t == IDYES) return true;
	}
	else if (rounds % 2 == 1)
	{
		auto t = MessageBox(NULL, "玩家1申请悔棋，是否同意？", "悔棋", MB_YESNO);	//弹出一个提示框
		if (t == IDNO) return false;
		else if (t == IDYES) return true;
	}
}