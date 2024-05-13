#include "Aurora.h"

Aurora::Aurora(string fen) : fen(fen) {
	Load(fen);
}

list<Move> Aurora::GetMoves() {
	list<Move> moves;

	unsigned long long empty = ~(wp | wr | wn | wb | wq | wk | bp | br | bn | bb | bq | bk);

	if (turn) {
		unsigned long long notWhitePieces = ~(wp | wr | wn | wb | wq | wk | bk);
		unsigned long long blackPieces = bp | br | bn | bb | bq;

		moves = GetWhiteMoves(notWhitePieces, blackPieces, empty);
	}
	else {
		unsigned long long notBlackPieces = ~(bp | br | bn | bb | bq | bk | wk);
		unsigned long long whitePieces = wp | wr | wn | wb | wq;

		moves = GetBlackMoves(notBlackPieces, whitePieces, empty);
	}

	return moves;
}

list<Move> Aurora::GetWhiteMoves(unsigned long long notWhitePieces, unsigned long long blackPieces, unsigned long long empty) {
	list<Move> moves = GetWPMoves(blackPieces, empty);
	list<Move> knightMoves = GetWNMoves(notWhitePieces, blackPieces);

	moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());

	return moves;
}

list<Move> Aurora::GetBlackMoves(unsigned long long notBlackPieces, unsigned long long whitePieces, unsigned long long empty) {
	list<Move> moves = GetBPMoves(whitePieces, empty);
	list<Move> knightMoves = GetBNMoves(notBlackPieces, whitePieces);

	moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());

	return moves;
}

list<Move> Aurora::GetWPMoves(unsigned long long blackPieces, unsigned long long empty) {
	list<Move> moves;
	const int types = 4;
	unsigned long long* promotionTypes[types] = { &wn, &wb, &wr, &wq };

	// Pawn move forward 1
	unsigned long long forwardMoves = (wp << 8) & empty & ~RANK_8;
	unsigned long long piece = forwardMoves & ~(forwardMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &wp, piece | (piece >> 8) });

		// Loop
		forwardMoves ^= piece;
		piece = forwardMoves & ~(forwardMoves - 1);
	}


	// Pawn move forward 2
	unsigned long long dblForwadMoves = (wp << 16) & empty & (empty << 8) & RANK_4;
	piece = dblForwadMoves & ~(dblForwadMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &wp, piece | (piece >> 16) });

		// Loop
		dblForwadMoves ^= piece;
		piece = dblForwadMoves & ~(dblForwadMoves - 1);
	}


	// Capture right
	unsigned long long captureRightMoves = (wp << 7) & blackPieces & ~RANK_8 & ~FILE_A;
	piece = captureRightMoves & ~(captureRightMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &wp, piece | (piece >> 7), piece, GetBlackPiece(piece) });


		// Loop
		captureRightMoves ^= piece;
		piece = captureRightMoves & ~(captureRightMoves - 1);
	}


	// Capture left
	unsigned long long captureLeftMoves = (wp << 9) & blackPieces & ~RANK_8 & ~FILE_H;
	piece = captureLeftMoves & ~(captureLeftMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &wp, piece | (piece >> 9), piece, GetBlackPiece(piece) });

		// Loop
		captureLeftMoves ^= piece;
		piece = captureLeftMoves & ~(captureLeftMoves - 1);
	}


	// Promotion forward
	unsigned long long promotionForward = (wp << 8) & empty & RANK_8;
	piece = promotionForward & ~(promotionForward - 1);

	while (piece) {
		// Loop for all promotion piece type
		for (int type = 0; type < types; type++) {
			// Add pawn move
			moves.push_back({ &wp, piece | (piece >> 8), 0ULL, 0ULL, piece, promotionTypes[type] });
		}

		// Loop
		promotionForward ^= piece;
		piece = promotionForward & ~(promotionForward - 1);
	}


	// Promotion capture right
	unsigned long long promotionCaptureRight = (wp << 7) & blackPieces & RANK_8 & ~FILE_A;
	piece = promotionCaptureRight & ~(promotionCaptureRight - 1);

	while (piece) {
		// Loop for all promotion piece type
		for (int type = 0; type < types; type++) {
			// Add pawn move
			moves.push_back({ &wp, piece | (piece >> 7), piece, GetBlackPiece(piece), piece, promotionTypes[type]});
		}

		// Loop
		promotionCaptureRight ^= piece;
		piece = promotionCaptureRight & ~(promotionCaptureRight - 1);
	}


	// Promotion capture left
	unsigned long long promotionCaptureLeft = (wp << 9) & blackPieces & RANK_8 & ~FILE_H;
	piece = promotionCaptureLeft & ~(promotionCaptureLeft - 1);

	while (piece) {
		// Loop for all promotion piece type
		for (int type = 0; type < types; type++) {
			// Add pawn move
			moves.push_back({ &wp, piece | (piece >> 9), piece, GetBlackPiece(piece), piece, promotionTypes[type] });
		}

		// Loop
		promotionCaptureLeft ^= piece;
		piece = promotionCaptureLeft & ~(promotionCaptureLeft - 1);
	}


	// En passant capture right 
	unsigned long long enPassantCaptureRight = (wp >> 1) & bp & RANK_5 & ~FILE_A;
	piece = enPassantCaptureRight & ~(enPassantCaptureRight - 1);

	while (piece) {
		// Check if can en passant
		if ((piece << 8) == enPassant) {
			// Add pawn move
			moves.push_back({ &wp, (piece << 1) | enPassant, 0ULL, 0ULL, 0ULL, 0ULL, piece, &bp });
		}

		// Loop
		enPassantCaptureRight ^= piece;
		piece = enPassantCaptureRight & ~(enPassantCaptureRight - 1);
	}


	// En passant capture left 
	unsigned long long enPassantCaptureLeft = (wp << 1) & bp & RANK_5 & ~FILE_H;
	piece = enPassantCaptureLeft & ~(enPassantCaptureLeft - 1);

	while (piece) {
		// Check if can en passant
		if ((piece << 8) == enPassant) {
			// Add pawn move
			moves.push_back({ &wp, (piece >> 1) | enPassant, 0ULL, 0ULL, 0ULL, 0ULL, piece, &bp });
		}

		// Loop
		enPassantCaptureLeft ^= piece;
		piece = enPassantCaptureLeft & ~(enPassantCaptureLeft - 1);
	}

	return moves;
}

