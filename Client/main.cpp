#include"linked_list.h"
#include"menu.h"

extern int Map[15][15];
extern std::vector<Point> SetChessOrder;

extern void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);
extern void WithAI();
extern void Offline();
extern int Online();

void InitUI()
{
	initgraph(384, 538, 0);
	loadimage(NULL, "./src/start_menu.png", 384, 538);
}

void ChooseMenu()
{
	while (1)
	{
		MOUSEMSG mouse = { 0 };
		mouse = GetMouseMsg();	//获取一个鼠标消息
		if (mouse.uMsg == WM_LBUTTONDOWN)
		{
			if (34 < mouse.x && mouse.x < 358 && 200 < mouse.y && mouse.y < 276)
			{
				WithAI();
			}
			else if (34 < mouse.x && mouse.x < 358 && 200 + 110 < mouse.y && mouse.y < 276 + 110)
			{
				Offline();
			}
			else if (34 < mouse.x && mouse.x < 358 && 200 + 110 * 2 < mouse.y && mouse.y < 276 + 110 * 2)
			{
				Online();
			}
			else {};
		}
	}
}


int main()
{
	InitUI();
	ChooseMenu();

	return 0;
}