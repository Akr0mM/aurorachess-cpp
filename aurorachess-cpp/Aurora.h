#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <cmath>

#include "Board.h"

using namespace std;

class Aurora
{
public:
	Aurora(string fen);
	list<unsigned long long> GetMoves();
	void PlayMove(unsigned long long& move);
	unsigned long long GetMoveMask(const string& move);
	unsigned long long GetSquareMask(const string& square);
	static string LongToBinary(const unsigned long long& mask);
	static unsigned long long BinaryToLong(const string& binary);
	void Load(string fen);

	string fen;
	string turn;
	string castleRights;

	unsigned long long wp;
	unsigned long long wr;
	unsigned long long wn;
	unsigned long long wb;
	unsigned long long wq;
	unsigned long long wk;
	unsigned long long bp;
	unsigned long long br;
	unsigned long long bn;
	unsigned long long bb;
	unsigned long long bq;
	unsigned long long bk;
};

