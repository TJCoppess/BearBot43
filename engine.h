#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "board.h"

std::string convertCoordsToUci(int, int);

std::vector<std::string> generateKingMoves(Board& board, int r, int c);
std::vector<std::string> generateQueenMoves(Board& board, int r, int c);
std::vector<std::string> generateBishopMoves(Board& board, int r, int c);
std::vector<std::string> generateKnightMoves(Board& board, int r, int c);
std::vector<std::string> generateRookMoves(Board& board, int r, int c);
std::vector<std::string> generatePawnMoves(Board& board, int r, int c);

std::vector<std::string> generatePseudoLegalMoves(Board& board); // Renamed old function
std::vector<std::string> generateLegalMoves(Board& board);      // New validation function
std::string search(Board& board);

#endif