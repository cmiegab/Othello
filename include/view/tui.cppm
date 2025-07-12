// tui.cppm	
module;
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <optional>
#include <string>
export module tui;
export import view;

export class TUIView: public View {
public:
	TUIView();
	void showHelp() override;
	void updateBoard(const OthelloBoard& board, const BitBoard& validMoves) override;
	void displayCurrentPlayer(Player player) override;
	void displayScore(const OthelloBoard& board) override;
	void messageSkip(Player player) override;
	void messageEndGame() override;
	void invalidMove() override;
	void displayMessage() const override;
	void clearScreen() override;
	void updateDisplay(const OthelloBoard& board, const BitBoard& validMoves) override;
	ParsedCommand parseCommandLineInput(const QString& input) override;
	std::optional<size_t> parseBoardPosition(const QString& position) override;
	QString getPlayerInput() override;
private:
	bool isValidBoardPosition(const QString& position) const;
	bool m_showHelp = false;
	QCommandLineParser m_parser;
	QString m_message; // Used to store messages for display
};