list<Move> Aurora::GetBPMoves(unsigned long long whitePieces, unsigned long long empty) {
	list<Move> moves;
	const int types = 4;
	unsigned long long* promotionTypes[types] = { &bn, &bb, &br, &bq };

	// Pawn move forward 1
	unsigned long long forwardMoves = (bp >> 8) & empty & ~RANK_1;
	unsigned long long piece = forwardMoves & ~(forwardMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &bp, piece | (piece << 8) });

		// Loop
		forwardMoves ^= piece;
		piece = forwardMoves & ~(forwardMoves - 1);
	}


	// Pawn move forward 2
	unsigned long long dblForwadMoves = (bp >> 16) & empty & (empty >> 8) & RANK_5;
	piece = dblForwadMoves & ~(dblForwadMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &bp, piece | (piece << 16) });

		// Loop
		dblForwadMoves ^= piece;
		piece = dblForwadMoves & ~(dblForwadMoves - 1);
	}


	// Capture right
	unsigned long long captureRightMoves = (bp >> 7) & whitePieces & ~RANK_1 & ~FILE_H;
	piece = captureRightMoves & ~(captureRightMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &bp, piece | (piece << 7), piece, GetWhitePiece(piece) });


		// Loop
		captureRightMoves ^= piece;
		piece = captureRightMoves & ~(captureRightMoves - 1);
	}


	// Capture left
	unsigned long long captureLeftMoves = (bp >> 9) & whitePieces & ~RANK_1 & ~FILE_A;
	piece = captureLeftMoves & ~(captureLeftMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ &bp, piece | (piece << 9), piece, GetWhitePiece(piece) });

		// Loop
		captureLeftMoves ^= piece;
		piece = captureLeftMoves & ~(captureLeftMoves - 1);
	}


	// Promotion forward
	unsigned long long promotionForward = (bp >> 8) & empty & RANK_1;
	piece = promotionForward & ~(promotionForward - 1);

	while (piece) {
		// Loop for all promotion piece type
		for (int type = 0; type < types; type++) {
			// Add pawn move
			moves.push_back({ &bp, piece | (piece << 8), 0ULL, 0ULL, piece, promotionTypes[type] });
		}

		// Loop
		promotionForward ^= piece;
		piece = promotionForward & ~(promotionForward - 1);
	}


	// Promotion capture right
	unsigned long long promotionCaptureRight = (bp >> 7) & whitePieces & RANK_1 & ~FILE_H;
	piece = promotionCaptureRight & ~(promotionCaptureRight - 1);

	while (piece) {
		// Loop for all promotion piece type
		for (int type = 0; type < types; type++) {
			// Add pawn move
			moves.push_back({ &bp, piece | (piece << 7), piece, GetWhitePiece(piece),  piece, promotionTypes[type] });
		}

		// Loop
		promotionCaptureRight ^= piece;
		piece = promotionCaptureRight & ~(promotionCaptureRight - 1);
	}


	// Promotion capture left
	unsigned long long promotionCaptureLeft = (bp >> 9) & whitePieces & RANK_1 & ~FILE_A;
	piece = promotionCaptureLeft & ~(promotionCaptureLeft - 1);

	while (piece) {
		// Loop for all promotion piece type
		for (int type = 0; type < types; type++) {
			// Add pawn move
			moves.push_back({ &bp, piece | (piece << 9), piece, GetWhitePiece(piece), piece, promotionTypes[type]});
		}

		// Loop
		promotionCaptureLeft ^= piece;
		piece = promotionCaptureLeft & ~(promotionCaptureLeft - 1);
	}


	// En passant capture right 
	unsigned long long enPassantCaptureRight = (bp << 1) & wp & RANK_4 & ~FILE_H;
	piece = enPassantCaptureRight & ~(enPassantCaptureRight - 1);

	while (piece) {
		// Check if can en passant
		if ((piece >> 8) == enPassant) {
			// Add pawn move
			moves.push_back({ &bp, (piece >> 1) | enPassant, 0ULL, 0ULL, 0ULL, 0ULL, piece, &wp });
		}

		// Loop
		enPassantCaptureRight ^= piece;
		piece = enPassantCaptureRight & ~(enPassantCaptureRight - 1);
	}


	// En passant capture left 
	unsigned long long enPassantCaptureLeft = (bp >> 1) & wp & RANK_4 & ~FILE_A;
	piece = enPassantCaptureLeft & ~(enPassantCaptureLeft - 1);

	while (piece) {
		// Check if can en passant
		if ((piece >> 8) == enPassant) {
			// Add pawn move
			moves.push_back({ &bp, (piece << 1) | enPassant, 0ULL, 0ULL, 0ULL, 0ULL, piece, &wp });
		}

		// Loop
		enPassantCaptureLeft ^= piece;
		piece = enPassantCaptureLeft & ~(enPassantCaptureLeft - 1);
	}

	return moves;
}


