#include"Gobang_Class.h"
#include<fstream>
using namespace MyGobang;

extern const int N=16;		//尽量把所有涉及棋盘大小的地方都替换为关于N的表达式，以便修改
extern int rounds=0;			//记录下棋次数

extern std::vector<MyGobang::chess> setchessorder = {};			//用于存储落子记录的STL容器
extern MyGobang::Judge judgement(true);					//定义裁判类实例对象
extern MyGobang::chessboard gobangboard(0);			//定义棋盘类实例对象
extern MyGobang::AI Ai(true);						//定义AI类实例对象

static const int vertif = 0x1234abcd;		//vertif放在文件的开头和结尾用于简单判断是否正确读取数据 
static std::string filePath = "D:/Program Files (x86)/IDE Projects/Cpp Projects/Microsoft Visual Studio/Gobang/SaveAndLoad.txt";		//文件路径

static MOUSEMSG mouse;		//定义鼠标类实例对象
static int x = 0, y = 0;			//鼠标坐标
static int a = 0, b = 0;			//棋盘列行

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

//加载初始界面
void initUI()
{
	IMAGE img1, img2, img3, img4;
	initgraph(350, 500);
	loadimage(NULL, "./src/bg_wood.jpg", 350, 500);			//加载背景贴图
	loadimage(&img1, "./src/exit.png", 40, 40);
	drawAlpha(&img1, 300, 10);
	loadimage(&img2, "./src/button.png", 280, 60);
	drawAlpha(&img2, 35, 200);
	drawAlpha(&img2, 35, 300);
	drawAlpha(&img2, 35, 400);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));
	outtextxy(75, 200, "人机对战");
	outtextxy(75, 300, "本地双人");
	outtextxy(75, 400, "联机双人");
	settextstyle(80, 0, _T("华文新魏"));
	outtextxy(55, 80, "五子棋");
}

//画出棋盘
void initGame()		 	//初始化游戏
{
	IMAGE img1, img2, img3, img4,img5;
	initgraph(660, 560);
	loadimage(NULL, "./src/bg_wood.jpg", 660, 560);			//加载背景贴图
	loadimage(&img1, "./src/pwhite.jpg", 95, 20);
	putimage(545, 109, &img1); putimage(545, 169, &img1);
	loadimage(&img2, "./src/pwhite.jpg", 95, 40);
	putimage(545, 219, &img2);
	loadimage(&img3, "./src/save.png", 100, 100);
	drawAlpha(&img3, 545, 300);
	loadimage(&img4, "./src/load.png", 80, 80);
	drawAlpha(&img4, 555, 450);
	loadimage(&img5, "./src/exit.png", 50, 50);
	drawAlpha(&img5, 590, 10);
	mciSendString("open ./src/HanYaXiShui.mp3 alias bgm", NULL, 0, NULL);		//循环播放背景音乐《寒鸦戏水》
	mciSendString("play bgm repeat", NULL, 0, NULL);
	setlinecolor(BLACK);
	for (int i = 35; i < 560; i += 35)
	{
		line(0, i, 525, i);
		line(i, 0, i, 560);
	}
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(16, 0, _T("宋体"));
	outtextxy(550, 110, "玩家1：黑棋");
	outtextxy(550, 170, "玩家2：白棋");
	settextstyle(35, 0, _T("隶书"));
	outtextxy(557, 221, "悔棋");
	settextstyle(20, 0, _T("宋体"));
	outtextxy(575, 300, "存档");
	outtextxy(575, 430, "读档");
}

//将vector保存到二进制文件
int SaveData()
{
	using namespace std;

	ofstream ofile(filePath.c_str(),ios::binary);		//自动覆盖文本

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

//从二进制中读取之前保存的数据并还原vector和棋盘数据
int LoadData()
{
	using namespace std;

	//初始化游戏数据
	setchessorder.clear();

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			gobangboard.SetCell(i, j, 0);
		}
	}

	ifstream ifile(filePath.c_str(), ios::binary);

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

//确认存档，基于SaveData()实现
void ifSave()
{
	auto t = MessageBox(NULL, "确认存档？", "存档", MB_YESNO);	//弹出一个提示框
	if (t == IDYES) { SaveData(); }
}

//读取存档，基于LoadData()实现
void ifLoad()
{
	auto t = MessageBox(NULL, "确认读取最近一次的存档？", "读档", MB_YESNO);
	if (t == IDYES)
	{
		initGame();
		LoadData();
	}
}

