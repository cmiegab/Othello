// tui.cppm	
module;
#include <iostream>
#include <optional>
export module tui;
export import board;

export class TUIView {
public:
	void showHelp();
	void updateBoard(const OthelloBoard& board, Player player);
	void displayCurrentPlayer(Player player);
	void displayScore(const OthelloBoard& board);
	void displayMessage(const std::string& message);
	void displayGameOver();
	std::string getUserInput();
	std::optional<size_t> parseMove(const std::string& move) const;
};

