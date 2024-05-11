#include <iostream>
#include <chrono>

#include "Aurora.h"
#include "Board.h"

int main()
{
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board(fen);
    Aurora aurora(fen);

    auto start = chrono::steady_clock::now();

    list<Move> moves = aurora.GetMoves();

    auto end = chrono::steady_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    for (const auto& move : moves) {
        Board::DrawMask(move.mask);
    }

    cout << "Total moves : " << moves.size() << " in " << duration << "ms" << endl;
    
    return 0;
}
