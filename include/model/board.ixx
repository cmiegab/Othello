// board.cppm
module;
#include <bitset>
#include <cstdint>
#include <array>
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
	[[nodiscard]] BitBoard genMoves(Player& player) const;
	[[nodiscard]] void makeMove(Player& player, size_t idx);
	bool hasValidMove(const BitBoard& moves) const;
	bool isValidMove(const BitBoard& moves, size_t idx) const;


	BitBoard getPlayerPieces(Player& player) const;
	BitBoard getOpponentPieces(Player& player) const;
	BitBoard getEmptrySpaces() const;
	std::tuple<size_t, size_t> getScore() const { return m_score; };
	BitBoard getVerticalMask() const { return verticalMask; }
	BitBoard getHorizontalMask() const { return horizontalMask; };
	BitBoard getEdgeMask() const { return edgeMask; }

	

private:
	BitBoard shift(BitBoard& bitboard, Direction direction) const;
	BitBoard floodFill(const BitBoard& mask, const BitBoard& pieces, Direction direction) const;
	size_t bitIndex(size_t row, size_t col) const;

	BitBoard verticalMask{ 0x00FFFFFFFFFFFF00 };
	BitBoard horizontalMask{ 0x7E7E7E7E7E7E7E7E };
	BitBoard edgeMask{ 0x007E7E7E7E7E7E00 };

	std::tuple<size_t, size_t> m_score = std::tuple{ m_black.count(), m_white.count() };
    BitBoard m_black;
    BitBoard m_white;
};