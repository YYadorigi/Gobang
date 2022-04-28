#include"linked_list.h"

extern int Map[15][15];
extern Node* HEAD, * _END;

BOOL InitCheckerBoard()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			Map[i][j] = 0;
	HEAD = (Node*)malloc(sizeof(Node));
	_END = (Node*)malloc(sizeof(Node));
	if (HEAD == NULL || _END == NULL)
		return FALSE;
	HEAD->NEXT = _END;
	_END->LAST = HEAD;
	return TRUE;
}

BOOL AddNewNode(int x, int y, int p)
{
	Node* P = _END->LAST, * Q = _END;
	Node* N = (Node*)malloc(sizeof(Node));
	if (N == NULL)
		return FALSE;
	N->x = x;
	N->y = y;
	N->P = p;
	N->LAST = P;
	N->NEXT = Q;
	P->NEXT = N;
	Q->LAST = N;
	return TRUE;
}