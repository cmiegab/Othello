// board.cppm
module;
#include <bitset>
#include <cstdint>
#include <array>
#include <immintrin.h>
export module board;

export constexpr std::int8_t BITBOARD_WIDTH{ 8 };
export constexpr std::int8_t BITBOARD_SIZE{ BITBOARD_WIDTH * BITBOARD_WIDTH };
export constexpr std::int8_t NUM_DIRECTIONS{ 8 };
export using BitBoard = std::bitset<BITBOARD_SIZE>;

export enum class Player { WHITE, BLACK };

export enum class Direction {
	NORTH = -BITBOARD_WIDTH,
	NORTH_EAST = -BITBOARD_WIDTH + 1,
	EAST = 1,
	SOUTH_EAST = BITBOARD_WIDTH + 1,
	SOUTH = BITBOARD_WIDTH,
	SOUTH_WEST = BITBOARD_WIDTH - 1,
	WEST = -1,
	NORTH_WEST = -BITBOARD_WIDTH - 1
};

export constexpr std::array<Direction, NUM_DIRECTIONS> Directions = {
	Direction::NORTH,
	Direction::NORTH_EAST,
	Direction::EAST,
	Direction::SOUTH_EAST,
	Direction::SOUTH,
	Direction::SOUTH_WEST,
	Direction::WEST,
	Direction::NORTH_WEST
};


export class OthelloBoard {
public:
    OthelloBoard();
	[[nodiscard]] BitBoard genMoves(Player player) const;
	[[nodiscard]] BitBoard AVX2_genMoves(Player player) const;
	[[nodiscard]] BitBoard AVX512_genMoves(Player player) const;

	void makeMove(Player player, size_t idx);
	void AVX2_makeMove(Player player, size_t idx);

	[[nodiscard]] bool hasValidMove(const BitBoard& moves) const;
	[[nodiscard]] bool isValidMove(const BitBoard& moves, size_t idx) const;

	[[nodiscard]] BitBoard getPlayerPieces(Player player) const;
	[[nodiscard]] BitBoard getOpponentPieces(Player player) const;
	[[nodiscard]] const BitBoard& getBlackPieces() const { return m_black; };
	[[nodiscard]] const BitBoard& getWhitePieces() const { return m_white; };

	[[nodiscard]] BitBoard getEmptrySpaces() const;
	[[nodiscard]] std::tuple<size_t, size_t> getScore() const;
	[[nodiscard]] const BitBoard& getVerticalMask() const { return verticalMask; }
	[[nodiscard]] const BitBoard& getHorizontalMask() const { return horizontalMask; };
	[[nodiscard]] const BitBoard& getEdgeMask() const { return edgeMask; };

	[[nodiscard]] Player getCurrentPlayer() const { return m_currentPlayer; };
	void setCurrentPlayer(Player player) { m_currentPlayer = player; };
	void switchPlayer();

	void setState(const BitBoard& black, const BitBoard& white);

private:
	BitBoard shift(BitBoard& bitboard, Direction direction) const;
	BitBoard floodFill(const BitBoard& mask, const BitBoard& pieces, Direction direction) const;
	size_t bitIndex(size_t row, size_t col) const;

	static constexpr BitBoard verticalMask{ 0x00FFFFFFFFFFFF00 };
	static constexpr BitBoard horizontalMask{ 0x7E7E7E7E7E7E7E7E };
	static constexpr BitBoard edgeMask{ 0x007E7E7E7E7E7E00 };

	Player m_currentPlayer;
    BitBoard m_black;
    BitBoard m_white;
};