#pragma once

#include"base.h"

class Menu
{
public:
	void DrawBoardAndPiece();
	BOOL GetInput(int MX, int MY, UINT UMSG);
	BOOL OpenPostMenu(UINT UMSG);
	void DrawSystemInfo();
	int DrawButton(int MX, int MY, UINT UMSG);
	BOOL JudgeWin();
	BOOL JudgeTakeback();
	void RePlayOnline();
	void RePlayOffline();
	void DrawLineRed(int a, int b);
	void DrawLineWhite(int a, int b);
	void DropChessBlack(int a, int b);
	void DropChessWhite(int a, int b);
	int SaveData(const char* filePath);
	int LoadData(const char* filePath);

};

