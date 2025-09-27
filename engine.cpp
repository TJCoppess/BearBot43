#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "engine.h"
#include "board.h"

std::string convertCoordsToUci(int r, int c) {
    char file = 'a' + (c - 1); // 'a' + (5 - 1) = 'e'
    char rank = '1' + (9 - r); // '1' + (9 - 2) = '8'
    return {file, rank};
}

std::vector<std::string> generateKingMoves(Board &board, int r, int c) {
    std::vector<std::string> moves;
    Piece king = board.getPieceAt(r, c);
    PieceColor kingColor = king.getColor();
    std::string fromSquare = convertCoordsToUci(r, c);

    // 1. Determine the opponent's color to check for attacks.
    PieceColor opponentColor = (kingColor == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

    const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, 1}, {0, -1},
        {-1, 1}, {-1, -1}, {1, 1}, {1, -1}
    };

    for (const auto& dir : directions) {
        int dr = dir[0];
        int dc = dir[1];
        int newR = r + dr;
        int newC = c + dc;

        // Check if the square is on the board
        if (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {

            // 2. Check if the destination square is NOT attacked by the opponent.
            if (!board.isSquareAttacked(newR, newC, opponentColor)) {
                Piece targetPiece = board.getPieceAt(newR, newC);

                // 3. Add the move if the square is empty or has an enemy piece.
                if (targetPiece.getColor() != kingColor) {
                    moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
                }
            }
        }
    }

    return moves;
}

std::vector<std::string> generateQueenMoves(Board &board, int r, int c) {
	std::vector<std::string> moves;
	Piece queen = board.getPieceAt(r, c);
	PieceColor queenColor = queen.getColor();
	std::string fromSquare = convertCoordsToUci(r, c);

	// Define the 8 directions a queen can move (N, S, E, W, NE, NW, SE, SW)
	// These are pairs of {row_change, col_change}
	const int directions[8][2] = {
		{-1, 0},  // Up
		{1, 0},   // Down
		{0, 1},   // Right
		{0, -1},  // Left
		{-1, 1},  // Up-Right
		{-1, -1}, // Up-Left
		{1, 1},   // Down-Right
		{1, -1}   // Down-Left
	};

	// Iterate through each of the 8 directions
	for (const auto& dir : directions) {
		int dr = dir[0]; // Change in row for this direction
		int dc = dir[1]; // Change in col for this direction
		int newR = r + dr;
		int newC = c + dc;

		// Keep moving in the current direction until we go off-board or are blocked
		while (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {
			Piece targetPiece = board.getPieceAt(newR, newC);

			// Case 1: The square is empty
			if (targetPiece.getType() == PieceType::EMPTY) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
			}
			// Case 2: The square has an enemy piece
			else if (targetPiece.getColor() != queenColor) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
				break; // Can't move past an enemy piece
			}
			// Case 3: The square has a friendly piece
			else { // targetPiece.getColor() == queenColor
				break; // Can't move onto or past a friendly piece
			}

			// Move to the next square in this direction
			newR += dr;
			newC += dc;
		}
	}

	return moves;
}

std::vector<std::string> generateBishopMoves(Board &board, int r, int c) {
	std::vector<std::string> moves;
	PieceColor bishopColor = board.getPieceAt(r, c).getColor();
	std::string fromSquare = convertCoordsToUci(r, c);

	// Directions for a bishop: Diagonals
	const int directions[4][2] = {
		{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
	};

	for (const auto& dir : directions) {
		int newR = r + dir[0];
		int newC = c + dir[1];

		while (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {
			Piece targetPiece = board.getPieceAt(newR, newC);

			if (targetPiece.getType() == PieceType::EMPTY) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
			} else if (targetPiece.getColor() != bishopColor) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
				break; // Capture and stop
			} else {
				break; // Blocked by friendly piece
			}
			newR += dir[0];
			newC += dir[1];
		}
	}
	return moves;
}

