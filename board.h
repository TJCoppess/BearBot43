#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <vector>

const int BOARD_ROWS = 12;
const int BOARD_COLS = 10;

class Board {
public:
    Board();

    Piece getPieceAt(int row, int col) const;
    void setPieceAt(int row, int col, const Piece& piece);
    void printBoard() const;
    // void makeMove(const std::string& move);
    // bool isMoveLegal(...);

private:
    Piece squares[BOARD_ROWS][BOARD_COLS];
    void setupRank(int row, PieceColor color, const std::vector<PieceType>& pieceOrder);
};

#endif