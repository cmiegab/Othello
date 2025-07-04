module;
#include <fstream>
module file_repository;


void FileRepository::saveGame(const GameState& state) {
	std::ofstream outFile("game_state.txt");
	if (!outFile) {
		throw std::runtime_error("Failed to open file for saving game state.");
	}
	outFile << static_cast<int>(state.currentPlayer) << "\n";
	outFile << state.blackPieces.to_string() << "\n";
	outFile << state.whitePieces.to_string() << "\n";
	outFile.close();
}

GameState FileRepository::loadGame() {
	std::ifstream inFile("game_state.txt");
	if (!inFile) {
		throw std::runtime_error("Failed to open file for loading game state.");
	}
	
	GameState state;
    int playerInt;
    if (!(inFile >> playerInt)) {
        throw std::runtime_error("Failed to read current player from file");
    }
    state.currentPlayer = static_cast<Player>(playerInt);

    // Load black pieces bitboard
    std::string blackPiecesStr;
    if (!(inFile >> blackPiecesStr)) {
        throw std::runtime_error("Failed to read black pieces from file");
    }
    state.blackPieces = BitBoard(blackPiecesStr);

    // Load white pieces bitboard
    std::string whitePiecesStr;
    if (!(inFile >> whitePiecesStr)) {
        throw std::runtime_error("Failed to read white pieces from file");
    }
    state.whitePieces = BitBoard(whitePiecesStr);

    inFile.close();
    return state;
}