std::vector<std::string> generateKnightMoves(Board &board, int r, int c) {
	std::vector<std::string> moves;
	PieceColor knightColor = board.getPieceAt(r, c).getColor();
	std::string fromSquare = convertCoordsToUci(r, c);

	// 8 possible L-shaped moves for a knight
	const int directions[8][2] = {
		{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
		{1, -2}, {1, 2}, {2, -1}, {2, 1}
	};

	for (const auto& dir : directions) {
		int newR = r + dir[0];
		int newC = c + dir[1];

		// Check if the target square is on the board
		if (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {
			Piece targetPiece = board.getPieceAt(newR, newC);
			// Add move if the square is empty or contains an enemy piece
			if (targetPiece.getType() == PieceType::EMPTY || targetPiece.getColor() != knightColor) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
			}
		}
	}
	return moves;
}

std::vector<std::string> generateRookMoves(Board &board, int r, int c) {
	std::vector<std::string> moves;
	PieceColor rookColor = board.getPieceAt(r, c).getColor();
	std::string fromSquare = convertCoordsToUci(r, c);

	// Directions for a rook: Up, Down, Left, Right
	const int directions[4][2] = {
		{-1, 0}, {1, 0}, {0, -1}, {0, 1}
	};

	for (const auto& dir : directions) {
		int newR = r + dir[0];
		int newC = c + dir[1];

		while (newR >= 2 && newR <= 9 && newC >= 1 && newC <= 8) {
			Piece targetPiece = board.getPieceAt(newR, newC);

			if (targetPiece.getType() == PieceType::EMPTY) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
			} else if (targetPiece.getColor() != rookColor) {
				moves.push_back(fromSquare + convertCoordsToUci(newR, newC));
				break; // Capture and stop
			} else {
				break; // Blocked by friendly piece
			}
			newR += dir[0];
			newC += dir[1];
		}
	}
	return moves;
}

std::vector<std::string> generatePawnMoves(Board &board, int r, int c) {
    std::vector<std::string> moves;
    PieceColor pawnColor = board.getPieceAt(r, c).getColor();
    std::string fromSquare = convertCoordsToUci(r, c);

    // Determine direction and key ranks based on the pawn's color
    int direction = (pawnColor == PieceColor::WHITE) ? -1 : 1;
    int startRank = (pawnColor == PieceColor::WHITE) ? 8 : 3;
    int promotionRank = (pawnColor == PieceColor::WHITE) ? 2 : 9;

    // --- 1. Forward Moves ---
    int oneStepR = r + direction;
    // Check if the square in front is on the board and is empty
    if (oneStepR >= 2 && oneStepR <= 9 && board.getPieceAt(oneStepR, c).getType() == PieceType::EMPTY) {
        
        // A) Promotion on a single push
        if (oneStepR == promotionRank) {
            std::string toSquare = convertCoordsToUci(oneStepR, c);
            moves.push_back(fromSquare + toSquare + 'q'); // Promote to Queen
            moves.push_back(fromSquare + toSquare + 'r'); // Promote to Rook
            moves.push_back(fromSquare + toSquare + 'b'); // Promote to Bishop
            moves.push_back(fromSquare + toSquare + 'n'); // Promote to Knight
        } 
        // B) Regular single push
        else {
            moves.push_back(fromSquare + convertCoordsToUci(oneStepR, c));
        }

        // C) Double push from starting rank (can only happen if single push is legal)
        if (r == startRank) {
            int twoStepsR = r + 2 * direction;
            if (board.getPieceAt(twoStepsR, c).getType() == PieceType::EMPTY) {
                moves.push_back(fromSquare + convertCoordsToUci(twoStepsR, c));
            }
        }
    }

    // --- 2. Captures ---
    int captureCols[] = {c - 1, c + 1}; // Check left and right diagonals
    for (int captureC : captureCols) {
        if (captureC >= 1 && captureC <= 8) { // Check if capture column is on the board
            Piece targetPiece = board.getPieceAt(oneStepR, captureC);
            // A) Regular Capture: Is there an enemy piece on the diagonal?
            if (targetPiece.getType() != PieceType::EMPTY && targetPiece.getColor() != pawnColor) {
                // Check for promotion on capture
                if (oneStepR == promotionRank) {
                    std::string toSquare = convertCoordsToUci(oneStepR, captureC);
                    moves.push_back(fromSquare + toSquare + 'q');
                    moves.push_back(fromSquare + toSquare + 'r');
                    moves.push_back(fromSquare + toSquare + 'b');
                    moves.push_back(fromSquare + toSquare + 'n');
                } else {
                    moves.push_back(fromSquare + convertCoordsToUci(oneStepR, captureC));
                }
            }
        }
    }
    
    // --- 3. En Passant Capture ---
    std::pair<int, int> enPassantTarget = board.getEnPassantTarget();
    // Check if a valid en passant target square exists from the last move
    if (enPassantTarget.first != -1) {
        // Check if the pawn's capture move would land on that target square
        if (oneStepR == enPassantTarget.first && (c - 1 == enPassantTarget.second || c + 1 == enPassantTarget.second)) {
             moves.push_back(fromSquare + convertCoordsToUci(enPassantTarget.first, enPassantTarget.second));
        }
    }

    return moves;
}

