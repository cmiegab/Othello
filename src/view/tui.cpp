import tui;
#include <iostream>

void printBoard(OthelloBoard board) {
	auto m_black = board.getBlackPieces();
	auto m_white = board.getWhitePieces();
	std::cout << std::format("  A B C D E F G H");
	for (int bit{}; bit < BITBOARD_SIZE; ++bit) {
		if (bit % BITBOARD_WIDTH == 0) {
			std::cout << '\n' << (bit / BITBOARD_WIDTH) + 1 << ' ';
		}
		if (m_black[bit] == 1) {
			std::cout << 'b';
		} else if (m_white[bit] == 1) {
			std::cout << 'w';
		} else {
			std::cout << '.';
		}
		std::cout << ' ';
	}
}

