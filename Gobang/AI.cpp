#include"Gobang_Class.h"
using namespace MyGobang;

//���뺯��
static int d(int x1, int y1, int x2, int y2)
{
	int t1 = (int)(x1 - x2) * (int)(x1 - x2) + (int)(y1 - y2) * (int)(y1 - y2);
	int t2 = (int)pow((double)t1, 0.5);
	return t2;
}

//����ֵΪ��������Ӿ�������Ŀո�������
static point nearestCell()
{
	std::vector<point> BlankCell;				//���Դ洢�������пո��ӵ�λ����Ϣ
	for (int i = 0; i < N; ++i)									//¼�볡�����пո��ӵ�λ����Ϣ
	{
		for (int j = 0; j < N; ++j)
			if (gobangboard.ShowCell(i, j) == 0)
			{
				BlankCell.emplace_back(i, j);
			}
	}
	point TempCell = BlankCell[0];
	for (int i = 1; i < (int)BlankCell.size(); ++i)
	{
		int32_t d_original = d(setchessorder[(int)setchessorder.size() - 1].ShowX(), setchessorder[(int)setchessorder.size() - 1].ShowY(), TempCell.ShowX(), TempCell.ShowY());
		int32_t d_new = d(setchessorder[(int)setchessorder.size() - 1].ShowX(), setchessorder[(int)setchessorder.size() - 1].ShowY(), BlankCell[i].ShowX(), BlankCell[i].ShowY());
		if (d_new < d_original)
		{
			TempCell=BlankCell[i];
		}
	}
	return TempCell;
};

