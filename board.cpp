#include <iostream>
#include <vector>
#include <utility>
#include "board.h"
#include "piece.h"

const Piece EMPTY_PIECE(PieceType::EMPTY, PieceColor::NONE);

Board::Board() {
	moveCount = 0;
	enPassantTargetSquare = {-1, -1};
    wtime = 0;
    btime = 0;
    winc = 0;
    binc = 0;
	
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
    
    whiteKingsideCastle = true;
	whiteQueensideCastle = true;
	blackKingsideCastle = true;
	blackQueensideCastle = true;
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

std::pair<int, int> Board::convertUciToCoords(const std::string& uciSquare) const {
    if (uciSquare.length() < 2) {
        return {-1, -1}; // Invalid input
    }
    // Converts file 'a'-'h' to col 1-8
    int col = uciSquare[0] - 'a' + 1;
    // Converts rank '1'-'8' to row 9-2
    int row = 10 - (uciSquare[1] - '0');
    return {row, col};
}

// In board.cpp
void Board::pushMove(const std::string& move) {
    if (move.length() < 4) return;

    // --- Checkpoint A ---
    BoardState currentState;
    currentState.enPassantTargetSquare = this->enPassantTargetSquare;
    currentState.whiteKingsideCastle = this->whiteKingsideCastle;
    currentState.whiteQueensideCastle = this->whiteQueensideCastle;
    currentState.blackKingsideCastle = this->blackKingsideCastle;
    currentState.blackQueensideCastle = this->blackQueensideCastle;

    // --- Checkpoint B ---
    std::pair<int, int> from = convertUciToCoords(move.substr(0, 2));
    std::pair<int, int> to = convertUciToCoords(move.substr(2, 2));
    int fromRow = from.first;
    int fromCol = from.second;
    int toRow = to.first;
    int toCol = to.second;

    // --- Checkpoint C ---
    Piece pieceToMove = getPieceAt(fromRow, fromCol);
    Piece capturedPiece = getPieceAt(toRow, toCol);
    enPassantTargetSquare = {-1, -1};

    // --- Checkpoint D ---
    if (pieceToMove.getType() == PieceType::PAWN) {
        if (abs(fromRow - toRow) == 2) {
            enPassantTargetSquare = {(fromRow + toRow) / 2, fromCol};
        } else if (fromCol != toCol && capturedPiece.getType() == PieceType::EMPTY) {
            int capturedPawnRow = fromRow;
            int capturedPawnCol = toCol;
            capturedPiece = getPieceAt(capturedPawnRow, capturedPawnCol); 
            setPieceAt(capturedPawnRow, capturedPawnCol, EMPTY_PIECE);
        }
    }
    
    // --- Checkpoint E ---
    currentState.capturedPiece = capturedPiece;
    history.push_back(currentState);

    // --- Checkpoint F ---
    if (pieceToMove.getType() == PieceType::KING) {
        if (pieceToMove.getColor() == PieceColor::WHITE) { whiteKingsideCastle = false; whiteQueensideCastle = false; }
        else { blackKingsideCastle = false; blackQueensideCastle = false; }
    }
    if ((fromRow == 9 && fromCol == 1) || (toRow == 9 && toCol == 1)) whiteQueensideCastle = false;
    if ((fromRow == 9 && fromCol == 8) || (toRow == 9 && toCol == 8)) whiteKingsideCastle = false;
    if ((fromRow == 2 && fromCol == 1) || (toRow == 2 && toCol == 1)) blackQueensideCastle = false;
    if ((fromRow == 2 && fromCol == 8) || (toRow == 2 && toCol == 8)) blackKingsideCastle = false;

    // --- Checkpoint G ---
    setPieceAt(toRow, toCol, pieceToMove);
    setPieceAt(fromRow, fromCol, EMPTY_PIECE);

    // --- Checkpoint H ---
    if (pieceToMove.getType() == PieceType::KING && abs(fromCol - toCol) == 2) {
        if (toCol == 7) { setPieceAt(fromRow, 6, getPieceAt(fromRow, 8)); setPieceAt(fromRow, 8, EMPTY_PIECE); } 
        else { setPieceAt(fromRow, 4, getPieceAt(fromRow, 1)); setPieceAt(fromRow, 1, EMPTY_PIECE); }
    }
    if (move.length() == 5) {
        char promotionChar = move[4];
        PieceType promotionType = PieceType::QUEEN;
        switch (promotionChar) {
            case 'n': case 'N': promotionType = PieceType::KNIGHT; break;
            case 'b': case 'B': promotionType = PieceType::BISHOP; break;
            case 'r': case 'R': promotionType = PieceType::ROOK;   break;
        }
        setPieceAt(toRow, toCol, Piece(promotionType, pieceToMove.getColor()));
    }

    moveCount++;
}

void Board::popMove(const std::string& move) {
    if (history.empty()) {
        return; // Safety check
    }

    // 1. Get the last state from history
    BoardState lastState = history.back();
    history.pop_back();

    // 2. Restore all high-level state variables
    this->enPassantTargetSquare = lastState.enPassantTargetSquare;
    this->whiteKingsideCastle = lastState.whiteKingsideCastle;
    this->whiteQueensideCastle = lastState.whiteQueensideCastle;
    this->blackKingsideCastle = lastState.blackKingsideCastle;
    this->blackQueensideCastle = lastState.blackQueensideCastle;

    // 3. Get coordinates to reverse the move
    std::pair<int, int> from = convertUciToCoords(move.substr(0, 2));
    std::pair<int, int> to = convertUciToCoords(move.substr(2, 2));
    int fromRow = from.first;
    int fromCol = from.second;
    int toRow = to.first;
    int toCol = to.second;

    Piece pieceThatMoved = getPieceAt(toRow, toCol);

    // 4. Handle promotion first: the piece moving back is a pawn
    if (move.length() == 5) {
        pieceThatMoved = Piece(PieceType::PAWN, pieceThatMoved.getColor());
    }

    // 5. Move the piece back to its original square
    setPieceAt(fromRow, fromCol, pieceThatMoved);

    // 6. *** THE FIX ***: Determine if the move was an en passant capture
    bool wasEnPassant = (pieceThatMoved.getType() == PieceType::PAWN &&
                         fromCol != toCol &&
                         lastState.capturedPiece.getType() != PieceType::EMPTY &&
                         // The key condition: the destination square for the capture
                         // matches the en passant target from the previous state.
                         toRow == lastState.enPassantTargetSquare.first &&
                         toCol == lastState.enPassantTargetSquare.second);

    if (wasEnPassant) {
        // For en passant, the destination square becomes empty.
        setPieceAt(toRow, toCol, Piece(PieceType::EMPTY, PieceColor::NONE));
        // The captured pawn is placed back on its original square.
        setPieceAt(fromRow, toCol, lastState.capturedPiece);
    } else {
        // For all other moves (regular captures or quiet moves), restore the
        // piece that was on the destination square.
        setPieceAt(toRow, toCol, lastState.capturedPiece);
    }

    // 7. Undo castling (move the rook back)
    if (pieceThatMoved.getType() == PieceType::KING && abs(fromCol - toCol) == 2) {
        if (toCol == 7) { // Kingside
            setPieceAt(fromRow, 8, getPieceAt(fromRow, 6));
            setPieceAt(fromRow, 6, EMPTY_PIECE);
        } else { // Queenside
            setPieceAt(fromRow, 1, getPieceAt(fromRow, 4));
            setPieceAt(fromRow, 4, EMPTY_PIECE);
        }
    }

    // 8. Decrement the move count to fully restore the state
    moveCount--;
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

// Checks if a player of the given color is currently in check.
bool Board::isInCheck(PieceColor color) const {
    // Find the king of the specified color.
    std::pair<int, int> kingPos = findKing(color);
    if (kingPos.first == -1) {
        return false; // Should not happen in a legal game.
    }

    // Determine the opponent's color.
    PieceColor opponentColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

    // Return true if the king's square is attacked by the opponent.
    return isSquareAttacked(kingPos.first, kingPos.second, opponentColor);
}
