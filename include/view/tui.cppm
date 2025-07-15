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

export class TUIView: public View  {
	Q_OBJECT
public:
	TUIView();
	void messageSkip(Player player) override;
	void invalidMove() override;
	void updateDisplay(const OthelloBoard& board, const BitBoard& validMoves) override;

	void displayCurrentPlayer(Player player);
	void displayScore(const OthelloBoard& board);
	void updateBoard(const OthelloBoard& board, const BitBoard& validMoves);
	void showHelp();
	void displayMessage() const;
	void clearScreen();
	void parseCommandLineInput(const QString& input);
	std::optional<size_t> parseBoardPosition(const QString& position);
	void messageEndGame();

signals:
	void inputReady();

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