list<Move> Aurora::GetWNMoves(unsigned long long notWhitePieces, unsigned long long blackPieces) {
	list<Move> moves;

	unsigned long long knightsBB = wn;
	unsigned long long piece = knightsBB & ~(knightsBB - 1);
	unsigned long long knightMoves;

	while (piece) {
		int i = log2(piece);

		if (i > 18) {
			knightMoves = KNIGHT_SPAN << (i - 18) & notWhitePieces;
		}
		else {
			knightMoves = KNIGHT_SPAN >> (18 - i) & notWhitePieces;
		}

		if (piece & FILE_AB)
			knightMoves &= ~FILE_GH;
		else if (piece & FILE_GH)
			knightMoves &= ~FILE_AB;

		unsigned long long move = knightMoves & ~(knightMoves - 1);
		while (move) {
			Move knight = { &wn, piece | move };

			if (move & blackPieces) {
				knight.capture = move;
				knight.capturedPiece = GetBlackPiece(move);
			}

			moves.push_back(knight);

			// Loop through moves
			knightMoves ^= move;
			move = knightMoves & ~(knightMoves - 1);
		}

		// Loop through knights
		knightsBB ^= piece;
		piece = knightsBB & ~(knightsBB - 1);
	}

	return moves;
}

list<Move> Aurora::GetBNMoves(unsigned long long notBlackPieces, unsigned long long whitePieces) {
	list<Move> moves;

	unsigned long long knightsBB = bn;
	unsigned long long piece = knightsBB & ~(knightsBB - 1);
	unsigned long long knightMoves;

	while (piece) {
		int i = log2(piece);

		if (i > 18) {
			knightMoves = KNIGHT_SPAN << (i - 18) & notBlackPieces;
		}
		else {
			knightMoves = KNIGHT_SPAN >> (18 - i) & notBlackPieces;
		}

		if (piece & FILE_AB)
			knightMoves &= ~FILE_GH;
		else if (piece & FILE_GH)
			knightMoves &= ~FILE_AB;

		unsigned long long move = knightMoves & ~(knightMoves - 1);
		while (move) {
			Move knight = { &bn, piece | move };

			if (move & whitePieces) {
				knight.capture = move;
				knight.capturedPiece = GetWhitePiece(move);
			}

			moves.push_back(knight);

			// Loop through moves
			knightMoves ^= move;
			move = knightMoves & ~(knightMoves - 1);
		}

		// Loop through knights
		knightsBB ^= piece;
		piece = knightsBB & ~(knightsBB - 1);
	}

	return moves;
}