std::vector<std::string> generatePseudoLegalMoves(Board& board) { // Pass board by reference
	std::vector<std::string> generatedMoves;
	PieceColor currentPlayer = board.getCurrentPlayer();
	
	for (int r = 2; r < BOARD_ROWS - 2; ++r) {
		for (int c = 1; c < BOARD_COLS - 1; ++c) {
			Piece currentPiece = board.getPieceAt(r, c);
			if (currentPiece.getColor() == currentPlayer) {
				switch (currentPiece.getType()) {
					case PieceType::KING: {
						std::vector<std::string> kingMoves = generateKingMoves(board, r, c);
						generatedMoves.insert(generatedMoves.end(), kingMoves.begin(), kingMoves.end());
						break;
					}
					case PieceType::QUEEN: {
						std::vector<std::string> queenMoves = generateQueenMoves(board, r, c);
						generatedMoves.insert(generatedMoves.end(), queenMoves.begin(), queenMoves.end());
						break;
					}
					case PieceType::BISHOP: {
						std::vector<std::string> bishopMoves = generateBishopMoves(board, r, c);
						generatedMoves.insert(generatedMoves.end(), bishopMoves.begin(), bishopMoves.end());
						break;
					}
					case PieceType::KNIGHT: {
						std::vector<std::string> knightMoves = generateKnightMoves(board, r, c);
						generatedMoves.insert(generatedMoves.end(), knightMoves.begin(), knightMoves.end());
						break;
					}
					case PieceType::ROOK: {
						std::vector<std::string> rookMoves = generateRookMoves(board, r, c);
						generatedMoves.insert(generatedMoves.end(), rookMoves.begin(), rookMoves.end());
						break;
					}
					case PieceType::PAWN: {
						std::vector<std::string> pawnMoves = generatePawnMoves(board, r, c);
						generatedMoves.insert(generatedMoves.end(), pawnMoves.begin(), pawnMoves.end());
						break;
					}
					default:
						break;
				}
			}
		}
	}
	return generatedMoves;
}

// This function filters the pseudo-legal moves to produce only fully legal moves.
std::vector<std::string> generateLegalMoves(Board& board) {
    std::vector<std::string> legalMoves;
    std::vector<std::string> pseudoLegalMoves = generatePseudoLegalMoves(board);
    PieceColor currentPlayer = board.getCurrentPlayer();
    PieceColor opponentColor = (currentPlayer == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

    for (const std::string& move : pseudoLegalMoves) {
        Board boardCopy = board; // Create a temporary copy of the board
        boardCopy.pushMove(move);   // Make the move on the copy

        // Find the king on the board *after* the move
        std::pair<int, int> kingPos = boardCopy.findKing(currentPlayer);
        
        // If the king is not in check after the move, it's a legal move
        if (!boardCopy.isSquareAttacked(kingPos.first, kingPos.second, opponentColor)) {
            legalMoves.push_back(move);
        }
    }
    return legalMoves;
}

std::string search(std::vector<std::string> moves) {
    if (moves.empty()) {
        return ""; // Return an empty string or handle no-moves case
    }
	return moves[rand() % moves.size()];
}