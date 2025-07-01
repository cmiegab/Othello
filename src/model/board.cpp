import board;
#include <bitset>

size_t bitIndex(int row, int col) {
	return row * BITBOARD_WIDTH + col;
}

/**
* @brief Constructs an OthelloBoard with the initial setup.
* Currently only setup for a 8x8 Othello board with the initial four pieces.
*/
OthelloBoard::OthelloBoard() {
  m_white.set(bitIndex(3, 3));
  m_black.set(bitIndex(3, 4));
  m_black.set(bitIndex(4, 3));
  m_white.set(bitIndex(4, 4));
}

/**
* @brief Generates all possible moves for a given player using Dumb7Fill flood algorithm.
* @param player The player for whom to generate moves (WHITE or BLACK).
* @return A BitBoard representing all possible moves for the player.
*/
BitBoard OthelloBoard::generateMoves(Player& player) const {
    // AvoidWrap masks around during shift operations
	BitBoard verticalMask{ 0x00FFFFFFFFFFFF00 };
	BitBoard horizontalMask{ 0x7E7E7E7E7E7E7E7E };
	BitBoard edgeMask{ 0x007E7E7E7E7E7E00 };

    BitBoard moves{};
    BitBoard emptySpaces{ ~(m_black | m_white) }; // Get empty spaces on the board
	BitBoard playerPieces{ (player == Player::WHITE) ? m_white : m_black };
    BitBoard opponentPieces{ (player == Player::WHITE) ? m_black : m_white };
    BitBoard v_mask{ opponentPieces & verticalMask };
    BitBoard h_mask{ opponentPieces & horizontalMask };
    BitBoard e_mask{ opponentPieces & edgeMask };
	BitBoard tmp{};
	
    for(const Direction& dir: Directions) {
        switch(dir) {
            case Direction::EAST:
            case Direction::WEST:
				tmp = h_mask & shift(playerPieces, dir);
				tmp |= h_mask & shift(tmp, dir);
                tmp |= h_mask & shift(tmp, dir);
                tmp |= h_mask & shift(tmp, dir);
                tmp |= h_mask & shift(tmp, dir);
                tmp |= h_mask & shift(tmp, dir);
                moves |= emptySpaces & shift(tmp, dir);
                break;
            case Direction::SOUTH:
            case Direction::NORTH:
				tmp = v_mask & shift(playerPieces, dir);
				tmp |= v_mask & shift(tmp, dir);
				tmp |= v_mask & shift(tmp, dir);
				tmp |= v_mask & shift(tmp, dir);
				tmp |= v_mask & shift(tmp, dir);
				tmp |= v_mask & shift(tmp, dir);
				moves |= emptySpaces & shift(tmp, dir);
				break;
            default:
				tmp = e_mask & shift(playerPieces, dir);
				tmp |= e_mask & shift(tmp, dir);
				tmp |= e_mask & shift(tmp, dir);
				tmp |= e_mask & shift(tmp, dir);
				tmp |= e_mask & shift(tmp, dir);
				tmp |= e_mask & shift(tmp, dir);
				moves |= emptySpaces & shift(tmp, dir);
				break;
		}
		return moves;
	}


    // Generate moves for the given player
    // This is a placeholder implementation
    // Actual move generation logic will depend on the game rules
    return moves;
}

BitBoard OthelloBoard::shift(BitBoard& bitboard, Direction direction) const
{
	int dir{ static_cast<int>(direction) };
    if (dir >= 0) {
        return bitboard << dir;
    }
    else {
		return bitboard >> -dir;
    }
}
