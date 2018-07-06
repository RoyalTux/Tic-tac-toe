#pragma once
#include <Windows.h>
#include <string>
#include <random>
#include <ctime>
#include "resource.h"

struct Coordinates
{
	int I, J;
};
class game
{
private:
	char Map[3][3];
	wchar_t CurrentSymbol;
	Coordinates MoveAI;
	bool AI(wchar_t Symbol, int Number = 2);
	char GetEnemy();
public:
	game();
	void ClearMap();
	bool CheckWin();
	bool CheckDraw();
	char Next();
	char GetCurrentSymbol();
	void SetCurrentSymbol(wchar_t Value);
	void SetMap(int Index, wchar_t Value);
	int GetMove();
};