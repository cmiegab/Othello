// tui.cppm	
module;
#include <iostream>
#include <optional>
#include <string>
export module tui;
export import view;

export class TUIView: public View {
public:
	void showHelp() override;
	void updateBoard(const OthelloBoard& board, const BitBoard& validMoves) override;
	void displayCurrentPlayer(Player player) override;
	void displayScore(const OthelloBoard& board) override;
	void messageSkip(Player player) override;
	void messageEndGame() override;
	void messageSavingGame() override;
	void messageLoadGame() override;
	void messageInvalidInput() override;
	void setMessage(const std::string& message) override;
	void displayMessage() const override;
	void clearMessage() override;
	ParsedCommand parseCommandLineInput(const std::string& input) override;
	std::optional<size_t> parseBoardPosition(const std::string& position) override;
	std::string getPlayerInput() override;
private:
	std::string m_message; // Used to store messages for display
};