//本地双人下棋
void playChess1()
{
	MOUSEMSG& m = mouse;
	while (1)
	{
		m = GetMouseMsg();	//获取一个鼠标消息

		//获取离鼠标最近的点的坐标信息
		for (int i = 0; i < N+3; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (abs(m.x - i * 35) < 17 && abs(m.y - j * 35) < 17)
				{
					x = i * 35;
					y = j * 35;
					a = i;
					b = j;
				}
			}
		}

		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//点击棋盘外区域
			if (a >= N)
			{
				if ((555 <= x && x <= 635) && (310 <= y && y <= 390))
				{
					ifSave();
				}
				else if ((555 <= x && x <= 635) && (450 <= y && y <= 530))
				{
					ifLoad();
				}
				else if ((590 <= x && x <= 640) && (10 <= y && y <= 60))
				{
					auto t = MessageBox(NULL, "确认退出？", "退出游戏", MB_YESNO);	//弹出一个提示框
					if (t == IDYES)
					{
						exit(1);
					}
				}
				else if ((545 <= x && x <= 640) && (219 <= y && y <= 259))
				{
					if (judgement.JudgeTakeback())
					{
						//撤销一个棋子图像，并从STL容器中删去最后一步落子记录
						MessageBox(NULL, "悔棋成功！", "悔棋", MB_OK);
						IMAGE img_t;
						chess t = setchessorder[(int)setchessorder.size() - 1];
						clearcircle(t.ShowX() * 35, t.ShowY() * 35,14);
						loadimage(&img_t, "./src/bg_wood.jpg", 36, 36);
						drawAlpha(&img_t,t.ShowX() * 35 - 18, t.ShowY() * 35 - 18);
						line(t.ShowX() * 35 - 18, t.ShowY() * 35-18, t.ShowX() * 35 - 18, t.ShowY() * 35+18 );
						line(t.ShowX() * 35 - 18, t.ShowY() * 35 - 18, t.ShowX() * 35 + 18, t.ShowY() * 35 - 18);
						gobangboard.SetCell(t.ShowX(), t.ShowY() , 0);
						setchessorder.pop_back();
						rounds--;
					}
					else
					{
						MessageBox(NULL, "对手拒绝请求，悔棋失败！", "悔棋", MB_OK);
					}
				}
				else
				{
					MessageBox(NULL, "此处不可落子！", "提示", MB_OK);	//弹出一个提示框
				}
				continue;	//进入下一循环
			}
			//棋子重复下在一个位置上
			if (gobangboard.ShowCell(a, b) != 0)
			{
				MessageBox(NULL, "这里已有棋子，请重新选择！", "提示", MB_OK);	//弹出一个提示框
				continue;	//进入下一循环
			}
			//判断黑白子
			if (rounds % 2 == 0)			//	偶数次 黑子
			{
				setfillcolor(BLACK);
				solidcircle(x, y, 14);
				gobangboard.SetCell(a, b, 1);
				setchessorder.emplace_back(a, b,1);
			}
			else 						//	奇数次 白子
			{	
				setfillcolor(WHITE);
				solidcircle(x, y, 14);
				gobangboard.SetCell(a, b, 2);
				setchessorder.emplace_back(a, b, 2);
			}
			rounds++;
		}
		if (judgement.JudgeWin(a, b)) 
		{
			if (rounds % 2 == 0) 
			{
				MessageBox(NULL, "玩家2胜利。", "游戏结束", MB_OK);	//弹出一个提示框
				exit(0);
			}
			else 
			{
				MessageBox(NULL, "玩家1胜利。", "游戏结束", MB_OK);	//弹出一个提示框
				exit(0);
			}
		}
	}
}