//����
bool chongsi()
{
	//����---���ݡ�б����������з���
	/*����*/
	for (int i = 0; i < N; i++)
	{
		//�жϵ�ǰ���Ƿ���������ĸ����ӣ��Ҹ������ĸ����ӵ��������Ҳ����һ������
		for (int j = 0; j < N; j++)
		{
			if (j <= N-4 && gobangboard.ShowCell(i,j) == 1)
			{
				//�ж��Ƿ���j��ʼ���������ĸ�
				if (gobangboard.ShowCell(i, j+1) == 1 && gobangboard.ShowCell(i, j+2) == 1 && gobangboard.ShowCell(i, j + 3) == 1)
				{
					//�жϸ������������Ƿ��а��ӻ��������Ѿ�����߽�
					if (j + 4 == N && gobangboard.ShowCell(i, j -1) == 0)
					{
						gobangboard.SetCell(i, j - 1,2);
						setfillcolor(WHITE);
						solidcircle(i*35, (j-1)*35, 14);
						setchessorder.emplace_back(i,j-1,2);
						rounds++;
						return true;//�ĳɹ���
					}
					else if (j == 0 && gobangboard.ShowCell(i, j +4) == 0)
					{
						gobangboard.SetCell(i, j + 4,2);
						setfillcolor(WHITE);
						solidcircle(i * 35, (j +4) * 35, 14);
						setchessorder.emplace_back(i, j +4, 2);
						rounds++;
						return true;//�ĳɹ���
					}
					else if (j < N-4 && gobangboard.ShowCell(i, j -1) == 2 && gobangboard.ShowCell(i, j + 4) == 0)
					{
						gobangboard.SetCell(i, j + 4, 2);
						setfillcolor(WHITE);
						solidcircle(i * 35, (j + 4) * 35, 14);
						setchessorder.emplace_back(i, j +4, 2);
						rounds++;
						return true;//�ĳɹ���
					}
					else if (j > 0 && gobangboard.ShowCell(i, j + 4) == 2 && gobangboard.ShowCell(i, j -1) == 0)
					{
						gobangboard.SetCell(i, j - 1, 2);
						setfillcolor(WHITE);
						solidcircle(i * 35, (j -1) * 35, 14);
						setchessorder.emplace_back(i, j - 1, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
			}
		}
	}
	/*����*/
	for (int i = 0; i < N; i++)
	{
		//�жϵ�ǰ���Ƿ���������ĸ����ӣ��Ҹ������ĸ����ӵ��������Ҳ����һ������
		for (int j = 0; j < N; j++)
		{
			if (j <= N-4 && gobangboard.ShowCell(j, i) == 1)
			{
				//�ж��Ƿ���j��ʼ���������ĸ�
				if (gobangboard.ShowCell(j+1, i) == 1 && gobangboard.ShowCell(j+2, i) == 1 && gobangboard.ShowCell(j+3, i) == 1)
				{
					if (j == 0 && gobangboard.ShowCell(j+4, i) == 0)
					{
						gobangboard.SetCell(j+4, i,2);
						setfillcolor(WHITE);
						solidcircle((j+4)*35, i*35, 14);
						setchessorder.emplace_back(j+4,i, 2);
						rounds++;
						return true;//�ĳɹ���
					}
					else if (j + 4 == N && gobangboard.ShowCell(j - 1, i) == 0)
					{
						gobangboard.SetCell(j - 1, i,2);
						setfillcolor(WHITE);
						solidcircle((j - 1) * 35, i * 35, 14);
						setchessorder.emplace_back(j -1, i, 2);
						rounds++;
						return true;//�ĳɹ���
					}
					else if (j > 0 && gobangboard.ShowCell(j +4, i) == 2 && gobangboard.ShowCell(j - 1, i) == 0)
					{
						gobangboard.SetCell(j - 1, i, 2);
						setfillcolor(WHITE);
						solidcircle((j - 1 ) * 35, i * 35, 14);
						setchessorder.emplace_back(j -1, i, 2);
						rounds++;
						return true;//�ĳɹ���
					}
					else if (j < N-4 && gobangboard.ShowCell(j - 1, i) == 2 && gobangboard.ShowCell(j +4, i) == 0)
					{
						gobangboard.SetCell(j +4, i,2);
						setfillcolor(WHITE);
						solidcircle((j + 4) * 35, i * 35, 14);
						setchessorder.emplace_back(j + 4, i, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
			}
		}
	}
	/*��б*/
	for (int i = 0; i < N-3; i++)
	{
		for (int j = 3; j < N; j++)
		{
			if (!((i == 0 && j == 3) || (i == N-4 && j == N-1)))
			{
				if (gobangboard.ShowCell(i,j) == 1)
				{
					if (gobangboard.ShowCell(i+1, j-1) == 1 && gobangboard.ShowCell(i+2, j-2) == 1 && gobangboard.ShowCell(i+3, j-3) == 1)
					{
						if (i == 0 && gobangboard.ShowCell(i+4, j-4) == 0)
						{
							gobangboard.SetCell(i+4, j-4,2);
							setfillcolor(WHITE);
							solidcircle((i + 4) * 35, (j-4) * 35, 14);
							setchessorder.emplace_back(i+ 4, j-4, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (i == N-4 && gobangboard.ShowCell(i-1, j+1) == 0)
						{
							gobangboard.SetCell(i - 1, j + 1,2);
							setfillcolor(WHITE);
							solidcircle((i-1) * 35, (j+1) * 35, 14);
							setchessorder.emplace_back(i -1, j +1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (j == 3 && gobangboard.ShowCell(i - 1, j + 1) == 0)
						{
							gobangboard.SetCell(i - 1, j + 1,2);
							setfillcolor(WHITE);
							solidcircle((i-1) * 35, (j+1) * 35, 14);
							setchessorder.emplace_back(i -1, j +1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (j == N-1 && gobangboard.ShowCell(i +4, j -4) == 0)
						{
							gobangboard.SetCell(i + 4, j - 4,2);
							setfillcolor(WHITE);
							solidcircle((i + 4) * 35, (j-4) * 35, 14);
							setchessorder.emplace_back(i + 4, j - 4, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (i < N-4 && gobangboard.ShowCell(i -1, j +1) == 2 && gobangboard.ShowCell(i + 4, j - 4) == 0)
						{
							gobangboard.SetCell(i + 4, j - 4,2);
							setfillcolor(WHITE);
							solidcircle((i + 4) * 35, (j-4) * 35, 14);
							setchessorder.emplace_back(i + 4, j - 4, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (i > 0 && gobangboard.ShowCell(i + 4, j - 4) == 2 && gobangboard.ShowCell(i -1, j + 1) == 0)
						{
							gobangboard.SetCell(i - 1, j + 1, 2);
							setfillcolor(WHITE);
							solidcircle((i-1) * 35, (j+1) * 35, 14);
							setchessorder.emplace_back(i -1, j +1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
					}
				}
			}
		}
	}
	/*��б*/
	for (int i = 0; i < N-3; i++)
	{
		for (int j = 0; j < N-3; j++)
		{
			if (!(i == 0 && j == N-4) || (i == N-4 && j == 0))
			{
				if (gobangboard.ShowCell(i , j ) == 1)
				{
					if (gobangboard.ShowCell(i + 1, j +1) == 1 && gobangboard.ShowCell(i + 2, j +2) == 1 && gobangboard.ShowCell(i + 3, j +3) == 1)
					{
						if (i == 0 && gobangboard.ShowCell(i + 4, j + 4) == 0)
						{
							gobangboard.SetCell(i + 4, j + 4, 2);
							setfillcolor(WHITE);
							solidcircle((i +4) * 35, (j + 4) * 35, 14);
							setchessorder.emplace_back(i + 4, j + 4, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (i == N-4 && gobangboard.ShowCell(i -1, j - 1) == 0)
						{
							gobangboard.SetCell(i - 1, j - 1, 2);
							setfillcolor(WHITE);
							solidcircle((i - 1) * 35, (j - 1) * 35, 14);
							setchessorder.emplace_back(i - 1, j - 1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (j == N-4 && gobangboard.ShowCell(i - 1, j - 1) == 0)
						{
							gobangboard.SetCell(i - 1, j - 1, 2);
							setfillcolor(WHITE);
							solidcircle((i - 1) * 35, (j - 1) * 35, 14);
							setchessorder.emplace_back(i - 1, j - 1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (j == 0 && gobangboard.ShowCell(i +4, j +4) == 0)
						{
							gobangboard.SetCell(i +4, j +4, 2);
							setfillcolor(WHITE);
							solidcircle((i +4) * 35, (j + 4) * 35, 14);
							setchessorder.emplace_back(i + 4, j + 4, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (j < N-4 && gobangboard.ShowCell(i - 1, j - 1) == 2 && gobangboard.ShowCell(i + 4, j + 4) == 0)
						{
							gobangboard.SetCell(i + 4, j + 4, 2);
							setfillcolor(WHITE);
							solidcircle((i + 4) * 35, (j + 4) * 35, 14);
							setchessorder.emplace_back(i + 4, j + 4, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (i > 0 && gobangboard.ShowCell(i + 4, j + 4) == 2 && gobangboard.ShowCell(i - 1, j - 1) == 0)
						{
							gobangboard.SetCell(i - 1, j - 1, 2);
							setfillcolor(WHITE);
							solidcircle((i - 1) * 35, (j - 1) * 35, 14);
							setchessorder.emplace_back(i -1, j - 1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
					}
				}
			}
		}
	}
	return false;
}

//����
bool huosan()
{
	//�������߶��пո�
	/*����*/
	for (int i = 0; i < N; i++)
	{
		for (int j = 1; j < N-3; j++)
		{
			if (gobangboard.ShowCell(i, j) == 1)
			{
				if (gobangboard.ShowCell(i, j+1) == 1 && gobangboard.ShowCell(i, j+2) == 1)
				{
					if (gobangboard.ShowCell(i, j-1) == 0 && gobangboard.ShowCell(i, j+3) == 0)
					{
						if (j == 1 || j != N-4)
						{
							//��Ȼ���ұ�
							gobangboard.SetCell(i, j + 3, 2);
							setfillcolor(WHITE);
							solidcircle(i * 35, (j +3) * 35, 14);
							setchessorder.emplace_back(i , j +3, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						if (j == N-4)
						{
							//��Ȼ�����
							gobangboard.SetCell(i, j - 1, 2);
							setfillcolor(WHITE);
							solidcircle(i* 35, (j - 1) * 35, 14);
							setchessorder.emplace_back(i , j - 1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
					}
				}
			}
		}
	}
	/*����*/
	for (int j = 0; j < N; j++)
	{
		for (int i = 1; i < N-3; i++)
		{
			if (gobangboard.ShowCell(i, j ) == 1)
			{
				if (gobangboard.ShowCell(i+1, j) == 1 && gobangboard.ShowCell(i+2, j) == 1)
				{
					if (gobangboard.ShowCell(i-1, j) == 0 && gobangboard.ShowCell(i+3, j) == 0)
					{
						if (i == 1 || i != N-4)
						{
							gobangboard.SetCell(i+3, j,2 );
							setfillcolor(WHITE);
							solidcircle((i+3) * 35, j * 35, 14);
							setchessorder.emplace_back(i + 3, j , 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else if (i == N-4)
						{
							gobangboard.SetCell(i-1, j,2);
							setfillcolor(WHITE);
							solidcircle((i - 1) * 35, j * 35, 14);
							setchessorder.emplace_back(i -1, j, 2);
							rounds++;
							return true;//�ĳɹ���
						}
					}
				}
			}
		}
	}
	/*��б*/
	for (int i = 1; i < N-3; i++)
	{
		for (int j = 3; j < N-1; j++)
		{
			if (gobangboard.ShowCell(i, j ) == 1)
			{
				if (gobangboard.ShowCell(i+1, j - 1) == 1 && gobangboard.ShowCell(i+2, j - 2) == 1)
				{
					if (gobangboard.ShowCell(i-1, j + 1) == 0 && gobangboard.ShowCell(i+3, j - 3) == 0)
					{
						if (i == 12)
						{
							//��Ȼ���ϱ�
							gobangboard.SetCell(i-1, j + 1,2);
							setfillcolor(WHITE);
							solidcircle((i-1) * 35, (j + 1) * 35, 14);
							setchessorder.emplace_back(i -1, j +1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else
						{
							//���±�
							gobangboard.SetCell(i + 3, j - 3, 2);
							setfillcolor(WHITE);
							solidcircle((i+3) * 35, (j - 3) * 35, 14);
							setchessorder.emplace_back(i + 3, j - 3, 2);
							rounds++;
							return true;//�ĳɹ���
						}
					}
				}
			}
		}
	}
	/*��б*/
	for (int i = 1; i < N-3; i++)
	{
		for (int j = 1; j < N-3; j++)
		{
			if (gobangboard.ShowCell(i, j ) == 1)
			{
				if (gobangboard.ShowCell(i+1, j + 1) == 1 && gobangboard.ShowCell(i+2, j +2) == 1)
				{
					if (gobangboard.ShowCell(i+3, j +3) == 0 && gobangboard.ShowCell(i-1, j - 1) == 0)
					{
						if (i == 1)
						{
							//���±�
							gobangboard.SetCell(i+3, j +3, 2);
							setfillcolor(WHITE);
							solidcircle((i+3) * 35, (j +3) * 35, 14);
							setchessorder.emplace_back(i + 3, j +3, 2);
							rounds++;
							return true;//�ĳɹ���
						}
						else
						{
							//���ϱ�
							gobangboard.SetCell(i - 1, j - 1, 2);
							setfillcolor(WHITE);
							solidcircle((i-1) * 35, (j - 1) * 35, 14);
							setchessorder.emplace_back(i -1, j - 1, 2);
							rounds++;
							return true;//�ĳɹ���
						}
					}
				}
			}
		}
	}
	//������һ������һ���������пո�
	/*����*/
	for (int i = 0; i < N; i++)
	{
		for (int j = 1; j < N-4; j++)
		{
			if (gobangboard.ShowCell(i, j) == 1)
			{
				if (gobangboard.ShowCell(i, j + 1) == 1 && gobangboard.ShowCell(i, j +2) == 0 && gobangboard.ShowCell(i, j +3) == 1)
				{
					if (gobangboard.ShowCell(i, j - 1) == 0 && gobangboard.ShowCell(i, j +4) == 0)
					{
						gobangboard.SetCell(i, j + 2, 2);
						setfillcolor(WHITE);
						solidcircle(i * 35, (j +2) * 35, 14);
						setchessorder.emplace_back(i , j +2, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
				else if (gobangboard.ShowCell(i, j + 1) == 0 && gobangboard.ShowCell(i, j +2) == 1 && gobangboard.ShowCell(i, j +3) == 1)
				{
					if (gobangboard.ShowCell(i, j - 1) == 0 && gobangboard.ShowCell(i, j +4) == 0)
					{
						gobangboard.SetCell(i, j + 1, 2);
						setfillcolor(WHITE);
						solidcircle(i * 35, (j + 1) * 35, 14);
						setchessorder.emplace_back(i, j+1, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
			}
		}
	}
	/*����*/
	for (int j = 0; j < N; j++)
	{
		for (int i = 1; i < N-4; i++)
		{
			if (gobangboard.ShowCell(i, j) == 1)
			{
				if (gobangboard.ShowCell(i+1, j ) == 1 && gobangboard.ShowCell(i+2, j) == 0 && gobangboard.ShowCell(i+3, j) == 1)
				{
					if (gobangboard.ShowCell(i-1, j) == 0 && gobangboard.ShowCell(i+4, j) == 0)
					{
						gobangboard.SetCell(i + 2, j, 2);
						setfillcolor(WHITE);
						solidcircle((i+2) * 35, j* 35, 14);
						setchessorder.emplace_back(i + 2, j, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
				else if (gobangboard.ShowCell(i+1, j) == 0 && gobangboard.ShowCell(i + 2, j) == 1 && gobangboard.ShowCell(i + 3, j) == 1)
				{
					if (gobangboard.ShowCell(i - 1, j) == 0 && gobangboard.ShowCell(i + 4, j) == 0)
					{
						gobangboard.SetCell(i + 1, j, 2);
						setfillcolor(WHITE);
						solidcircle((i+1) * 35, j* 35, 14);
						setchessorder.emplace_back(i + 1, j , 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
			}
		}
	}
	/*��б*/
	for (int i = 1; i < N-4; i++)
	{
		for (int j = 4; j < N-1; j++)
		{
			if (gobangboard.ShowCell(i, j) == 1)
			{
				if (gobangboard.ShowCell(i + 1, j-1) == 0 && gobangboard.ShowCell(i + 2, j-2) == 1 && gobangboard.ShowCell(i + 3, j-3) == 1)
				{
					if (gobangboard.ShowCell(i - 1, j+1) == 0 && gobangboard.ShowCell(i + 4, j-4) == 0)
					{
						gobangboard.SetCell(i + 1, j - 1, 2);
						setfillcolor(WHITE);
						solidcircle((i+1) * 35, (j - 1) * 35, 14);
						setchessorder.emplace_back(i + 1, j - 1, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
				else if (gobangboard.ShowCell(i + 1, j-1) == 1 && gobangboard.ShowCell(i + 2, j-2) == 0 && gobangboard.ShowCell(i + 3, j-3) == 1)
				{
					if (gobangboard.ShowCell(i - 1, j+1) == 0 && gobangboard.ShowCell(i + 4, j-4) == 0)
					{
						gobangboard.SetCell(i + 2, j - 2, 2);
						setfillcolor(WHITE);
						solidcircle((i+2) * 35, (j - 2) * 35, 14);
						setchessorder.emplace_back(i + 2, j - 2, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
			}
		}
	}
	/*��б*/
	for (int i = 1; i < N-4; i++)
	{
		for (int j = 1; j < N-4; j++)
		{
			if (gobangboard.ShowCell(i , j) == 1)
			{
				if (gobangboard.ShowCell(i + 1, j+1) == 0 && gobangboard.ShowCell(i + 2, j+2) == 1 && gobangboard.ShowCell(i + 3, j+3) == 1)
				{
					if (gobangboard.ShowCell(i-1, j-1) == 0 && gobangboard.ShowCell(i+4, j+4) == 0)
					{
						gobangboard.SetCell(i + 1, j + 1, 2);
						setfillcolor(WHITE);
						solidcircle((i+1) * 35, (j + 1) * 35, 14);
						setchessorder.emplace_back(i + 1, j +1, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
				else if (gobangboard.ShowCell(i+1, j+1) == 1 && gobangboard.ShowCell(i+2, j+2) == 0 && gobangboard.ShowCell(i+3, j+3) == 1)
				{
					if (gobangboard.ShowCell(i-1, j-1) == 0 && gobangboard.ShowCell(i+4, j+4) == 0)
					{
						gobangboard.SetCell(i + 2, j + 2, 2);
						setfillcolor(WHITE);
						solidcircle((i+2) * 35, (j +2) * 35, 14);
						setchessorder.emplace_back(i + 2, j +2, 2);
						rounds++;
						return true;//�ĳɹ���
					}
				}
			}
		}
	}
	return false;
}

//�ĶԷ����---���ġ��������������֣��������֡����Ľ��ֱȽ��鷳���ݲ�����
bool AI::bet()
{
	//����---���ݡ�б����������з���
	return chongsi() || huosan();
}

//���Լ����---���Ӿ��������Լ�Ӯ�����
bool AI::find()
{
	//˼·���ҵ��Լ������ӣ����������ҿ�����������
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (gobangboard.ShowCell(i, j) == 2)
			{
				/*����*/
				if (j < N - 4 && (i == 0 || gobangboard.ShowCell(i - 1, j) == 1))
				{
					//ֻ�������ж�
					if (gobangboard.ShowCell(i, j + 1) != 1 && gobangboard.ShowCell(i, j + 2) != 1 && gobangboard.ShowCell(i, j + 3) != 1 && gobangboard.ShowCell(i, j + 4) != 1)
					{
						for (int k = j + 1; k <= j + 4; k++)
						{
							if (gobangboard.ShowCell(i, k) == 0)
							{
								gobangboard.SetCell(i, k, 2);
								setfillcolor(WHITE);
								solidcircle(i * 35, k * 35, 14);
								setchessorder.emplace_back(i, k, 2);
								rounds++;
								return true;//�ҳɹ���
							}
						}
					}
				}
				else if ((j > 3) && (i == N - 1 || gobangboard.ShowCell(i + 1, j) == 1))
				{
					//ֻ�������ж�
					if (gobangboard.ShowCell(i, j - 1) != 1 && gobangboard.ShowCell(i, j - 2) != 1 && gobangboard.ShowCell(i, j - 3) != 1 && gobangboard.ShowCell(i, j - 4) != 1)
					{
						for (int k = j - 1; k >= j - 4; k--)
						{
							if (gobangboard.ShowCell(i, k) == 0)
							{
								gobangboard.SetCell(i, k, 2);
								setfillcolor(WHITE);
								solidcircle(i * 35, k * 35, 14);
								setchessorder.emplace_back(i, k, 2);
								rounds++;
								return true;//�ҳɹ���
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool AI::random()
{
	point t(nearestCell());
	gobangboard.SetCell(t.ShowX(), t.ShowY(), 2);
	setfillcolor(WHITE);
	solidcircle(t.ShowX() * 35, t.ShowY() * 35, 14);
	setchessorder.emplace_back(t.ShowX(), t.ShowY(), 2);
	rounds++;
	return true;
}





