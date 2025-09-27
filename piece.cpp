#include <iostream>
#include "piece.h"

Piece::Piece(PieceType pt, PieceColor pc) {
	this->pt = pt;
	this->pc = pc;
}

void Piece::setType(PieceType pt) {
	this->pt = pt;
}

PieceType Piece::getType() const {
	return pt;
}

PieceColor Piece::getColor() const{
	return pc;
}
