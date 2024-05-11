#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <list>
#include <bitset>
#include <cmath>

#include "Board.h"

using namespace std;

// Move Structure
struct Move {
	unsigned long long* piece;
	unsigned long long mask;

	unsigned long long capture;

	unsigned long long promotion;
	unsigned long long* promotionPiece;

	unsigned long long enPassantCapture;
	unsigned long long* enPassantPiece;
};

class Aurora
{
public:
	// Constructor 
	Aurora(string fen);
	void Load(string fen);
	void MakeMove(Move move);
	unsigned long long GetMoveMask(const string& move);
	unsigned long long GetSquareMask(const string& square);

	// GetMoves
	list<Move> GetMoves();
	list<Move> GetWhiteMoves(unsigned long long notWhitePieces, unsigned long long blackPieces, unsigned long long empty);
	list<Move> GetMovesWP(unsigned long long blackPieces, unsigned long long empty);
	list<Move> GetBlackMoves(unsigned long long notBlackPieces, unsigned long long whitePieces, unsigned long long empty);
	list<Move> GetMovesBP(unsigned long long whitePieces, unsigned long long empty);
	list<Move> GetKnightMoves(unsigned long long bb, unsigned long long notMyPieces, unsigned long long oppPieces);

	// Conversion
	static unsigned long long BinaryToLong(const string& binary);
	unsigned long long* GetWhitePiece(unsigned long long mask);
	unsigned long long* GetBlackPiece(unsigned long long mask);
	int NumberOfTrailingZeros(unsigned long long bb);

	// Game States
	string fen, castling, halfMoveClock, fullMoveNumber;
	unsigned long long enPassant = 0ULL;
	bool turn;

	// Bitboards
	unsigned long long wp, wr, wn, wb, wq, wk, bp, br, bn, bb, bq, bk;
	const unsigned long long GetAllWhitePieces();
	const unsigned long long GetAllBlackPieces();
	const unsigned long long GetAllPieces();

	// Mask
	const static unsigned long long FILE_A = 0x8080808080808080;
	const static unsigned long long FILE_H = 0x101010101010101;
	const static unsigned long long FILE_AB = 0xc0c0c0c0c0c0c0c0;
	const static unsigned long long FILE_GH = 0x303030303030303;
	const static unsigned long long RANK_1 = 0xff;
	const static unsigned long long RANK_4 = 0xff000000;
	const static unsigned long long RANK_5 = 0xff00000000;
	const static unsigned long long RANK_8 = 0xff00000000000000;
	const static unsigned long long CENTER = 0x1818000000;
	const static unsigned long long EXTENDED_CENTER = 0x3c3c3c3c0000;
	const static unsigned long long KING_SIDE = 0x0f0f0f0f0f0f0f0f;
	const static unsigned long long QUEEN_SIDE = 0xf0f0f0f0f0f0f0f0;
	const static unsigned long long KNIGHT_SPAN = 0xa1100110a;
	const static unsigned long long KING_SPAN = 0x70507;
};

