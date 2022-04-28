#include"Gobang_Class.h"
using namespace MyGobang;

//距离函数
static int d(int x1, int y1, int x2, int y2)
{
	int t1 = (int)(x1 - x2) * (int)(x1 - x2) + (int)(y1 - y2) * (int)(y1 - y2);
	int t2 = (int)pow((double)t1, 0.5);
	return t2;
}

//返回值为离最后落子距离最近的空格子坐标
static point nearestCell()
{
	std::vector<point> BlankCell;				//用以存储场上所有空格子的位置信息
	for (int i = 0; i < N; ++i)									//录入场上所有空格子的位置信息
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

//冲四
bool chongsi()
{
	//冲四---横纵、斜几个情况进行分析
	/*横向*/
	for (int i = 0; i < N; i++)
	{
		//判断当前行是否存在连续四个黑子，且该连续四个黑子的左侧或者右侧存在一个白子
		for (int j = 0; j < N; j++)
		{
			if (j <= N-4 && gobangboard.ShowCell(i,j) == 1)
			{
				//判断是否会从j开始出现连续四个
				if (gobangboard.ShowCell(i, j+1) == 1 && gobangboard.ShowCell(i, j+2) == 1 && gobangboard.ShowCell(i, j + 3) == 1)
				{
					//判断该四连的左右是否有白子或者左右已经到达边界
					if (j + 4 == N && gobangboard.ShowCell(i, j -1) == 0)
					{
						operate.dropChessWhite(i, j - 1);
						return true;//赌成功了
					}
					else if (j == 0 && gobangboard.ShowCell(i, j +4) == 0)
					{
						operate.dropChessWhite(i, j +4);
						return true;//赌成功了
					}
					else if (j < N-4 && gobangboard.ShowCell(i, j -1) == 2 && gobangboard.ShowCell(i, j + 4) == 0)
					{
						operate.dropChessWhite(i, j +4);
						return true;//赌成功了
					}
					else if (j > 0 && gobangboard.ShowCell(i, j + 4) == 2 && gobangboard.ShowCell(i, j -1) == 0)
					{
						operate.dropChessWhite(i, j - 1);
						return true;//赌成功了
					}
				}
			}
		}
	}
	/*纵向*/
	for (int i = 0; i < N; i++)
	{
		//判断当前行是否存在连续四个黑子，且该连续四个黑子的左侧或者右侧存在一个白子
		for (int j = 0; j < N; j++)
		{
			if (j <= N-4 && gobangboard.ShowCell(j, i) == 1)
			{
				//判断是否会从j开始出现连续四个
				if (gobangboard.ShowCell(j+1, i) == 1 && gobangboard.ShowCell(j+2, i) == 1 && gobangboard.ShowCell(j+3, i) == 1)
				{
					if (j == 0 && gobangboard.ShowCell(j+4, i) == 0)
					{
						operate.dropChessWhite(j+4,i);
						return true;//赌成功了
					}
					else if (j + 4 == N && gobangboard.ShowCell(j - 1, i) == 0)
					{
						operate.dropChessWhite(j - 1, i);
						return true;//赌成功了
					}
					else if (j > 0 && gobangboard.ShowCell(j +4, i) == 2 && gobangboard.ShowCell(j - 1, i) == 0)
					{
						operate.dropChessWhite(j - 1, i);
						return true;//赌成功了
					}
					else if (j < N-4 && gobangboard.ShowCell(j - 1, i) == 2 && gobangboard.ShowCell(j +4, i) == 0)
					{
						operate.dropChessWhite(j +4, i);
						return true;//赌成功了
					}
				}
			}
		}
	}
	/*左斜*/
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
							operate.dropChessWhite(i+4, j-4);
							return true;//赌成功了
						}
						else if (i == N-4 && gobangboard.ShowCell(i-1, j+1) == 0)
						{
							operate.dropChessWhite(i - 1, j+1);
							return true;//赌成功了
						}
						else if (j == 3 && gobangboard.ShowCell(i - 1, j + 1) == 0)
						{
							operate.dropChessWhite(i - 1, j + 1);
							return true;//赌成功了
						}
						else if (j == N-1 && gobangboard.ShowCell(i +4, j -4) == 0)
						{
							operate.dropChessWhite(i +4, j -4);
							return true;//赌成功了
						}
						else if (i < N-4 && gobangboard.ShowCell(i -1, j +1) == 2 && gobangboard.ShowCell(i + 4, j - 4) == 0)
						{
							operate.dropChessWhite(i + 4, j - 4);
							return true;//赌成功了
						}
						else if (i > 0 && gobangboard.ShowCell(i + 4, j - 4) == 2 && gobangboard.ShowCell(i -1, j + 1) == 0)
						{
							operate.dropChessWhite(i - 1, j + 1);
							return true;//赌成功了
						}
					}
				}
			}
		}
	}
	/*右斜*/
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
							operate.dropChessWhite(i +4, j + 4);
							return true;//赌成功了
						}
						else if (i == N-4 && gobangboard.ShowCell(i -1, j - 1) == 0)
						{
							operate.dropChessWhite(i - 1, j - 1);
							return true;//赌成功了
						}
						else if (j == N-4 && gobangboard.ShowCell(i - 1, j - 1) == 0)
						{
							operate.dropChessWhite(i - 1, j - 1);
							return true;//赌成功了
						}
						else if (j == 0 && gobangboard.ShowCell(i +4, j +4) == 0)
						{
							operate.dropChessWhite(i + 4, j + 4);
							return true;//赌成功了
						}
						else if (j < N-4 && gobangboard.ShowCell(i - 1, j - 1) == 2 && gobangboard.ShowCell(i + 4, j + 4) == 0)
						{
							operate.dropChessWhite(i + 4, j + 4);
							return true;//赌成功了
						}
						else if (i > 0 && gobangboard.ShowCell(i + 4, j + 4) == 2 && gobangboard.ShowCell(i - 1, j - 1) == 0)
						{
							operate.dropChessWhite(i-1, j-1);
							return true;//赌成功了
						}
					}
				}
			}
		}
	}
	return false;
}

