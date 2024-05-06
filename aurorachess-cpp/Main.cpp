#include <iostream>

#include "Aurora.h"
#include "Board.h"

using namespace std;

int main()
{
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board(fen);
    Aurora aurora(fen);
    
    list<unsigned long long> moves = aurora.GetMoves();

    for (const auto& move: moves) {
        Board::DrawMask(move);
    }

    return 0;
}
