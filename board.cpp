#include <iostream>
#include <vector>
#include "Board.h"

Board::Board() {
    for (int r = 0; r < BOARD_ROWS; ++r) {
        for (int c = 0; c < BOARD_COLS; ++c) {
            squares[r][c] = Piece(PieceType::EMPTY, PieceColor::NONE);
        }
    }

    // Piece order for back ranks (Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook)
    std::vector<PieceType> backRankOrder = {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    // 2. Setup Black's pieces
    // Black's back rank (Rank 8 -> array row 0)
    for (int c = 0; c < BOARD_COLS; ++c) {
        squares[0][c] = Piece(backRankOrder[c], PieceColor::BLACK);
    }
    // Black's pawns (Rank 7 -> array row 1)
    for (int c = 0; c < BOARD_COLS; ++c) {
        squares[1][c] = Piece(PieceType::PAWN, PieceColor::BLACK);
    }

    // 3. Setup White's pieces
    // White's pawns (Rank 2 -> array row 6)
    for (int c = 0; c < BOARD_COLS; ++c) {
        squares[6][c] = Piece(PieceType::PAWN, PieceColor::WHITE);
    }
    // White's back rank (Rank 1 -> array row 7)
    for (int c = 0; c < BOARD_COLS; ++c) {
        squares[7][c] = Piece(backRankOrder[c], PieceColor::WHITE);
    }
}

// Gets the piece at a given 0-indexed row and column.
Piece Board::getPieceAt(int row, int col) const {
    if (row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS) {
        return squares[row][col];
    }
    // Return an empty piece if out of bounds (or throw an error)
    return Piece(PieceType::EMPTY, PieceColor::NONE);
}

// Sets a piece at a given 0-indexed row and column.
void Board::setPieceAt(int row, int col, const Piece& piece) {
    if (row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS) {
        squares[row][col] = piece;
    }
    // Else, you might want to log an error or throw an exception if out of bounds.
}

void Board::printBoard() const {
    std::cout << std::endl;
    std::cout << "   a b c d e f g h" << std::endl;
    std::cout << "  +-----------------" << std::endl;
    for (int r = 0; r < BOARD_ROWS; ++r) {
        // Print rank number (8 down to 1)
        std::cout << (8 - r) << " |";
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
    std::cout << "  +-----------------" << std::endl;
    std::cout << "   a b c d e f g h" << std::endl;
    std::cout << std::endl;
}

// Implementation for the helper function (if you added it to Board.h)
// void Board::setupRank(int row, PieceColor color, const std::vector<PieceType>& pieceOrder) {
//     for (int c = 0; c < BOARD_COLS; ++c) {
//         if (c < pieceOrder.size()) {
//             squares[row][c] = Piece(pieceOrder[c], color);
//         } else {
//             // Handle cases where pieceOrder might be shorter than BOARD_COLS, though not for back ranks
//             squares[row][c] = Piece(PieceType::EMPTY, PieceColor::NONE);
//         }
//     }
// }