module;
#include <optional>
#include <string>
export module view;
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

export class View {
public:
	virtual void showHelp() = 0;
	virtual void updateBoard(const OthelloBoard& board, const BitBoard& validMoves) = 0;
	virtual void displayCurrentPlayer(Player player) = 0;
	virtual void displayScore(const OthelloBoard& board) = 0;
	virtual void messageSkip(Player player) = 0;
	virtual void messageEndGame() = 0;
	virtual void messageSavingGame() = 0;
	virtual void messageLoadGame() = 0;
	virtual void messageInvalidInput() = 0;
	virtual void setMessage(const std::string& message) = 0;
	virtual void displayMessage() const = 0;
	virtual void clearMessage() = 0;
	virtual ParsedCommand parseCommandLineInput(const std::string& input) = 0;
	virtual std::optional<size_t> parseBoardPosition(const std::string& position) = 0;
	virtual std::string getPlayerInput() = 0;
	virtual ~View() = default;
};