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

export enum class CellState {
	EMPTY = 0,
	WHITE = 1,
	BLACK = 2
};

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



// AvoidWrap masks around during shift operations
export constexpr BitBoard VERT_MASK{ 0x00FFFFFFFFFFFF00 };
export constexpr BitBoard HORI_MASK{ 0x7E7E7E7E7E7E7E7E };
export constexpr BitBoard EDGE_MASK{ 0x007E7E7E7E7E7E00 };

export class OthelloBoard {
public:
    OthelloBoard();
	[[nodiscard]] BitBoard generateMoves(Player& player) const;
	[[nodiscard]] const BitBoard& getBlackPieces() const { return m_black; };
	[[nodiscard]] const BitBoard& getWhitePieces() const { return m_white; };

private:
	BitBoard shift(BitBoard& bitboard, Direction direction) const;
    BitBoard m_black;
    BitBoard m_white;
};