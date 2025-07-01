import board;
#include <bitset>
#include <iostream>

size_t bitIndex(size_t row, size_t col) {
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
	}
    return moves;
}


/**
* @brief Generates all possible moves for a given player using Dumb7Fill flood algorithm.
* @param player The player for whom to generate moves (WHITE or BLACK).
* @return A BitBoard representing all possible moves for the player.
*/
void OthelloBoard::setMoves(Player& player, size_t idx) {
	// AvoidWrap masks around during shift operations
	BitBoard verticalMask{ 0x00FFFFFFFFFFFF00 };
	BitBoard horizontalMask{ 0x7E7E7E7E7E7E7E7E };
	BitBoard edgeMask{ 0x007E7E7E7E7E7E00 };

	BitBoard emptySpaces{ ~(m_black | m_white) };
	BitBoard playerPieces{ (player == Player::WHITE) ? m_white : m_black };
	BitBoard opponentPieces{ (player == Player::WHITE) ? m_black : m_white };
	BitBoard v_mask{ opponentPieces & verticalMask };
	BitBoard h_mask{ opponentPieces & horizontalMask };
	BitBoard e_mask{ opponentPieces & edgeMask };
	BitBoard tmp{};
	BitBoard captured_disks{};
	BitBoard bounding_disk{};
	BitBoard new_disk{};
	std::cout << "Setting move for player: " << (player == Player::WHITE ? "WHITE" : "BLACK") << " at index: " << idx << '\n';
	new_disk.set(idx);
	playerPieces |= new_disk;

	for (const Direction& dir : Directions) {
		switch (dir) {
		case Direction::EAST:
		case Direction::WEST:
			tmp = h_mask & shift(new_disk, dir);
			tmp |= h_mask & shift(tmp, dir);
			tmp |= h_mask & shift(tmp, dir);
			tmp |= h_mask & shift(tmp, dir);
			tmp |= h_mask & shift(tmp, dir);
			tmp |= h_mask & shift(tmp, dir);
			bounding_disk = playerPieces & shift(tmp, dir);
			captured_disks |= (bounding_disk.any() ? tmp : 0);
			break;
		case Direction::SOUTH:
		case Direction::NORTH:
			tmp = v_mask & shift(new_disk, dir);
			tmp |= v_mask & shift(tmp, dir);
			tmp |= v_mask & shift(tmp, dir);
			tmp |= v_mask & shift(tmp, dir);
			tmp |= v_mask & shift(tmp, dir);
			tmp |= v_mask & shift(tmp, dir);
			bounding_disk = playerPieces & shift(tmp, dir);
			captured_disks |= (bounding_disk.any() ? tmp : 0);
			break;
		default:
			tmp = e_mask & shift(new_disk, dir);
			tmp |= e_mask & shift(tmp, dir);
			tmp |= e_mask & shift(tmp, dir);
			tmp |= e_mask & shift(tmp, dir);
			tmp |= e_mask & shift(tmp, dir);
			tmp |= e_mask & shift(tmp, dir);
			bounding_disk = playerPieces & shift(tmp, dir);
			captured_disks |= (bounding_disk.any() ? tmp : 0);
			break;
		}
	}
	playerPieces ^= captured_disks;
	opponentPieces ^= captured_disks;
	m_black = (player == Player::BLACK) ? playerPieces : opponentPieces;
	m_white = (player == Player::WHITE) ? playerPieces : opponentPieces;
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
