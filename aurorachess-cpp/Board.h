#pragma once
#include "Aurora.h"

using namespace std;

class Board
{
public:
	Board(string fen);

	string fen;
	string boardPosition;

	static void DrawMask(const unsigned long long& mask);
	void DrawBoard(string fenToDraw = "");
};

