#include "Aurora.h"

Aurora::Aurora(string fen) : fen(fen) {
	Load(fen);
}

list<unsigned long long> Aurora::GetMoves() {
	list<unsigned long long> moves;

	return moves;
}


void Aurora::PlayMove(unsigned long long& move) {
	unsigned long long* bitboards[12] = { &wp, &bp, &wn, &bn, &wb, &bb, &wq, &bq, &wr, &br, &wk, &bk };

	for (int i = 0; i < 12; ++i) {
		if (*bitboards[i] & move) {
			*bitboards[i] ^= move;
		}
	}
}

unsigned long long Aurora::GetMoveMask(const string& move) {
	unsigned long long piece = GetSquareMask(move.substr(0, 2));
	unsigned long long target = GetSquareMask(move.substr(2));

	return piece | target;
}

unsigned long long Aurora::GetSquareMask(const string& square) {
	int col = 7 - (square[0] - 'a');
	int row = square[1] - '9';

	return 1ULL << (row * 8 + col);
}

void Aurora::Load(string fen) {
	cout << "Loading " << fen << endl;

	size_t fenBoardIndex = fen.find(' ');
	size_t turnIndex = fen.find(' ', fenBoardIndex);
	size_t castleRightsIndex = fen.find(' ', turnIndex + 1);

	// demander a chatgpt d'optimiser pour fetch le fen
	string fenBoard = fen.substr(0, fenBoardIndex);
	turn = fen.substr(turnIndex + 1, 1);
	castleRights = fen.substr(castleRightsIndex + 1, 4);

	unordered_map<char, string> bitboardsChar = {
		{'P', "wp"}, {'R', "wr"}, {'B', "wb"}, {'N', "wn"}, {'Q', "wq"}, {'K', "wk"},
		{'p', "bp"}, {'r', "br"}, {'n', "bn"}, {'b', "bb"}, {'q', "bq"}, {'k', "bk"}
	};

	unordered_map<string, string> bitboards;
	for (const auto& entry : bitboardsChar) {
		bitboards[entry.second] = "";
	}

	for (char c : fenBoard) {
		if (c != '/') {
			if (isdigit(c)) {
				for (int i = 0; i < (c - '0'); ++i) {
					for (auto& entry : bitboards) {
						entry.second += '0';
					}
				}
			}
			else {
				auto it = bitboardsChar.find(c);
				if (it != bitboardsChar.end()) {
					for (auto& entry : bitboards) {
						if (entry.first == it->second) {
							entry.second += '1';
						}
						else {
							entry.second += '0';
						}
					}
				}
			}
		}
	}

	wp = BinaryToLong(bitboards["wp"]);
	wr = BinaryToLong(bitboards["wr"]);
	wn = BinaryToLong(bitboards["wn"]);
	wb = BinaryToLong(bitboards["wb"]);
	wq = BinaryToLong(bitboards["wq"]);
	wk = BinaryToLong(bitboards["wk"]);
	bp = BinaryToLong(bitboards["bp"]);
	br = BinaryToLong(bitboards["br"]);
	bn = BinaryToLong(bitboards["bn"]);
	bb = BinaryToLong(bitboards["bb"]);
	bq = BinaryToLong(bitboards["bq"]);
	bk = BinaryToLong(bitboards["bk"]);
}

unsigned long long Aurora::BinaryToLong(const string& binary) {
	long long result = 0;
	for (char bit : binary) {
		result <<= 1;
		if (bit == '1') {
			result |= 1;
		}
	}
	return result;
}

string Aurora::LongToBinary(const unsigned long long& mask) {
	return "sdf";
}
