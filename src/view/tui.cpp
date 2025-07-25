module;
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QObject>
#include <optional>
#include <string>
#include <unordered_map>
#include <tuple>
#include <QWinEventNotifier>
#include <Windows.h>
module tui;

TUIView::TUIView() {
	m_parser.setApplicationDescription("Othello Game - QT CLI");
	m_parser.addHelpOption();
	m_parser.addOption({ {"q", "quit"}, "Exit the game." });
	m_parser.addOption({ {"s", "save"}, "Save the game state." });
	m_parser.addOption({ {"l", "load"}, "Load a saved game state." });
	m_parser.addPositionalArgument("move", "Enter a move in the format 'A1', 'B2', etc.");
	
	m_notifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE));
	connect(m_notifier, &QWinEventNotifier::activated, this, &TUIView::onInputReady);
	m_notifier->setEnabled(true); // Enable the notifier to start listening for input

	connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &TUIView::messageEndGame);
}

void TUIView::onInputReady() {
	m_notifier->setEnabled(false); // Disable the notifier to prevent re-entrancy
	QTextStream in(stdin);
	parseCommandLineInput(in.readLine());
	emit inputReady(); // Emit signal to notify that input is ready
	m_notifier->setEnabled(true); // Re-enable the notifier for future input
}

void TUIView::showHelp() {
	QTextStream out(stdout);
	out << m_parser.helpText() << Qt::endl;
}

void TUIView::updateBoard(const OthelloBoard& board, const BitBoard& validMoves)
{
	QTextStream out(stdout);
	BitBoard black = board.getBlackPieces();
	BitBoard white = board.getWhitePieces();

	out << "  A B C D E F G H";;
	for (int bit{}; bit < BITBOARD_SIZE; ++bit) {
		if (bit % BITBOARD_WIDTH == 0) {
			out << '\n' << (bit / BITBOARD_WIDTH) + 1 << ' ';
		}
		if (black[bit] == 1) {
			out << 'B';
		}
		else if (white[bit] == 1) {
			out << 'W';
		}
		else if (validMoves[bit] == 1) {
			out << '*';
		}
		else {
			out << '.';
		}
		out << ' ';
	}
	out << Qt::endl;
}

void TUIView::displayCurrentPlayer(Player player)
{
	QTextStream out(stdout);
	out << "\nCurrent Player: " << (player == Player::BLACK ? "Black" : "White") << Qt::endl;
}

void TUIView::displayScore(const OthelloBoard& board)
{
	QTextStream out(stdout);
	size_t blackScore = std::get<0>(board.getScore());
	size_t whiteScore = std::get<1>(board.getScore());
	out << "Scores - Black: " << blackScore << ", White: " << whiteScore << Qt::endl;
}

void TUIView::clearScreen()
{
	std::system("cls"); // Use "cls" for Windows
}

void TUIView::updateDisplay(const OthelloBoard& board, const BitBoard& validMoves)
{
	clearScreen();
	displayMessage();
	updateBoard(board, validMoves);
	displayCurrentPlayer(board.getCurrentPlayer());
	displayScore(board);
	QTextStream out(stdout);
	out << "Enter your move (e.g., A1, B2) or command (q, s, l): ";
	if (m_showHelp) {
		showHelp();
		m_showHelp = false; 
	}
}


void TUIView::messageEndGame()
{
	clearScreen();
	displayScore(OthelloBoard());
	m_message = "Thanks for Playing! Press any key to exit";
	displayMessage();
}

void TUIView::invalidMove()
{
	m_message = "Invalid move! Please try again.";
}


void TUIView::messageSkip(Player player)
{
	QTextStream out(stdout);
	out << "\n*** " << (player == Player::BLACK ? "Black" : "White")
		<< " player has no valid moves and must skip their turn! ***\n\n";
}


void TUIView::parseCommandLineInput(const QString& input)
{
	m_message.clear();
	QStringList args = input.split(' ');
	m_parser.parse(args);
	//ParsedCommand command;
	//command.type = CommandType::INVALID; // Default to invalid command
	//command.moveIndex = std::nullopt; // Default to no move index
	
	if (!args.isEmpty()) {
		QString firstArg = args.first().toLower();
		if (firstArg == "q" || firstArg == "quit") {
			//command.type = CommandType::QUIT;
			messageEndGame();
			emit quitRequested(); // Emit signal to notify that quit was requested
			return; // Return early to avoid further processing
		}
		else if (firstArg == "s" || firstArg == "save") {
			//command.type = CommandType::SAVE;
			m_message = "Saving game state";
			emit saveRequested(); // Emit signal to notify that save was requested
			return; // Return early to avoid further processing
		}
		else if (firstArg == "l" || firstArg == "load") {
			//command.type = CommandType::LOAD;
			m_message = "Game state loaded";
			emit loadRequested(); // Emit signal to notify that load was requested
			return;
		}
		else if (firstArg == "h" || firstArg == "help") {
			//command.type = CommandType::HELP;
			m_showHelp = true;
			return; // Return early to avoid further processing
		}
		else if (firstArg.size() == 2) {
			if (isValidBoardPosition(firstArg)) {
				m_message = "set position " + firstArg.toUpper();
				//command.type = CommandType::MOVE;
				auto idx = parseBoardPosition(firstArg);
				if (idx.has_value()) {
					emit moveRequested(idx.value());
				}
				return; // Return early after processing a valid move
			}
			else {
				m_message = "Invalid board position: " + firstArg;
			}
		}
		else {
			m_message = "Invalid command: " + firstArg;
		}
	}
	//emit commandParsed(command); // Emit the parsed command
	//return command;
}

std::optional<size_t> TUIView::parseBoardPosition(const QString& position)
{
	size_t row = static_cast<size_t>(position[1].digitValue() - 1); // Convert '1'-'8' to 0-7
	size_t col = static_cast<size_t>(position[0].unicode() - 'a'); // Convert 'a'-'h' to 0-7
	return (row * BITBOARD_WIDTH + col); // Calculate the bit index
}

bool TUIView::isValidBoardPosition(const QString& position) const
{
	if (position[0].isLetter() && position[1].isDigit()) {
		size_t col = static_cast<size_t>(position[0].toLower().unicode() - 'a');
		size_t row = static_cast<size_t>(position[1].digitValue() - 1);
		return (col >= 0 && col < BITBOARD_WIDTH) && (row >= 0 && row < BITBOARD_WIDTH);
	}

	return false;
}


void TUIView::displayMessage() const
{
	if (!m_message.isEmpty()) {
		QTextStream out(stdout);
		out << "*** " << m_message << " ***\n\n";
	}
}

//#include "tui.moc"