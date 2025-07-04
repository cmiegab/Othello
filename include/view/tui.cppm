// tui.cppm	
module;
#include <iostream>
#include <optional>
#include <string>
export module tui;
export import board;

export enum class CommandType {
	HELP,
	QUIT,
	SAVE,
	LOAD,
	MOVE,
	INVALID
};

export struct ParsedCommand {
	CommandType type;
	std::optional<size_t> moveIndex; // Used for MOVE command
};

export class TUIView {
public:
	void showHelp();
	void updateBoard(const OthelloBoard& board, const BitBoard& validMoves);
	void displayCurrentPlayer(Player player);
	void displayScore(const OthelloBoard& board);
	void displaySkip(Player player);
	ParsedCommand parseCommandLineInput(const std::string& input);
	std::optional<size_t> parseBoardPosition(const std::string& position);
	std::string getPlayerInput();
};

