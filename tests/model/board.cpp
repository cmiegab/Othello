#include <gtest/gtest.h>
#include <chrono>
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

/*
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
*/


TEST(OthelloBoardTest, AVX2GenMovesMatchesScalar) {
	OthelloBoard board;

	// Test initial board state
	BitBoard scalarMovesBlack = board.genMoves(Player::BLACK);
	BitBoard avx2MovesBlack = board.AVX2_genMoves(Player::BLACK);
	EXPECT_EQ(scalarMovesBlack, avx2MovesBlack) << "AVX2 and scalar genMoves should produce identical results for BLACK player";

	BitBoard scalarMovesWhite = board.genMoves(Player::WHITE);
	BitBoard avx2MovesWhite = board.AVX2_genMoves(Player::WHITE);
	EXPECT_EQ(scalarMovesWhite, avx2MovesWhite) << "AVX2 and scalar genMoves should produce identical results for WHITE player";

	// Test after making a few moves
	board.makeMove(Player::BLACK, 19); // Make a move
	board.switchPlayer();

	BitBoard scalarMovesAfterMove = board.genMoves(Player::WHITE);
	BitBoard avx2MovesAfterMove = board.AVX2_genMoves(Player::WHITE);
	EXPECT_EQ(scalarMovesAfterMove, avx2MovesAfterMove) << "AVX2 and scalar genMoves should match after making moves";
}

TEST(OthelloBoardTest, GenMovesBenchmark) {
	constexpr int NUM_ITERATIONS = 100000;
	constexpr int NUM_WARMUP = 1000;

	OthelloBoard board;

	// Warm up the CPU cache and branch predictor
	for (int i = 0; i < NUM_WARMUP; ++i) {
		volatile auto result1 = board.genMoves(Player::BLACK);
		volatile auto result2 = board.AVX2_genMoves(Player::BLACK);
		(void)result1; // Suppress unused variable warning
		(void)result2;
	}

	// Benchmark scalar genMoves
	auto scalarStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < NUM_ITERATIONS; ++i) {
		volatile auto result = board.genMoves(Player::BLACK);
		(void)result; // Suppress unused variable warning
	}
	auto scalarEnd = std::chrono::high_resolution_clock::now();

	// Benchmark AVX2 genMoves
	auto avx2Start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < NUM_ITERATIONS; ++i) {
		volatile auto result = board.AVX2_genMoves(Player::BLACK);
		(void)result; // Suppress unused variable warning
	}
	auto avx2End = std::chrono::high_resolution_clock::now();

	// Calculate durations
	auto scalarDuration = std::chrono::duration_cast<std::chrono::microseconds>(scalarEnd - scalarStart);
	auto avx2Duration = std::chrono::duration_cast<std::chrono::microseconds>(avx2End - avx2Start);

	// Calculate performance metrics
	double scalarTimePerOp = static_cast<double>(scalarDuration.count()) / NUM_ITERATIONS;
	double avx2TimePerOp = static_cast<double>(avx2Duration.count()) / NUM_ITERATIONS;
	double speedup = scalarTimePerOp / avx2TimePerOp;

	// Output results
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\n=== genMoves Performance Benchmark ===" << std::endl;
	std::cout << "Iterations: " << NUM_ITERATIONS << std::endl;
	std::cout << "Scalar genMoves:" << std::endl;
	std::cout << "  Total time: " << scalarDuration.count() << " ?s" << std::endl;
	std::cout << "  Time per operation: " << scalarTimePerOp << " ?s" << std::endl;
	std::cout << "AVX2 genMoves:" << std::endl;
	std::cout << "  Total time: " << avx2Duration.count() << " ?s" << std::endl;
	std::cout << "  Time per operation: " << avx2TimePerOp << " ?s" << std::endl;
	std::cout << "Performance:" << std::endl;
	std::cout << "  Speedup: " << speedup << "x" << std::endl;
	std::cout << "  Performance improvement: " << (speedup - 1.0) * 100.0 << "%" << std::endl;
	std::cout << "=========================================\n" << std::endl;

	// Test should pass regardless of performance, but we can add basic sanity checks
	EXPECT_GT(scalarDuration.count(), 0) << "Scalar implementation should take measurable time";
	EXPECT_GT(avx2Duration.count(), 0) << "AVX2 implementation should take measurable time";

	// Optional: Add performance expectations (uncomment and adjust as needed)
	// EXPECT_GT(speedup, 1.0) << "AVX2 should be faster than scalar implementation";
	// EXPECT_LT(speedup, 10.0) << "Speedup should be reasonable (sanity check)";
}

TEST(OthelloBoardTest, GenMovesVariousStates) {
	constexpr int NUM_ITERATIONS = 10000;
	std::vector<OthelloBoard> testBoards;

	// Create different board states for testing
	testBoards.push_back(OthelloBoard()); // Initial state

	// Create board with some moves
	OthelloBoard board2;
	board2.makeMove(Player::BLACK, 19);
	board2.switchPlayer();
	board2.makeMove(Player::WHITE, 18);
	board2.switchPlayer();
	testBoards.push_back(board2);

	// Create board with more moves
	OthelloBoard board3;
	board3.makeMove(Player::BLACK, 19);
	board3.switchPlayer();
	board3.makeMove(Player::WHITE, 18);
	board3.switchPlayer();
	board3.makeMove(Player::BLACK, 20);
	board3.switchPlayer();
	testBoards.push_back(board3);

	std::cout << "\n=== Multi-State Performance Benchmark ===" << std::endl;

	for (size_t boardIdx = 0; boardIdx < testBoards.size(); ++boardIdx) {
		const auto& board = testBoards[boardIdx];

		// Test both players
		for (auto player : { Player::BLACK, Player::WHITE }) {
			// Benchmark scalar
			auto scalarStart = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < NUM_ITERATIONS; ++i) {
				volatile auto result = board.genMoves(player);
				(void)result;
			}
			auto scalarEnd = std::chrono::high_resolution_clock::now();

			// Benchmark AVX2
			auto avx2Start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < NUM_ITERATIONS; ++i) {
				volatile auto result = board.AVX2_genMoves(player);
				(void)result;
			}
			auto avx2End = std::chrono::high_resolution_clock::now();

			auto scalarDuration = std::chrono::duration_cast<std::chrono::microseconds>(scalarEnd - scalarStart);
			auto avx2Duration = std::chrono::duration_cast<std::chrono::microseconds>(avx2End - avx2Start);

			double scalarTimePerOp = static_cast<double>(scalarDuration.count()) / NUM_ITERATIONS;
			double avx2TimePerOp = static_cast<double>(avx2Duration.count()) / NUM_ITERATIONS;
			double speedup = scalarTimePerOp / avx2TimePerOp;

			std::cout << "Board " << boardIdx << " - " << (player == Player::BLACK ? "BLACK" : "WHITE") << ":" << std::endl;
			std::cout << "  Scalar: " << std::fixed << std::setprecision(3) << scalarTimePerOp << " ?s/op" << std::endl;
			std::cout << "  AVX2: " << avx2TimePerOp << " ?s/op" << std::endl;
			std::cout << "  Speedup: " << speedup << "x" << std::endl;

			// Verify correctness
			EXPECT_EQ(board.genMoves(player), board.AVX2_genMoves(player))
				<< "Results should match for board " << boardIdx << " player " << (player == Player::BLACK ? "BLACK" : "WHITE");
		}
	}

	std::cout << "==========================================\n" << std::endl;
}