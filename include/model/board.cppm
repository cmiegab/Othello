export module board;
#include <cstdint>
#include <bitset>

export
constexpr std::size_t BITBOARD_WIDTH = 8;
export
constexpr std::size_t BITBOARD_SIZE = BITBOARD_WIDTH * BITBOARD_WIDTH;

export
constexpr std::size_t bitIndex(uint8_t row, uint8_t col) {
  return (row * BITBOARD_WIDTH) + col;
}

export
enum class Player { BLACK, WHITE };

export enum class Direction {
  N = BITBOARD_WIDTH - 8,
  NE = BITBOARD_WIDTH - 7,
  E = BITBOARD_WIDTH - 6,
  SE = BITBOARD_WIDTH - 5,
  S = BITBOARD_WIDTH - 4,
  SW = BITBOARD_WIDTH - 3,
  W = BITBOARD_WIDTH - 2,
  NW = BITBOARD_WIDTH - 1
};


export
class BitBoard {
public:
    using Bits = std::bitset<BITBOARD_SIZE>;
    BitBoard() = default;
    BitBoard& setBit(uint8_t row, uint8_t col, bool value = true);
    bool getBit(uint8_t row, uint8_t col);
    // Overload bitwise operators

    BitBoard operator&(const BitBoard& other) const {
      return BitBoard(m_bits & other.m_bits);
    }

    BitBoard operator|(const BitBoard& other) const {
      return BitBoard(m_bits | other.m_bits);
    }

    BitBoard operator^(const BitBoard& other) const {
      return BitBoard(m_bits ^ other.m_bits);
    }

    BitBoard operator~() const { return BitBoard(~m_bits); }

    BitBoard& operator&=(const BitBoard& other) {
      m_bits &= other.m_bits;
      return *this;
    }

    BitBoard& operator|=(const BitBoard& other) {
      m_bits |= other.m_bits;
      return *this;
    }

    BitBoard& operator^=(const BitBoard& other) {
      m_bits ^= other.m_bits;
      return *this;
    }
    /*

    */


private:
    std::bitset<BITBOARD_SIZE> m_bits;
    explicit BitBoard(const std::bitset<BITBOARD_SIZE>& bits) : m_bits(bits) {}
};

export
class OthelloBoard {
public:
  OthelloBoard();
  std::bitset<BITBOARD_SIZE> getEmptySpacesBitBoard();
  //bool isLegalMove(Player player, uint8_t row, uint8_t col);
  //bool makeMove(Player player, uint8_t row, uint8_t col);
  //BitBoard getValidMoves(Player player);

private:
    BitBoard m_black;
    BitBoard m_white;
};