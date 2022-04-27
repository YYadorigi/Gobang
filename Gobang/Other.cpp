#include"Gobang_Class.h"
using namespace MyGobang;

//�жϻ�ʤ�Ĵ�����
bool Judge::JudgeWin(int a, int b)
{
	int i, j;
	int t = 2 - rounds % 2;	//	1 �ж����1�Ƿ�Ӯ	2 �ж����2�Ƿ�Ӯ
	//����
	for (i = a - 4, j = b; i <= a; i++)
	{
		if (i >= 0 && i < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i + 1, j) && t == gobangboard.ShowCell(i + 2, j) && t == gobangboard.ShowCell(i + 3, j) && t == gobangboard.ShowCell(i + 4, j))
		{
			return 1;
		}
	}
	//����
	for (i = a, j = b - 4; j <= b; j++)
	{
		if (j >= 0 && j < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i, j + 1) && t == gobangboard.ShowCell(i, j + 2) && t == gobangboard.ShowCell(i, j + 3) && t == gobangboard.ShowCell(i, j + 4))
		{
			return 1;
		}
	}
	//����
	for (i = a - 4, j = b - 4; i <= a, j <= b; i++, j++)
	{
		if (i >= 0 && i < 12 && j >= 0 && j < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i + 1, j + 1) && t == gobangboard.ShowCell(i + 2, j + 2) && t == gobangboard.ShowCell(i + 3, j + 3) && t == gobangboard.ShowCell(i + 4, j + 4))
		{
			return 1;
		}
	}
	//����
	for (i = a - 4, j = b + 4; i <= a, j >= b; i++, j--)
	{
		if (i >= 0 && i < 12 && j >= 4 && j < 12 && t == gobangboard.ShowCell(i, j) && t == gobangboard.ShowCell(i + 1, j - 1) && t == gobangboard.ShowCell(i + 2, j - 2) && t == gobangboard.ShowCell(i + 3, j - 3) && t == gobangboard.ShowCell(i + 4, j - 4))
		{
			return 1;
		}
	}
	return 0;
}

//�жϻ���ĺϷ���
bool MyGobang::Judge::JudgeTakeback()
{
	if (rounds % 2 == 0)
	{
		auto t = MessageBox(NULL, "���2������壬�Ƿ�ͬ�⣿", "����", MB_YESNO);	//����һ����ʾ��
		if (t == IDNO) return false;
		else if (t == IDYES) return true;
	}
	else if (rounds % 2 == 1)
	{
		auto t = MessageBox(NULL, "���1������壬�Ƿ�ͬ�⣿", "����", MB_YESNO);	//����һ����ʾ��
		if (t == IDNO) return false;
		else if (t == IDYES) return true;
	}
}