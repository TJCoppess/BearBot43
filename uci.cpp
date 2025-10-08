#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include "uci.h"
#include "board.h"
#include "engine.h"

std::vector<std::string> parseCommand(std::string command) {
	std::vector<std::string> splitCommand;
	std::stringstream ss(command);
	std::string word;
	while (ss >> word) {
		splitCommand.push_back(word);
	}	
	return splitCommand;
}

int main() {
	srand(static_cast<unsigned int>(time(NULL))); // Add the static_cast
	bool isRunning = true;
	Board board;
	while (isRunning) {
		std::string command;
		getline(std::cin, command);
		std::vector<std::string> commandSegments = parseCommand(command);

		
		if (commandSegments.empty()) {
			continue;
		}
		
		if (commandSegments[0] == "uci") {
			std::cout << "id name BearBot\n"
				 << "id author Trevor Coppess\n"
			     << "uciok" << std::endl;
		} else if (commandSegments[0] == "isready") {
			std::cout << "readyok" << std::endl;
		} else if (commandSegments[0] == "position") {
			// I don't think I will need this implementation right away, so I will save it for later.
            // Example: "position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4"
            
            
            // I think that this is the main sting of this command, so this is where I will start.
            // Example: "position startpos moves e2e4 e7e5"
            if (commandSegments.size() > 1 && commandSegments[1] == "startpos") {
            	board.initializeBoard();
            	if (commandSegments.size() > 2 && commandSegments[2] == "moves") {
            		if (commandSegments.size() > 3) {
            			for (int i = 3; i < commandSegments.size(); i++) {
            				board.pushMove(commandSegments[i]);
						}
					}
				}
			}
		} else if (commandSegments[0] == "ucinewgame") {
			board.initializeBoard();
			std::cout << "info new game initialized" << std::endl;
		} else if (commandSegments[0] == "go") {
			// I don't think I will need this implementation right away, so I will save it for later.
			// This is where your engine starts thinking.
            // Examples:
            // "go movetime 5000"
            // "go depth 5"
            // "go infinite"
            
            
			// I think that this is the main string of this command, so this is where I will start.
			// "go wtime 300000 btime 300000 winc 2000 binc 2000"
			if (commandSegments.size() >= 2 && commandSegments[1] == "wtime") {
				board.setWTime(std::stoi(commandSegments[2]));
			}
			if (commandSegments.size() >= 4 && commandSegments[3] == "btime") {
				board.setBTime(std::stoi(commandSegments[4]));
			}
			if (commandSegments.size() >= 6 && commandSegments[5] == "winc") {
				board.setWInc(std::stoi(commandSegments[6]));
			}
			if (commandSegments.size() >= 8 && commandSegments[7] == "binc") {
				board.setBInc(std::stoi(commandSegments[8]));
			}
			
			// 3. If there are legal moves, find the best one and send it.
			std::string bestMove = search(board);
			std::cout << "bestmove " << bestMove << std::endl;
					
		} else if (commandSegments[0] == "quit") {
			isRunning = false;
		} else if (commandSegments[0] == "print") {
			board.printBoard();
		} else if (commandSegments[0] == "perft") {
		    if (commandSegments.size() > 1) {
		        int depth = std::stoi(commandSegments[1]);
		        PerftTest(board, depth);
		    } else {
		        std::cout << "Usage: perft <depth>" << std::endl;
		    }
		} else {
			std::cout << "I don't know that command yet: " << command << std::endl;
		}
	}
	
	return 0;
}