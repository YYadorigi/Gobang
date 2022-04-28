#pragma once

#include"base.h"

//ChessColor枚举类仅起注释作用
enum class ChessColor
{
	NullChessColor = 0,
	Black = 1,
	White = -1,
};


class Point
{
public:
	Point(int i = 0, int j = 0, int p = 0) :x(i), y(j), P(p) {};
	Point(const Point& p) { x = p.ShowX(); y = p.ShowY(); P = p.ShowP(); }
	void Set(int i, int j, int p) { x = i; y = j; P = p; }
	virtual ~Point() {};
	const int ShowX()const { return x; }
	const int ShowY()const { return y; }
	const int ShowP()const { return P; }
	int x;
	int y;
	int P;
};

class Node :public Point
{
public:
	class Node* LAST;
	class Node* NEXT;
};

BOOL InitCheckerBoard();

BOOL AddNewNode(int x, int y, int p);

