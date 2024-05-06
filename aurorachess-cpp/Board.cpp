#include "Board.h"

Board::Board(string fen) : fen(fen) {
	size_t space = fen.find(' ');
	boardPosition = fen.substr(0, space);
};

void Board::DrawMask(const unsigned long long& mask) {
	string board[8][8];

	for (int i = 0; i < 64; i++) {
		if (mask & (static_cast<unsigned long long>(1) << (63 - i))) {
			board[i / 8][i % 8] = 'X';
		}
	}

	cout << endl << "Mask : " << mask << endl << "  +---+---+---+---+---+---+---+---+" << endl;;

	for (int row = 0; row < 8; row++) {
		cout << 8 - row << " | ";

		for (int file = 0; file < 8; file++) {
			if (board[row][file] == "") {
				cout << "  | "; 
			}
			else {
				cout << board[row][file] << " | ";
			}
		}

		cout << endl << "  +---+---+---+---+---+---+---+---+" << endl;
	}

	cout << "    a   b   c   d   e   f   g   h  " << endl << endl;
}

void Board::DrawBoard(string fenToDraw) {
	if (fenToDraw.empty()) {
		fenToDraw = fen;
	}

	cout << "Fen : " << fenToDraw << endl << "Position : " << endl << "  +---+---+---+---+---+---+---+---+" << endl;

	size_t space = fenToDraw.find(' ');
	boardPosition = fenToDraw.substr(0, space);

	size_t pos = 0;
	int rowNumber = 8;
	while (pos < boardPosition.length()) {
		size_t slashPos = boardPosition.find("/", pos);

		string row;
		if (slashPos == string::npos) {
			row = boardPosition.substr(pos);
		}
		else {
			row = boardPosition.substr(pos, slashPos - pos);
			pos = slashPos;
		}

		cout << rowNumber-- << " | ";

		for (char piece : row) {
			if (isdigit(piece)) {
				int num_empty = piece - '0';
				for (int i = 0; i < num_empty; ++i) {
					cout << "  | ";
				}
			}
			else {
				cout << piece << " | ";
			}
		}
		cout << endl << "  +---+---+---+---+---+---+---+---+" << endl;

		pos = slashPos == string::npos ? string::npos : slashPos + 1;
	}

	cout << "    a   b   c   d   e   f   g   h  " << endl << endl;
}