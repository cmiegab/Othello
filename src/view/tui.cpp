module;
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <optional>
#include <string>
#include <unordered_map>
#include <tuple>
module tui;

TUIView::TUIView() {
	m_parser.setApplicationDescription("Othello Game - QT CLI");
	m_parser.addHelpOption();
	m_parser.addOption({ {"q", "quit"}, "Exit the game." });
	m_parser.addOption({ {"s", "save"}, "Save the game state." });
	m_parser.addOption({ {"l", "load"}, "Load a saved game state." });
	m_parser.addPositionalArgument("move", "Enter a move in the format 'A1', 'B2', etc.");
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
	if (m_showHelp) {
		showHelp();
		m_showHelp = false; 
	}
}


void TUIView::messageEndGame()
{
	clearScreen();
	setMessage("Thanks for PLaying! Press any key to exit.");
}

void TUIView::messageSavingGame()
{
	setMessage("Saving game state");
}

void TUIView::messageLoadGame()
{
	setMessage("Game state loaded");
}


void TUIView::messageInvalidInput()
{
	setMessage("Invalid input! Please try again.");
}

void TUIView::setMessage(const QString& message)
{
	m_message = message;
}

void TUIView::messageSkip(Player player)
{
	QTextStream out(stdout);
	out << "\n*** " << (player == Player::BLACK ? "Black" : "White")
		<< " player has no valid moves and must skip their turn! ***\n\n";
}


ParsedCommand TUIView::parseCommandLineInput(const QString& input)
{
	m_message.clear();
	QStringList args = input.split(' ');
	m_parser.parse(args);
	ParsedCommand command;
	command.type = CommandType::INVALID; // Default to invalid command
	command.moveIndex = std::nullopt; // Default to no move index
	
	if (!args.isEmpty()) {
		QString firstArg = args.first().toLower();
		if (firstArg == "q" || firstArg == "quit") {
			command.type = CommandType::QUIT;
		}
		else if (firstArg == "s" || firstArg == "save") {
			command.type = CommandType::SAVE;
		}
		else if (firstArg == "l" || firstArg == "load") {
			command.type = CommandType::LOAD;
		}
		else if (firstArg == "h" || firstArg == "help") {
			command.type = CommandType::HELP;
			m_showHelp = true;
		}
		else if (firstArg.size() == 2 && firstArg[0].isLetter() && firstArg[1].isDigit()) {
			command.type = CommandType::MOVE;
			command.moveIndex = parseBoardPosition(firstArg);
			setMessage(" set position " + firstArg.toUpper());
		}
	}
	return command;
}

std::optional<size_t> TUIView::parseBoardPosition(const QString& position)
{
	size_t row = static_cast<size_t>(position[1].digitValue() - 1); // Convert '1'-'8' to 0-7
	size_t col = static_cast<size_t>(position[0].unicode() - 'a'); // Convert 'a'-'h' to 0-7
	return (row * BITBOARD_WIDTH + col); // Calculate the bit index
}

QString TUIView::getPlayerInput()
{
	QTextStream in(stdin);
	QTextStream out(stdout);
	out << "Enter your move (e.g., A1, B2) or command (h for help, q to quit): " << Qt::flush;
	return in.readLine();
}


void TUIView::displayMessage() const
{
	if (!m_message.isEmpty()) {
		QTextStream out(stdout);
		out << "*** " << m_message << " ***\n\n";
	}
}
