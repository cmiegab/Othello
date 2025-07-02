module;
#include <iostream>
#include <optional>
#include <string>
//#include <tuple>
module tui;

void TUIView::showHelp() {
	std::cout << "\n=== Othello Game - TUI ===\n"
		<< "1. Players take turns placing pieces on the board.\n"
		<< "2. A valid move captures opponent's pieces by sandwiching them.\n"
		<< "3. The game ends when no valid moves are left for either player.\n"
		<< "4. The player with the most pieces at the end wins.\n\n"
		<< "Commands:\n"
		<< "  h or help - Show this help\n"
		<< "  q or quit - Exit the game\n\n"
		<< "Board layout:\n"
		<< "  Columns: A-H (left to right)\n"
		<< "  Rows: 1-8 (top to bottom)\n"
		<< "  Symbols: 'B' = Black, 'W' = White, '*' = Valid move, '.' = Empty\n"
		<< "========================\n\n";
}

void TUIView::updateBoard(const OthelloBoard& board, Player player)
{
	Player blackPlayer = Player::BLACK;
	Player whitePlayer = Player::WHITE;
	BitBoard black = board.getPlayerPieces(blackPlayer);
	BitBoard white = board.getPlayerPieces(whitePlayer);
	auto moves = board.genMoves(player);
	std::cout << std::format("  A B C D E F G H");
	for (int bit{}; bit < BITBOARD_SIZE; ++bit) {
		if (bit % BITBOARD_WIDTH == 0) {
			std::cout << '\n' << (bit / BITBOARD_WIDTH) + 1 << ' ';
		}
		if (black[bit] == 1) {
			std::cout << 'B';
		}
		else if (white[bit] == 1) {
			std::cout << 'W';
		}
		else if (moves[bit] == 1) {
			std::cout << '*';
		}
		else {
			std::cout << '.';
		}
		std::cout << ' ';
	}
}

void TUIView::displayCurrentPlayer(Player player)
{
	std::cout << "Current Player: " << (player == Player::BLACK ? "Black" : "White") << "\n";
}

void TUIView::displayScore(const OthelloBoard& board)
{
	size_t blackScore = std::get<0>(board.getScore());
	size_t whiteScore = std::get<1>(board.getScore());
	std::cout << "Scores - Black: " << blackScore << ", White: " << whiteScore << "\n";
}

void TUIView::displayMessage(const std::string& message)
{
	std::cout << message << "\n";
}

void TUIView::displayGameOver()
{
	
}

std::string TUIView::getUserInput()
{
	std::string input;
	std::cout << "Enter your move (e.g., 'A1', 'B2', or 'h' for help): ";
	std::getline(std::cin, input);
	return input;
}

std::optional<size_t> TUIView::parseMove(const std::string& move) const
{

	return std::optional<size_t>();
}

/*

std::optional<size_t> Controller::parseMove(const std::string& move) const
{
	char col{};
	char row{};
	if (std::isalpha(move[0]) && std::isdigit(move[1])) {
		col = move[0];
		row = move[1];
	}
	else if (std::isdigit(move[0]) && std::isalpha(move[1])) {
		col = move[1];
		row = move[0];
	}
	else {
		return std::nullopt; // Invalid format
	}
	size_t colIndex = col - 'a'; // Convert 'a'-'h' to 0-7
	size_t rowIndex = row - '1'; // Convert '1'-'8' to 0-7
	if (colIndex < BITBOARD_WIDTH && rowIndex < BITBOARD_WIDTH) {
		return rowIndex * BITBOARD_WIDTH + colIndex; // Convert to bit index
	}
	else
	{
		return std::nullopt; // Out of bounds
	}
}

*/