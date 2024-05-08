#pragma once
#include "Aurora.h"

using namespace std;

class Board
{
public:
	// Constructor
	Board(string fen);
	
	// Public Variables
	string fen;
	string boardPosition;

	// Public Function
	static void DrawMask(const unsigned long long& mask);
	void DrawBoard(string fenToDraw = "");
};

