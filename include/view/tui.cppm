// tui.cppm	
module;
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <optional>
#include <string>
#include <QWinEventNotifier>
#include <Windows.h>
#include <QObject>
export module tui;
export import view;

export class TUIView: public QObject , public View  {
	Q_OBJECT
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
	void clearScreen();
	void updateDisplay(const OthelloBoard& board, const BitBoard& validMoves) override;
	ParsedCommand parseCommandLineInput(const QString& input) override;
	std::optional<size_t> parseBoardPosition(const QString& position) override;

signals:
	void inputReady();
	void commandParsed(const ParsedCommand& command);

private slots:
	void onInputReady();

private:
	QWinEventNotifier* m_notifier = nullptr;
	bool isValidBoardPosition(const QString& position) const;
	bool m_showHelp = false;
	QCommandLineParser m_parser;
	QString m_message; // Used to store messages for display
};

#include "tui.moc"