module;
#include <iostream>
#include <optional>
#include <QString>
export module gui;
export import view;

export class GUIView : public View {
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
	void setMessage(const QString& message) override;
	void displayMessage() const override;
	void clearMessage() override;
	ParsedCommand parseCommandLineInput(const QString& input) override;
	std::optional<size_t> parseBoardPosition(const QString& position) override;
	QString getPlayerInput() override;
	~GUIView() override = default;
private:
	QString m_message; // Used to store messages for display
};

