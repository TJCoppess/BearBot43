#ifndef PIECE_H_
#define PIECE_H_

enum class PieceType {EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
enum class PieceColor {NONE, WHITE, BLACK};

class Piece {
public:
    Piece(PieceType type = PieceType::EMPTY, PieceColor color = PieceColor::NONE);

	void setType(PieceType);
	PieceType getType() const;
	PieceColor getColor() const;

private:
	PieceType pt;
	PieceColor pc;
};

#endif