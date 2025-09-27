#ifndef BOARD_H_
#define BOARD_H_

#include "piece.h"
#include <vector>
#include <string>
#include <utility>

const int BOARD_ROWS = 12;
const int BOARD_COLS = 10;

class Board {
public:
    Board();

    Piece getPieceAt(int row, int col) const;
    void setPieceAt(int row, int col, const Piece& piece);
    void printBoard();
	void setWTime(int WTime);
	void setBTime(int BTime);
	void setWInc(int WInc);
	void setBInc(int BInc);
	std::pair<int, int> convertUciToCoords(const std::string& uciSquare);
	void pushMove(const std::string& move);
	void initializeBoard();
	PieceColor getCurrentPlayer() const;
	std::pair<int, int> getEnPassantTarget() const;
	std::pair<int, int> findKing(PieceColor kingColor) const;
	bool isSquareAttacked(int r, int c, PieceColor attackerColor) const;
    // void makeMove(const std::string& move);
    // bool isMoveLegal(...);

private:
    Piece squares[BOARD_ROWS][BOARD_COLS];
    int moveCount;
    std::pair<int, int> enPassantTargetSquare;
    
    int wtime; // White's remaining time in milliseconds
    int btime; // Black's remaining time in milliseconds
    int winc;  // White's increment in milliseconds
    int binc;  // Black's increment in milliseconds
    
    void setupRank(int row, PieceColor color, const std::vector<PieceType>& pieceOrder);
};

#endif