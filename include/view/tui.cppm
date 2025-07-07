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
	void messageSkip(Player player);
	void messageEndGame();
	void messageSavingGame();
	void messageLoadGame();
	void messageInvalidInput();
	void setMessage(const std::string& message);
	void displayMessage() const;
	void clearMessage();
	ParsedCommand parseCommandLineInput(const std::string& input);
	std::optional<size_t> parseBoardPosition(const std::string& position);
	std::string getPlayerInput();
private:
	std::string m_message; // Used to store messages for display
};

