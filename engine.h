#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <cstdint>
#include <thread>
#include <atomic>
#include <functional>
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

int evaluatePosition(Board& board);
double minimax(Board& board, int depth, bool maximizingPlayer);
uint64_t Perft_recursive(Board& board, int depth);
uint64_t Perft_parallel(Board& board, int depth);
void PerftTest(Board& board, int depth);

std::string search(Board& board);

#endif