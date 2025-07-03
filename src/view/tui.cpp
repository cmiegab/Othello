module;
#include <iostream>
#include <optional>
#include <string>
#include <ranges>
#include <algorithm>
#include <unordered_map>
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
		<< "  s or save - Save the game state\n"
		<< "  l or load - Load a saved game state\n"
		<< "Board layout:\n"
		<< "  Columns: A-H (left to right)\n"
		<< "  Rows: 1-8 (top to bottom)\n"
		<< "  Symbols: 'B' = Black, 'W' = White, '*' = Valid move, '.' = Empty\n"
		<< "Example move: 'D3' or 'd3' (Column D, Row 3)\n"
		<< "========================\n\n";
}

void TUIView::updateBoard(const OthelloBoard& board, const BitBoard& validMoves)
{
	BitBoard black = board.getBlackPieces();
	BitBoard white = board.getWhitePieces();

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
		else if (validMoves[bit] == 1) {
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

void TUIView::displaySkip(Player player)
{
	std::cout << "\n*** " << (player == Player::BLACK ? "Black" : "White")
		<< " player has no valid moves and must skip their turn! ***\n\n";
}

ParsedCommand TUIView::parseCommandLineInput(const std::string& input)
{
	ParsedCommand command;
	command.type = CommandType::INVALID; // Default to invalid command
	command.moveIndex = std::nullopt; // Default to no move index
	
	std::string lowerInput = input;
	std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

	static const std::unordered_map<std::string, CommandType> commandMap = {
		{"h", CommandType::HELP},
		{"help", CommandType::HELP},
		{"q", CommandType::QUIT},
		{"quit", CommandType::QUIT},
		{"s", CommandType::SAVE},
		{"save", CommandType::SAVE},
		{"l", CommandType::LOAD},
		{"load", CommandType::LOAD},
	};
	auto it = commandMap.find(lowerInput);
	if (lowerInput.size() == 2 &&
		lowerInput[0] >= 'a' && lowerInput[0] <= 'h' &&
		lowerInput[1] >= '1' && lowerInput[1] <= '8') {
		command.type = CommandType::MOVE;
		command.moveIndex = parseBoardPosition(lowerInput);
	}
	else if (it != commandMap.end()) {
		command.type = it->second;
	}
	else {
		command.type = CommandType::INVALID;
	}
	return command;
}

std::optional<size_t> TUIView::parseBoardPosition(const std::string& position)
{
	size_t row = static_cast<size_t>(position[1] - '1'); // Convert '1'-'8' to 0-7
	size_t col = static_cast<size_t>(position[0] - 'a'); // Convert 'a'-'h' to 0-7
	return (row * BITBOARD_WIDTH + col); // Calculate the bit index
}

std::string TUIView::getPlayerInput()
{
	std::string input;
	std::cout << "Enter your move (e.g., A1, B2) or command (h for help, q to quit): ";
	std::getline(std::cin, input);
	return input;
}

/*
std::string TUIView::getPlayerInput() {
	std::string input;
	std::cout << "Enter your move (e.g., A1, B2) or command (h for help, q to quit): ";
	std::getline(std::cin, input);
	return input;
}
*/