//活三
bool huosan()
{
	//三连两边都有空格
	/*横向*/
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
							//必然堵右边
							operate.dropChessWhite(i , j + 3);
							return true;//赌成功了
						}
						if (j == N-4)
						{
							//必然堵左边
							operate.dropChessWhite(i , j -1);
							return true;//赌成功了
						}
					}
				}
			}
		}
	}
	/*纵向*/
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
							operate.dropChessWhite(i + 3, j );
							return true;//赌成功了
						}
						else if (i == N-4)
						{
							operate.dropChessWhite(i -1, j );
							return true;//赌成功了
						}
					}
				}
			}
		}
	}
	/*左斜*/
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
							//必然堵上边
							operate.dropChessWhite(i -1, j + 1);
							return true;//赌成功了
						}
						else
						{
							//赌下边
							operate.dropChessWhite(i + 3, j -3);
							return true;//赌成功了
						}
					}
				}
			}
		}
	}
	/*右斜*/
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
							//赌下边
							operate.dropChessWhite(i + 3, j + 3);
							return true;//赌成功了
						}
						else
						{
							//堵上边
							operate.dropChessWhite(i -1, j -1);
							return true;//赌成功了
						}
					}
				}
			}
		}
	}
	//两连空一个在有一个且两边有空格
	/*横向*/
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
						operate.dropChessWhite(i , j + 2);
						return true;//赌成功了
					}
				}
				else if (gobangboard.ShowCell(i, j + 1) == 0 && gobangboard.ShowCell(i, j +2) == 1 && gobangboard.ShowCell(i, j +3) == 1)
				{
					if (gobangboard.ShowCell(i, j - 1) == 0 && gobangboard.ShowCell(i, j +4) == 0)
					{
						operate.dropChessWhite(i , j + 1);
						return true;//赌成功了
					}
				}
			}
		}
	}
	/*纵向*/
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
						operate.dropChessWhite(i + 2, j );
						return true;//赌成功了
					}
				}
				else if (gobangboard.ShowCell(i+1, j) == 0 && gobangboard.ShowCell(i + 2, j) == 1 && gobangboard.ShowCell(i + 3, j) == 1)
				{
					if (gobangboard.ShowCell(i - 1, j) == 0 && gobangboard.ShowCell(i + 4, j) == 0)
					{
						operate.dropChessWhite(i + 1, j );
						return true;//赌成功了
					}
				}
			}
		}
	}
	/*左斜*/
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
						operate.dropChessWhite(i + 1, j -1);
						return true;//赌成功了
					}
				}
				else if (gobangboard.ShowCell(i + 1, j-1) == 1 && gobangboard.ShowCell(i + 2, j-2) == 0 && gobangboard.ShowCell(i + 3, j-3) == 1)
				{
					if (gobangboard.ShowCell(i - 1, j+1) == 0 && gobangboard.ShowCell(i + 4, j-4) == 0)
					{
						operate.dropChessWhite(i + 2, j -2);
						return true;//赌成功了
					}
				}
			}
		}
	}
	/*右斜*/
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
						operate.dropChessWhite(i + 1, j + 1);
						return true;//赌成功了
					}
				}
				else if (gobangboard.ShowCell(i+1, j+1) == 1 && gobangboard.ShowCell(i+2, j+2) == 0 && gobangboard.ShowCell(i+3, j+3) == 1)
				{
					if (gobangboard.ShowCell(i-1, j-1) == 0 && gobangboard.ShowCell(i+4, j+4) == 0)
					{
						operate.dropChessWhite(i + 2, j + 2);
						return true;//赌成功了
					}
				}
			}
		}
	}
	return false;
}

//赌对方棋局---冲四、活三、长连禁手（三三禁手、四四禁手比较麻烦，暂不处理）
bool AI::bet()
{
	//冲四---横纵、斜几个情况进行分析
	return chongsi() || huosan();
}

//找自己棋局---落子尽可能让自己赢面最大
bool AI::find()
{
	//思路：找到自己的棋子，连的最多的且可以组成五个的
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (gobangboard.ShowCell(i, j) == 2)
			{
				/*横向*/
				if (j < N - 4 && (i == 0 || gobangboard.ShowCell(i - 1, j) == 1))
				{
					//只能向右判断
					if (gobangboard.ShowCell(i, j + 1) != 1 && gobangboard.ShowCell(i, j + 2) != 1 && gobangboard.ShowCell(i, j + 3) != 1 && gobangboard.ShowCell(i, j + 4) != 1)
					{
						for (int k = j + 1; k <= j + 4; k++)
						{
							if (gobangboard.ShowCell(i, k) == 0)
							{
								operate.dropChessWhite(i , k);
								return true;//找成功了
							}
						}
					}
				}
				else if ((j > 3) && (i == N - 1 || gobangboard.ShowCell(i + 1, j) == 1))
				{
					//只能向左判断
					if (gobangboard.ShowCell(i, j - 1) != 1 && gobangboard.ShowCell(i, j - 2) != 1 && gobangboard.ShowCell(i, j - 3) != 1 && gobangboard.ShowCell(i, j - 4) != 1)
					{
						for (int k = j - 1; k >= j - 4; k--)
						{
							if (gobangboard.ShowCell(i, k) == 0)
							{
								operate.dropChessWhite(i , k);
								return true;//找成功了
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
	operate.dropChessWhite(t.ShowX(), t.ShowY());
	return true;
}