void Aurora::MakeMove(Move move) {
	*move.piece ^= move.mask;
	
	// En passant
	if (move.enPassantPawnCaptured) {
		// Remove pawn
		*move.enPassantPieceCaptured ^= move.enPassantPawnCaptured;
	}
	
	// Promotion
	if (move.promotion) {
		// Remove pawn that promotes
		*move.piece ^= move.promotion;
	
		// Add created piece
		*move.promotionPiece |= move.promotion;
	}
	
	// Capture
	if (move.capture) {
		// Remove captured piece
		*move.capturedPiece ^= move.capture;
	}
	
	enPassant = 0ULL;
	turn = !turn;
}

unsigned long long* Aurora::GetWhitePiece(unsigned long long mask) {
	unsigned long long* bitboards[6] = { &wp, &wn, &wb, &wq, &wr, &wk };

	for (int i = 0; i < 6; ++i) {
		if (*bitboards[i] & mask) {
			return bitboards[i];
		}
	}
}

unsigned long long* Aurora::GetBlackPiece(unsigned long long mask) {
	unsigned long long* bitboards[6] = { &bp, &bn, &bb, &bq, &br, &bk };

	for (int i = 0; i < 6; ++i) {
		if (*bitboards[i] & mask) {
			return bitboards[i];
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

	stringstream ss(fen);
	string fenBoard, enPassantString, turnString;

	// Fetch fen
	ss >> fenBoard >> turnString >> castling >> enPassantString >> halfMoveClock >> fullMoveNumber;
	
	turn = turnString == "w";

	if (castling == "-") {
		castling = "";
	}

	if (enPassantString != "-") {
		enPassant = GetSquareMask(enPassantString);
	}

	unordered_map<char, string> bitboardsChar = {
		{'P', "wp"}, {'R', "wr"}, {'B', "wb"}, {'N', "wn"}, {'Q', "wq"}, {'K', "wk"},
		{'p', "bp"}, {'r', "br"}, {'n', "bn"}, {'b', "bb"}, {'q', "bq"}, {'k', "bk"}
	};

	unordered_map<string, string> bitboards;
	for (const auto& entry : bitboardsChar) {
		bitboards[entry.second] = "";
	}

	//!		Init Bitboards
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

	// Set Bitboards
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

string Aurora::GetChessNotation(unsigned long long mask) {
	int bitPosition = log2(mask);

	int row = 1 + (bitPosition / 8);
	int col = 'h' - (bitPosition % 8);

	return static_cast<char>(col) + to_string(row);
}

const unsigned long long Aurora::GetAllWhitePieces() {
	return wp | wr | wn | wb | wq | wk;
}

const unsigned long long Aurora::GetAllBlackPieces() {
	return bp | br | bn | bb | bq | bk;
}

const unsigned long long Aurora::GetAllPieces() {
	return GetAllWhitePieces() | GetAllBlackPieces();
}
