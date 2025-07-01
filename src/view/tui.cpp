import tui;
#include <iostream>

void printBoard(OthelloBoard board, Player player) {
	auto black = board.getBlackPieces();
	auto white = board.getWhitePieces();
	auto moves = board.generateMoves(player);
	std::cout << std::format("  A B C D E F G H");
	for (int bit{}; bit < BITBOARD_SIZE; ++bit) {
		if (bit % BITBOARD_WIDTH == 0) {
			std::cout << '\n' << (bit / BITBOARD_WIDTH) + 1 << ' ';
		}
		if (black[bit] == 1) {
			std::cout << 'b';
		} else if (white[bit] == 1) {
			std::cout << 'w';
		} else if (moves[bit] == 1) {
			std::cout << 'x';
		} else {
			std::cout << '.';
		}
		std::cout << ' ';
	}
}

