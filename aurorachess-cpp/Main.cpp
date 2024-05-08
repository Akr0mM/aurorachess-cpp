#include <iostream>

#include "Aurora.h"
#include "Board.h"

using namespace std;

int main()
{
    string fen = "rnbqkbnr/ppppp2p/6p1/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3";

    Board board(fen);
    Aurora aurora(fen);
    
    list<Move> moves = aurora.GetMoves();

    for (const auto& move : moves) {
        Board::DrawMask(move.mask);
    }

    cout << "Total moves : " << moves.size() << endl;

    return 0;
}