//本地AI对弈
void playChess2()
{
	MOUSEMSG& m = mouse;
	while (1)
	{
		if (rounds % 2 == 0)			//	玩家走
		{
			m = GetMouseMsg();	//获取一个鼠标消息

			//获取离鼠标最近的点的坐标信息
			for (int i = 0; i < N + 3; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (abs(m.x - i * 35) < 17 && abs(m.y - j * 35) < 17)
					{
						x = i * 35;
						y = j * 35;
						a = i;
						b = j;
					}
				}
			}

			if (m.uMsg == WM_LBUTTONDOWN)
			{
				//点击棋盘外区域
				if (a >= N)
				{
					if ((555 <= x && x <= 635) && (310 <= y && y <= 390))
					{
						ifSave();
					}
					else if ((555 <= x && x <= 635) && (450 <= y && y <= 530))
					{
						ifLoad();
					}
					else if ((590 <= x && x <= 640) && (10 <= y && y <= 60))
					{
						auto t = MessageBox(NULL, "确认退出？", "退出游戏", MB_YESNO);	//弹出一个提示框
						if (t == IDYES)
						{
							exit(1);
						}
					}
					else if ((545 <= x && x <= 640) && (219 <= y && y <= 259))
					{
						//撤销一个棋子图像，并从STL容器中删去最后一步落子记录
						MessageBox(NULL, "悔棋成功！", "悔棋", MB_OK);
						for (int i = 0; i < 2; ++i)
						{
							IMAGE img_t;
							chess t = setchessorder[(int)setchessorder.size() - 1];
							clearcircle(t.ShowX() * 35, t.ShowY() * 35, 14);
							loadimage(&img_t, "./src/bg_wood.jpg", 36, 36);
							drawAlpha(&img_t, t.ShowX() * 35 - 18, t.ShowY() * 35 - 18);
							line(t.ShowX() * 35 - 18, t.ShowY() * 35 - 18, t.ShowX() * 35 - 18, t.ShowY() * 35 + 18);
							line(t.ShowX() * 35 - 18, t.ShowY() * 35 - 18, t.ShowX() * 35 + 18, t.ShowY() * 35 - 18);
							gobangboard.SetCell(t.ShowX(), t.ShowY(), 0);
							setchessorder.pop_back();
							rounds--;
						}
					}
					else
					{
						MessageBox(NULL, "此处不可落子！", "提示", MB_OK);	//弹出一个提示框
					}
					continue;	//进入下一循环
				}
				//棋子重复下在一个位置上
				if (gobangboard.ShowCell(a, b) != 0)
				{
					MessageBox(NULL, "这里已有棋子，请重新选择！", "提示", MB_OK);	//弹出一个提示框
					continue;	//进入下一循环
				}
				setfillcolor(BLACK);
				solidcircle(x, y, 14);
				gobangboard.SetCell(a, b, 1);
				setchessorder.emplace_back(a, b, 1);
				rounds++;
			}
			if (judgement.JudgeWin(a, b))
			{
				MessageBox(NULL, "玩家胜利。", "游戏结束", MB_OK);	//弹出一个提示框
				exit(0);
			}
		}
		else			//	电脑走
		{
			//思路：遍历棋盘查找对方是否存在成功的可能，如果有堵住对方；如果没有，找自己可以成功的位置
			Sleep(300);		//短暂落子延迟

			if (Ai.bet() == false)
			{
				//找自己可以成的位置；如果没有，则随机落子
				if (Ai.find() == false)
				{
					Ai.random();
				}
			}
			if (judgement.JudgeWin(setchessorder[(int)setchessorder.size() - 1].ShowX(), setchessorder[(int)setchessorder.size() - 1].ShowY()))
			{
				MessageBox(NULL, "电脑胜利。", "游戏结束", MB_OK);	//弹出一个提示框
				exit(0);
				
			}
		}
	}
}

//初始界面选择游戏模式
void ChooseMenu()
{
	while (1)
	{
		MOUSEMSG& m = mouse;
		m = GetMouseMsg();	//获取一个鼠标消息

		//获取离鼠标最近的点的坐标信息
		for (int i = 0; i < N + 3; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (abs(m.x - i * 35) < 17 && abs(m.y - j * 35) < 17)
				{
					x = i * 35;
					y = j * 35;
					a = i;
					b = j;
				}
			}
		}

		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if ((300 <= x && x <= 340) && (10 <= y && y <= 50))
			{
				auto t = MessageBox(NULL, "确认退出？", "退出游戏", MB_YESNO);	//弹出一个提示框
				if (t == IDYES)
				{
					exit(1);
				}
			}
			else if ((35 <= x && x <= 315) && (200 <= y && y <= 260))
			{
				initGame();
				playChess2();
				break;
			}
			else if ((35 <= x && x <= 315) && (300 <= y && y <= 360))
			{
				initGame();
				playChess1();
				break;
			}
			else if ((35 <= x && x <= 315) && (400 <= y && y <= 460))
			{
				MessageBox(NULL, "联网对战暂未开放！", "敬请期待", MB_OK);	//弹出一个提示框
			}
			else {};
		}
	}
}

int main() 
{
	initUI();
	ChooseMenu();

	return 0;
}


//存档功能可以进一步实现多个存档，这样一来，人机对战的存档也能和双人对战的存档彻底区分开了。
//按钮判断没必要点阵化，降低精准度；可以把判定区域做成圆角矩形，更加精确。
//自定义棋盘大小的实现。要想实现此功能，最好将棋盘类的成员变量由二维数组改为其指针。
// 玩家和AI对决时的选边实现。
// Socket网络编程的实现。
//边缘落子待优化。
//悔棋后留下的黑色边框阴影有待优化掉。
