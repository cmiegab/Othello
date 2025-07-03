module;
#include <bitset>
#include <iostream>
//import board;
module board;


/**
* @brief Constructs the Othello board (8x8) with the initial setup. 
* The initial setup places two black and two white pieces in the center of the board.
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
BitBoard OthelloBoard::genMoves(Player& player) const {
    BitBoard opponentPieces{ getOpponentPieces(player)};
	BitBoard v_mask{ opponentPieces & OthelloBoard::getVerticalMask() };
    BitBoard h_mask{ opponentPieces & OthelloBoard::getHorizontalMask()};
    BitBoard e_mask{ opponentPieces & OthelloBoard::getEdgeMask()};

	BitBoard emptySpaces{ OthelloBoard::getEmptrySpaces() }; // Get empty spaces on the board
	BitBoard playerPieces{ getPlayerPieces(player) };
	BitBoard tmp{};
	BitBoard moves{};

    for(const Direction& dir: Directions) {
        switch(dir) {
            case Direction::EAST:
            case Direction::WEST:
				tmp = floodFill(h_mask, playerPieces, dir);
                moves |= emptySpaces & shift(tmp, dir);
                break;
            case Direction::SOUTH:
            case Direction::NORTH:
				tmp = floodFill(v_mask, playerPieces, dir);
				moves |= emptySpaces & shift(tmp, dir);
				break;
            default:
				tmp = floodFill(e_mask, playerPieces, dir);
				moves |= emptySpaces & shift(tmp, dir);
				break;
		}
	}
    return moves;
}


/**
* @brief Sets the player's move on the board and captures opponent's pieces.
* @param player The player making the move (WHITE or BLACK).
* @param idx The index of the move (0-63) corresponding to the board position.
* @details This function updated both player's pieces and captures opponent's pieces and updates the score.
*/
void OthelloBoard::makeMove(Player& player, size_t idx) {
	BitBoard opponentPieces{ getOpponentPieces(player)};
	BitBoard v_mask{ opponentPieces & OthelloBoard::getVerticalMask() };
	BitBoard h_mask{ opponentPieces & OthelloBoard::getHorizontalMask() };
	BitBoard e_mask{ opponentPieces & OthelloBoard::getEdgeMask() };
	BitBoard tmp{};
	BitBoard captured_disks{};
	BitBoard bounding_disk{};

	BitBoard playerPieces{ getPlayerPieces(player) };
	BitBoard new_disk{};
	new_disk.set(idx);
	playerPieces |= new_disk;

	for (const Direction& dir : Directions) {
		switch (dir) {
		case Direction::EAST:
		case Direction::WEST:
			tmp = floodFill(h_mask, new_disk, dir);
			bounding_disk = playerPieces & shift(tmp, dir);
			captured_disks |= (bounding_disk.any() ? tmp : 0);
			break;
		case Direction::SOUTH:
		case Direction::NORTH:
			tmp = floodFill(v_mask, new_disk, dir);
			bounding_disk = playerPieces & shift(tmp, dir);
			captured_disks |= (bounding_disk.any() ? tmp : 0);
			break;
		default:
			tmp = floodFill(e_mask, new_disk, dir);
			bounding_disk = playerPieces & shift(tmp, dir);
			captured_disks |= (bounding_disk.any() ? tmp : 0);
			break;
		}
	}
	playerPieces ^= captured_disks;
	opponentPieces ^= captured_disks;
	m_black = (player == Player::BLACK) ? playerPieces : opponentPieces;
	m_white = (player == Player::WHITE) ? playerPieces : opponentPieces;
	//m_score = std::tuple{ m_black.count(), m_white.count() };
}

/**
* @brief Checks if there are any valid moves for the player based on the generated moves.
* @param moves The BitBoard representing all possible moves (genMoves).
* @return True if there are valid moves, false otherwise.
*/
bool OthelloBoard::hasValidMove(const BitBoard& moves) const
{
	return moves.any();
}

/**
* @brief Checks if the move at the specified index is valid.
* @param moves The BitBoard representing all possible moves (genMoves).
* @param idx The index of the move to check (0-63).
* @return True if the move is valid, false otherwise.
*/
bool OthelloBoard::isValidMove(const BitBoard& moves, size_t idx) const
{
	return moves[idx];
}

/**
* @brief Gets the pieces of the specified player.
* @param player The player for whom to get the pieces (WHITE or BLACK).
* @return A BitBoard representing the player's pieces.
*/
BitBoard OthelloBoard::getPlayerPieces(Player& player) const
{
	return (player == Player::WHITE) ? m_white : m_black;
}

/**
* @brief Gets the opponent's pieces for the given player.
* @param player The player for whom to get the opponent's pieces (WHITE or BLACK).
* @return A BitBoard representing the opponent's pieces.
*/
BitBoard OthelloBoard::getOpponentPieces(Player& player) const
{
	return (player == Player::WHITE) ? m_black : m_white;
}

/**
* @brief Gets the empty spaces on the board.
* @return A BitBoard representing the empty spaces on the board.
*/
BitBoard OthelloBoard::getEmptrySpaces() const
{
	BitBoard occupied = m_black | m_white;
	return occupied.flip();
}

std::tuple<size_t, size_t> OthelloBoard::getScore() const
{
	return std::tuple{ m_black.count(), m_white.count() };
}


/**
* @brief Shifts the given BitBoard in the specified direction.
* @param bitboard The BitBoard to shift.
* @param direction The direction in which to shift the BitBoard.
* @return A new BitBoard shifted in the specified direction.
*/
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

/**
* @brief Performs a flood fill (Dumb7Fill) operation on the given pieces using the specified mask and direction.
* @param pieces The BitBoard representing the pieces to flood fill.
* @param mask The BitBoard mask to apply during the flood fill.
* @param direction The direction in which to perform the flood fill.
* @return A BitBoard representing the result of the flood fill operation.
*/
BitBoard OthelloBoard::floodFill(const BitBoard& pieces, const BitBoard& mask, Direction direction) const
{
	BitBoard tmp = mask & shift(const_cast<BitBoard&>(const_cast<BitBoard&>(pieces)), direction);
	tmp |= mask & shift(tmp, direction);
	tmp |= mask & shift(tmp, direction);
	tmp |= mask & shift(tmp, direction);
	tmp |= mask & shift(tmp, direction);
	tmp |= mask & shift(tmp, direction);
	return tmp;
}

/**
* @brief Converts row and column indices to a bit index in the BitBoard.
* @param row The row index (0-7).
* @param col The column index (0-7).
* @return The bit index corresponding to the given row and column.
*/
size_t OthelloBoard::bitIndex(size_t row, size_t col) const
{
	return row * BITBOARD_WIDTH + col;
}