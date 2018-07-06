#include "game.h"

game::game()
{
	this->MoveAI.I = this->MoveAI.J = 0;
	srand(time(unsigned(NULL)));
	this->CurrentSymbol = 'O';
	this->ClearMap();
}

char game::Next()
{
	return (this->CurrentSymbol == 'O') ? 'X' : 'O';
}

void game::ClearMap()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			this->Map[i][j] = ' ';
}

bool game::CheckDraw()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->Map[i][j] == ' ')
				return false;
		}
	}
	return true;
}

bool game::CheckWin()
{
	int Counter = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->Map[i][j] == this->CurrentSymbol)
				Counter++;
			else break;
		}
		if (Counter == 3)
			return true;
		else Counter = 0;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->Map[j][i] == this->CurrentSymbol)
				Counter++;
			else break;
		}
		if (Counter == 3)
			return true;
		else Counter = 0;
	}
	for (int i = 0, j = 0; i < 3; i++, j++) {
		if (this->Map[i][j] == this->CurrentSymbol)
			Counter++;
		else break;
	}
	if (Counter == 3)
		return true;
	Counter = 0;
	for (int i = 0, j = 2; i < 3; i++, j--) {
		if (this->Map[i][j] == this->CurrentSymbol)
			Counter++;
		else break;
	}
	if (Counter == 3)
		return true;
	return false;
}

char game::GetCurrentSymbol()
{
	return this->CurrentSymbol;
}

void game::SetCurrentSymbol(wchar_t Value)
{
	this->CurrentSymbol = Value;
}

void game::SetMap(int Index, wchar_t Value)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (Index == 0)
			{
				this->Map[i][j] = Value;
				return;
			}
			else Index--;
}

int game::GetMove()
{
	int I, J, Index = 0;
	if (this->AI(this->CurrentSymbol) == true)// добить;
	{
		I = this->MoveAI.I;
		J = this->MoveAI.J;
	}
	else if (this->AI(this->GetEnemy()) == true)// предупредить;
	{
		I = this->MoveAI.I;
		J = this->MoveAI.J;
	}
	else if (this->Map[1][1] == ' ')// середина;
	{
		I = 1; J = 1;
	}
	else if (this->AI(this->CurrentSymbol, 0) == true)// пустое поле;
	{
		I = this->MoveAI.I;
		J = this->MoveAI.J;
	}
	else// рандом;
	{
		do
		{
			I = rand() % 3;
			J = rand() % 3;
		} while (this->Map[I][J] != ' ');
	}
	Map[I][J] = this->CurrentSymbol;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (I == i && J == j)
				return Index;
			else Index++;
			return -1;
}

char game::GetEnemy()
{
	return this->CurrentSymbol == 'O' ? 'X' : 'O';
}

bool game::AI(wchar_t Symbol, int Number)
{
	int Counter = 0;
	bool LastInLine = false;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->Map[i][j] == Symbol)
				Counter++;
			else if (this->Map[i][j] == ' ')
			{
				this->MoveAI.I = i;
				this->MoveAI.J = j;
				LastInLine = true;
			}
			else break;
		}
		if (Counter == Number && LastInLine == true)
			return true;
		else
		{
			Counter = 0;
			LastInLine = false;
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->Map[j][i] == Symbol)
				Counter++;
			else if (this->Map[j][i] == ' ')
			{
				this->MoveAI.I = j;
				this->MoveAI.J = i;
				LastInLine = true;
			}
			else break;
		}
		if (Counter == Number && LastInLine == true)
			return true;
		else
		{
			Counter = 0;
			LastInLine = false;
		}
	}
	for (int i = 0, j = 0; i < 3; i++, j++) {
		if (this->Map[i][j] == Symbol)
			Counter++;
		else if (this->Map[i][j] == ' ')
		{
			this->MoveAI.I = i;
			this->MoveAI.J = j;
			LastInLine = true;
		}
		else break;
	}
	if (Counter == Number && LastInLine == true)
		return true;
	Counter = 0;
	LastInLine = false;
	for (int i = 0, j = 2; i < 3; i++, j--) {
		if (this->Map[i][j] == Symbol)
			Counter++;
		else if (this->Map[i][j] == ' ')
		{
			this->MoveAI.I = i;
			this->MoveAI.J = j;
			LastInLine = true;
		}
		else break;
	}
	if (Counter == Number && LastInLine == true)
		return true;
	return false;
}