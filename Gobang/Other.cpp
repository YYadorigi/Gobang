#include"Gobang_Class.h"
#include<fstream>
using namespace MyGobang;

static const int vertif = 0x1234abcd;		//vertif�����ļ��Ŀ�ͷ�ͽ�β���ڼ��ж��Ƿ���ȷ��ȡ���� 

extern int x , y ;			//�������
extern int a , b ;			//��������

extern void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);		//����͸������pngͼƬ�����Զ�ȥ������
extern void initUI();		//���س�ʼ����
extern void initGame();		//��������

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
		if (t == IDYES) return true;
		else return false;
	}
	else if (rounds % 2 == 1)
	{
		auto t = MessageBox(NULL, "���1������壬�Ƿ�ͬ�⣿", "����", MB_YESNO);	//����һ����ʾ��
		if (t == IDYES) return true;
		else return false;
	}
	else return false;
}

//������(a,b)������ʮ����
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

//����������(a,b)��
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

//��vector���浽�������ļ�
int GameOperator::SaveData(const char* filePath)
{
	using namespace std;
	auto t = MessageBox(NULL, "ȷ�ϴ浵��", "�浵", MB_YESNO);	//����һ����ʾ��
	if (t == IDYES)
	{
		ofstream ofile(filePath, ios::binary);		//�Զ������ı�

		//��vector���浽�ļ�,��ʽ��4�ֽڼ�����+4�ֽ����鳤��+4�ֽ����ݳ���+�ɱ䳤�ȵ�����+4�ֽ�β��������
		if (!ofile.is_open())
		{
			MessageBox(NULL, "�浵ʧ�ܣ�", "�浵", MB_OK);
			return -1;
		}
		ofile.write((char*)&vertif, sizeof(int));

		int length = (int)setchessorder.size();
		ofile.write((char*)&length, sizeof(int));

		int totalSize = (int)setchessorder.size() * sizeof(chess);
		ofile.write((char*)&totalSize, sizeof(int));

		ofile.write((char*)&setchessorder[0], totalSize);	//ע��ȡַ��ʽ��������begin() 
		ofile.write((char*)&vertif, sizeof(int));

		ofile.close();

		return 0;
	}
	else return -1;
}

//�Ӷ������ж�ȡ֮ǰ��������ݲ���ԭvector����������
int GameOperator::LoadData(const char* filePath)
{
	using namespace std;
	auto t = MessageBox(NULL, "ȷ�϶�ȡ���һ�εĴ浵��", "����", MB_YESNO);
	if (t == IDYES)
	{
		initGame();

		ifstream ifile(filePath, ios::binary);

		int tmpVertif, length, totalSize;

		ifile.read((char*)&tmpVertif, sizeof(int));
		if (tmpVertif != vertif)
		{
			MessageBox(NULL, "��ȡ�浵ʧ�ܣ�", "����", MB_OK);
			return -1;
		}

		ifile.read((char*)&length, sizeof(int));
		ifile.read((char*)&totalSize, sizeof(int));

		vector<chess>setchessorder(length);	//��Ҫ���ļ�����ų��ȵ�ԭ��

		ifile.read((char*)&setchessorder[0], totalSize);
		ifile.read((char*)&tmpVertif, sizeof(int));

		if (tmpVertif != vertif)
		{
			MessageBox(NULL, "��ȡ�浵ʱ��������", "����", MB_OK);
			return -1;
		}

		rounds = (int)setchessorder.size();		//����غ���

		for (size_t i = 0; i < setchessorder.size(); ++i)			//�浵����gobangboard��ά���飬����ӡͼ��
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


