#include <gtest/gtest.h>
import board;


TEST(OthelloBoardTest, InitialSetup) {
	OthelloBoard board;
	Player white = Player::WHITE;
	Player black = Player::BLACK;
	BitBoard wPieces = board.getPlayerPieces(white);
	BitBoard bPieces = board.getPlayerPieces(black);
	BitBoard emptySpaces = board.getEmptrySpaces();
	EXPECT_EQ(emptySpaces.count(), 60); // 64 total spaces - 4 initial pieces
	EXPECT_EQ(wPieces.count(), 2);
	EXPECT_EQ(bPieces.count(), 2);
	EXPECT_TRUE(wPieces[27]);
	EXPECT_TRUE(bPieces[28]);
	EXPECT_TRUE(bPieces[35]);
	EXPECT_TRUE(wPieces[36]);
}

TEST(OthelloBoardTest, ValidMoves) {
	OthelloBoard board;
	Player white = Player::WHITE;
	Player black = Player::BLACK;
	// Test valid moves for white
	auto validMovesBlack = board.genMoves(black);
	EXPECT_EQ(validMovesBlack.count(), 4); // 4 valid moves
	EXPECT_FALSE(validMovesBlack[19]); // Top-left corner
	EXPECT_FALSE(validMovesBlack[26]); // Top-right corner
	EXPECT_FALSE(validMovesBlack[37]); // Bottom-left corner
	EXPECT_FALSE(validMovesBlack[44]); // Bottom-right corner
}
