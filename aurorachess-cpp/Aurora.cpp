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
	list<Move> moves;

	list<Move> pawnMoves = GetMovesWP(blackPieces, empty);

	moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());

	return moves;
}

list<Move> Aurora::GetBlackMoves(unsigned long long notBlackPieces, unsigned long long whitePieces, unsigned long long empty) {
	list<Move> moves;

	list<Move> pawnMoves = GetMovesBP(whitePieces, empty);

	moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());

	return moves;
}

list<Move> Aurora::GetMovesWP(unsigned long long blackPieces, unsigned long long empty) {
	list<Move> moves;
	const int types = 4;
	unsigned long long* promotionTypes[types] = { &wn, &wb, &wr, &wq };

	// Pawn move forward 1
	unsigned long long forwardMoves = (wp << 8) & empty & ~RANK_8;
	unsigned long long piece = forwardMoves & ~(forwardMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece >> 8) });

		// Loop
		forwardMoves ^= piece;
		piece = forwardMoves & ~(forwardMoves - 1);
	}


	// Pawn move forward 2
	unsigned long long dblForwadMoves = (wp << 16) & empty & (empty << 8) & RANK_4;
	piece = dblForwadMoves & ~(dblForwadMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece >> 16) });

		// Loop
		dblForwadMoves ^= piece;
		piece = dblForwadMoves & ~(dblForwadMoves - 1);
	}


	// Capture right
	unsigned long long captureRightMoves = (wp << 7) & blackPieces & ~RANK_8 & ~FILE_A;
	piece = captureRightMoves & ~(captureRightMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece >> 7), piece });


		// Loop
		captureRightMoves ^= piece;
		piece = captureRightMoves & ~(captureRightMoves - 1);
	}


	// Capture left
	unsigned long long captureLeftMoves = (wp << 9) & blackPieces & ~RANK_8 & ~FILE_H;
	piece = captureLeftMoves & ~(captureLeftMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece >> 9), piece });

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
			moves.push_back({ piece | (piece >> 8), 0ULL, piece, promotionTypes[type]});
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
			moves.push_back({ piece | (piece >> 7), piece, piece, promotionTypes[type] });
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
			moves.push_back({ piece | (piece >> 9), piece, piece, promotionTypes[type] });
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
			moves.push_back({ (piece << 1) | enPassant, 0ULL, 0ULL, 0ULL, piece });
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
			moves.push_back({ (piece >> 1) | enPassant, 0ULL, 0ULL, 0ULL, piece });
		}

		// Loop
		enPassantCaptureLeft ^= piece;
		piece = enPassantCaptureLeft & ~(enPassantCaptureLeft - 1);
	}

	return moves;
}

list<Move> Aurora::GetMovesBP(unsigned long long whitePieces, unsigned long long empty) {
	list<Move> moves;
	const int types = 4;
	unsigned long long* promotionTypes[types] = { &bn, &bb, &br, &bq };

	// Pawn move forward 1
	unsigned long long forwardMoves = (bp >> 8) & empty & ~RANK_1;
	unsigned long long piece = forwardMoves & ~(forwardMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece << 8) });

		// Loop
		forwardMoves ^= piece;
		piece = forwardMoves & ~(forwardMoves - 1);
	}


	// Pawn move forward 2
	unsigned long long dblForwadMoves = (bp >> 16) & empty & (empty >> 8) & RANK_5;
	piece = dblForwadMoves & ~(dblForwadMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece << 16) });

		// Loop
		dblForwadMoves ^= piece;
		piece = dblForwadMoves & ~(dblForwadMoves - 1);
	}


	// Capture right
	unsigned long long captureRightMoves = (bp >> 7) & whitePieces & ~RANK_1 & ~FILE_H;
	piece = captureRightMoves & ~(captureRightMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece << 7), piece });


		// Loop
		captureRightMoves ^= piece;
		piece = captureRightMoves & ~(captureRightMoves - 1);
	}


	// Capture left
	unsigned long long captureLeftMoves = (bp >> 9) & whitePieces & ~RANK_1 & ~FILE_A;
	piece = captureLeftMoves & ~(captureLeftMoves - 1);

	while (piece) {
		// Add pawn move
		moves.push_back({ piece | (piece << 9), piece });

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
			moves.push_back({ piece | (piece << 8), 0ULL, piece, promotionTypes[type] });
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
			moves.push_back({ piece | (piece << 7), piece, piece, promotionTypes[type] });
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
			moves.push_back({ piece | (piece << 9), piece, piece, promotionTypes[type] });
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
			moves.push_back({ (piece >> 1) | enPassant, 0ULL, 0ULL, 0ULL, piece });
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
			moves.push_back({ (piece << 1) | enPassant, 0ULL, 0ULL, 0ULL, piece });
		}

		// Loop
		enPassantCaptureLeft ^= piece;
		piece = enPassantCaptureLeft & ~(enPassantCaptureLeft - 1);
	}

	return moves;
}


void Aurora::PlayMove(Move move) {
	if (turn) {
		unsigned long long* bitboards[6] = { &wp, &wn, &wb, &wq, &wr, &wk };

		for (int i = 0; i < 6; ++i) {
			// Find the piece that moves
			if (*bitboards[i] & move.mask) {
				*bitboards[i] ^= move.mask;

				// White make en passant
				if (move.enPassantCapture) {
					// So remove black pawn
					bp ^= move.enPassantCapture;

					enPassant = 0ULL;
				}

				// White promotes
				if (move.promotion) {
					// New piece
					wp ^= move.promotion;

					// Remove old pawn that promote
					*move.promotionPiece |= move.promotion;
				}

				// White capture a black piece
				if (move.capture) {
					// Remove captured piece
					unsigned long long* piece = GetBlackPiece(move.capture);
					*piece ^= move.capture;
				}

				turn = !turn;
				return;
			}
		}
	}
	else {
		unsigned long long* bitboards[6] = { &bp, &bn, &bb, &bq, &br, &bk };

		for (int i = 0; i < 6; ++i) {
			// Find the piece that moves
			if (*bitboards[i] & move.mask) {
				*bitboards[i] ^= move.mask;

				// Black make en passant
				if (move.enPassantCapture) {
					// So remove white pawn
					wp ^= move.enPassantCapture;

					enPassant = 0ULL;
				}

				// Black promotes
				if (move.promotion) {
					// New piece
					bp ^= move.promotion;

					// Remove old pawn that promote
					*move.promotionPiece |= move.promotion;
				}

				// Black capture a white piece
				if (move.capture) {
					// Remove captured piece
					unsigned long long* piece = GetWhitePiece(move.capture);
					*piece ^= move.capture;
				}

				turn = !turn;
				return;
			}
		}
	}
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
