#pragma once
#pragma comment (lib, "winmm.lib") // link with Windows MultiMedia lib
#include<iostream>
#include<vector>
#include<Graphics.h>
#include<mmsystem.h>

extern const int N ;		//尽量把所有涉及棋盘大小的地方都替换为关于N的表达式，以便修改

extern int rounds;		//记录下棋次数

//定义一个命名空间，其中封装所有需要定义的类
namespace MyGobang
{
	//ChessColor枚举类仅起注释作用
	enum class ChessColor
	{
		NullChessColor=0,
		Black = 1,
		White = 2,
	};

	//电脑AI类
	class AI
	{
	public:
		AI(bool a) :ai(a) {};
		virtual ~AI() {};
		bool bet();
		bool find();
		bool random();
	private:
		bool ai;
	};

	//裁判类，用以判断获胜的存在性和悔棋的合法性
	class Judge
	{
	public:
		Judge(bool j) :judge(j) {};
		virtual ~Judge() {};
		bool JudgeWin(int, int);
		bool JudgeTakeback();
	private:
		bool judge;
	};

	//坐标点类，成员变量为点阵式横坐标与纵坐标
	class point
	{
	public:
		point() {};
		point(int i, int j) :x(i), y(j) {};
		point(const point& p) { x = p.ShowX(); y = p.ShowY(); }
		void Set(int i, int j) { x = i; y = j; }
		virtual ~point() {};
		const int ShowX()const { return x; }
		const int ShowY()const { return y; }
	protected:
		int x;
		int y;
	};

	//坐标点类的派生类：棋子类，新增成员变量为棋子颜色，用{0,1,2}中的整数表示
	class chess:virtual public point
	{
	public:
		chess() {};
		chess(int i, int j, int cnum) :point(i, j) { color = cnum; }
		virtual ~chess() {};
		const int ShowColor()const { return color; }
	protected:
		int color;
	};

	//棋盘类，成员变量为16*16的二维数组，用以存储棋盘上的落子信息，用{0,1,2}中的整数表示
	class chessboard
	{
	public:
		chessboard(int init) 
		{ 
			for (int i = 0; i < 16; ++i)
				for (int j = 0; j < 16; ++j)
					board[i][j] = init;
		}
		const int ShowCell(const int i, const int j) const { return board[i][j]; }
		void SetCell(int i, int j, int c) { board[i][j] = c; }
	private:
		int board[16][16];
	};

	//游戏操作类
	class GameOperator
	{
	public:
		GameOperator(bool go) :gameoperator(go) {};
		virtual ~GameOperator(){}
		void drawLineRed(int, int);		//在坐标(a,b)处绘制十字线
		void drawLineBlack(int, int);
		void dropChessBlack(int, int);	//落子于坐标(a,b)处
		void dropChessWhite(int, int);
		int SaveData(const char*);		//将vector保存到二进制文件
		int LoadData(const char*);		//从二进制中读取之前保存的数据并还原vector和棋盘数据
	private:
		bool gameoperator;
	};
}

extern std::vector<MyGobang::chess> setchessorder;			//用于存储落子记录的STL容器

extern MyGobang::Judge judgement;					//定义裁判类实例对象
extern MyGobang::chessboard gobangboard;			//定义棋盘类实例对象
extern MyGobang::AI Ai;						//定义AI类实例对象
extern MyGobang::GameOperator operate;		//定义游戏操作类实例对象




