#include <iostream>
#include <vector>
#include <utility>
#include "board.h"
#include "piece.h"

const Piece EMPTY_PIECE(PieceType::EMPTY, PieceColor::NONE);

Board::Board() {
	moveCount = 0;
	enPassantTargetSquare = {-1, -1};
	
    for (int r = 0; r < BOARD_ROWS; ++r) {
        for (int c = 0; c < BOARD_COLS; ++c) {
            squares[r][c] = EMPTY_PIECE;
        }
    }

    // Piece order for back ranks (Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook)
    std::vector<PieceType> backRankOrder = {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    // 2. Setup Black's pieces
    // Black's back rank (Rank 8 -> array row 0)
    for (int c = 1; c < 9; ++c) {
        squares[2][c] = Piece(backRankOrder[c-1], PieceColor::BLACK);
    }
    // Black's pawns (Rank 7 -> array row 1)
    for (int c = 1; c < 9; ++c) {
        squares[3][c] = Piece(PieceType::PAWN, PieceColor::BLACK);
    }

    // 3. Setup White's pieces
    // White's pawns (Rank 2 -> array row 6)
    for (int c = 1; c < 9; ++c) {
        squares[8][c] = Piece(PieceType::PAWN, PieceColor::WHITE);
    }
    // White's back rank (Rank 1 -> array row 7)
    for (int c = 1; c < 9; ++c) {
        squares[9][c] = Piece(backRankOrder[c-1], PieceColor::WHITE);
    }
}

// Gets the piece at a given 0-indexed row and column.
Piece Board::getPieceAt(int row, int col) const {
    if (row >= 2 && row < BOARD_ROWS-2 && col >= 1 && col < BOARD_COLS-1) {
        return squares[row][col];
    }
    // Return an empty piece if out of bounds (or throw an error)
    return Piece(PieceType::EMPTY, PieceColor::NONE);
}

// Sets a piece at a given 0-indexed row and column.
void Board::setPieceAt(int row, int col, const Piece& piece) {
    if (row >= 2 && row < BOARD_ROWS-2 && col >= 1 && col < BOARD_COLS-1) {
        squares[row][col] = piece;
    }
    // Else, you might want to log an error or throw an exception if out of bounds.
}

void Board::printBoard() {
	int x = 10;
    std::cout << std::endl;
    std::cout << "      a b c d e f g h" << std::endl;
    std::cout << "   +--------------------+" << std::endl;
    for (int r = 0; r < BOARD_ROWS; ++r) {
        // Print rank number (8 down to 1)
		// Add a space if the number is a single digit to align it with two-digit numbers.
		std::cout << (((x - r) >= 0 && (x - r) <= 9) ? " " : "") << (x - r) << " |";
        for (int c = 0; c < BOARD_COLS; ++c) {
            Piece piece = squares[r][c];
            char pieceChar = '.'; // Default for empty square

            switch (piece.getType()) {
                case PieceType::PAWN:   pieceChar = (piece.getColor() == PieceColor::WHITE) ? 'P' : 'p'; break;
                case PieceType::KNIGHT: pieceChar = (piece.getColor() == PieceColor::WHITE) ? 'N' : 'n'; break;
                case PieceType::BISHOP: pieceChar = (piece.getColor() == PieceColor::WHITE) ? 'B' : 'b'; break;
                case PieceType::ROOK:   pieceChar = (piece.getColor() == PieceColor::WHITE) ? 'R' : 'r'; break;
                case PieceType::QUEEN:  pieceChar = (piece.getColor() == PieceColor::WHITE) ? 'Q' : 'q'; break;
                case PieceType::KING:   pieceChar = (piece.getColor() == PieceColor::WHITE) ? 'K' : 'k'; break;
                case PieceType::EMPTY: // Fall through to default
                default:                pieceChar = '.'; break;
            }
            std::cout << pieceChar << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "   +--------------------+" << std::endl;
    std::cout << "      a b c d e f g h" << std::endl;
    std::cout << std::endl;
}

void Board::setWTime(int WTime) {
	this->wtime = WTime;
}

void Board::setBTime(int BTime) {
	this->btime = BTime;
}

void Board::setWInc(int WInc) {
	this->winc = WInc;
}
void Board::setBInc(int BInc) {
	this->binc = BInc;
}

std::pair<int, int> Board::convertUciToCoords(const std::string& uciSquare) {
    if (uciSquare.length() < 2) {
        return {-1, -1}; // Invalid input
    }
    // Converts file 'a'-'h' to col 1-8
    int col = uciSquare[0] - 'a' + 1;
    // Converts rank '1'-'8' to row 9-2
    int row = 10 - (uciSquare[1] - '0');
    return {row, col};
}

void Board::pushMove(const std::string& move) {
	if (move.length() < 4) return; // Invalid move string

    // 1. Convert UCI notation to board coordinates
    std::pair<int, int> from = convertUciToCoords(move.substr(0, 2));
    std::pair<int, int> to = convertUciToCoords(move.substr(2, 2));
    int fromRow = from.first;
    int fromCol = from.second;
    int toRow = to.first;
    int toCol = to.second;

    Piece pieceToMove = getPieceAt(fromRow, fromCol);

	std::pair<int, int> lastEnPassantTarget = enPassantTargetSquare; // Store old target

    // 1. First, reset the en passant square. It's only valid for one turn.
    enPassantTargetSquare = {-1, -1};
    // 2. Check if this move is a pawn double push, which creates a new en passant opportunity.
    if (pieceToMove.getType() == PieceType::PAWN && abs(fromRow - toRow) == 2) {
        enPassantTargetSquare = {(fromRow + toRow) / 2, fromCol};
    }

    if (pieceToMove.getType() == PieceType::PAWN) {
        // Check if the move is a diagonal pawn move to an empty square
        // and that square was the valid en passant target from the previous turn.
        if (fromCol != toCol && getPieceAt(toRow, toCol).getType() == PieceType::EMPTY) {
            if (toRow == lastEnPassantTarget.first && toCol == lastEnPassantTarget.second) {
                // This is an en passant capture. Remove the captured pawn.
                // The captured pawn is on the same row as the moving pawn, but in the destination column.
                int capturedPawnRow = fromRow;
                int capturedPawnCol = toCol;
                setPieceAt(capturedPawnRow, capturedPawnCol, EMPTY_PIECE); // Remove the ghost pawn
            }
        }
    }

    // 2. Handle special moves
    // Castling: represented by the king's two-square move
    if (pieceToMove.getType() == PieceType::KING) {
        int colDiff = fromCol - toCol;
        if (abs(colDiff) == 2) { // Castling move
            // White Kingside ("e1g1")
            if (fromRow == 9 && fromCol == 5 && toCol == 7) {
                setPieceAt(9, 6, getPieceAt(9, 8)); // Move rook
                setPieceAt(9, 8, EMPTY_PIECE);
            }
            // White Queenside ("e1c1")
            else if (fromRow == 9 && fromCol == 5 && toCol == 3) {
                setPieceAt(9, 4, getPieceAt(9, 1)); // Move rook
                setPieceAt(9, 1, EMPTY_PIECE);
            }
            // Black Kingside ("e8g8")
            else if (fromRow == 2 && fromCol == 5 && toCol == 7) {
                setPieceAt(2, 6, getPieceAt(2, 8)); // Move rook
                setPieceAt(2, 8, EMPTY_PIECE);
            }
            // Black Queenside ("e8c8")
            else if (fromRow == 2 && fromCol == 5 && toCol == 3) {
                setPieceAt(2, 4, getPieceAt(2, 1)); // Move rook
                setPieceAt(2, 1, EMPTY_PIECE);
            }
        }
    }

    // Pawn Promotion: identified by a 5th character
    if (move.length() == 5) {
        char promotionChar = move[4];
        PieceType promotionType = PieceType::QUEEN; // Default to queen
        switch (promotionChar) {
            case 'n':
            case 'N':
                promotionType = PieceType::KNIGHT;
                break;
            case 'b':
            case 'B':
                promotionType = PieceType::BISHOP;
                break;
            case 'r':
            case 'R':
                promotionType = PieceType::ROOK;
                break;
            case 'q':
            case 'Q':
                promotionType = PieceType::QUEEN;
                break;
        }
        Piece promotedPiece(promotionType, pieceToMove.getColor());
        setPieceAt(toRow, toCol, promotedPiece);
        setPieceAt(fromRow, fromCol, EMPTY_PIECE); // Empty the original square
    }

    // 3. Handle standard moves
    setPieceAt(toRow, toCol, pieceToMove);
    setPieceAt(fromRow, fromCol, EMPTY_PIECE);
    
    moveCount++;
}

void Board::initializeBoard() {
	*this = Board();
}

PieceColor Board::getCurrentPlayer() const {
	if (moveCount % 2 == 0) {
		return PieceColor::WHITE;
	}
	else {
		return PieceColor::BLACK;
	}
}

std::pair<int, int> Board::getEnPassantTarget() const {
    return enPassantTargetSquare;
}

// Scans the board to find the location of the specified king.
std::pair<int, int> Board::findKing(PieceColor kingColor) const {
    for (int r = 2; r < BOARD_ROWS - 2; ++r) {
        for (int c = 1; c < BOARD_COLS - 1; ++c) {
            Piece piece = getPieceAt(r, c);
            if (piece.getType() == PieceType::KING && piece.getColor() == kingColor) {
                return {r, c};
            }
        }
    }
    return {-1, -1}; // Should not happen in a legal game
}

// ✅ NEW IMPLEMENTATION: isSquareAttacked
// Checks if a square at (r, c) is being attacked by a piece of 'attackerColor'.
bool Board::isSquareAttacked(int r, int c, PieceColor attackerColor) const {
    // 1. Check for Pawn attacks
    int pawnDir = (attackerColor == PieceColor::WHITE) ? 1 : -1;
    if (getPieceAt(r + pawnDir, c - 1).getType() == PieceType::PAWN && getPieceAt(r + pawnDir, c - 1).getColor() == attackerColor) return true;
    if (getPieceAt(r + pawnDir, c + 1).getType() == PieceType::PAWN && getPieceAt(r + pawnDir, c + 1).getColor() == attackerColor) return true;

    // 2. Check for Knight attacks
    const int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (const auto& move : knightMoves) {
        Piece target = getPieceAt(r + move[0], c + move[1]);
        if (target.getType() == PieceType::KNIGHT && target.getColor() == attackerColor) return true;
    }

    // 3. Check for sliding pieces (Rooks, Bishops, Queens)
    // Check orthogonally (Rooks, Queens)
    const int orthoDirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (const auto& dir : orthoDirs) {
        int newR = r + dir[0];
        int newC = c + dir[1];
        while (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {
            Piece target = getPieceAt(newR, newC);
            if (target.getType() != PieceType::EMPTY) {
                if (target.getColor() == attackerColor && (target.getType() == PieceType::ROOK || target.getType() == PieceType::QUEEN)) {
                    return true;
                }
                break; // Path is blocked
            }
            newR += dir[0];
            newC += dir[1];
        }
    }

    // Check diagonally (Bishops, Queens)
    const int diagDirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (const auto& dir : diagDirs) {
        int newR = r + dir[0];
        int newC = c + dir[1];
        while (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {
            Piece target = getPieceAt(newR, newC);
            if (target.getType() != PieceType::EMPTY) {
                if (target.getColor() == attackerColor && (target.getType() == PieceType::BISHOP || target.getType() == PieceType::QUEEN)) {
                    return true;
                }
                break; // Path is blocked
            }
            newR += dir[0];
            newC += dir[1];
        }
    }

    // 4. Check for King attacks
    const int kingMoves[8][2] = {{-1,0},{1,0},{0,1},{0,-1},{-1,1},{-1,-1},{1,1},{1,-1}};
    for (const auto& move : kingMoves) {
        Piece target = getPieceAt(r + move[0], c + move[1]);
        if (target.getType() == PieceType::KING && target.getColor() == attackerColor) return true;
    }

    return false; // Square is not attacked
}

