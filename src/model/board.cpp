module;
#include <bitset>
#include <iostream>
#include <immintrin.h>
module board;


/**
* @brief Constructs the Othello board (8x8) with the initial setup. 
* The initial setup places two black and two white pieces in the center of the board.
*/
OthelloBoard::OthelloBoard() {
  m_currentPlayer = Player::BLACK;
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
BitBoard OthelloBoard::genMoves(Player player) const {
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


BitBoard OthelloBoard::AVX2_genMoves(Player player) const
{
	BitBoard opponentPieces{ getOpponentPieces(player) };
	BitBoard v_mask{ opponentPieces & OthelloBoard::getVerticalMask() };
	BitBoard h_mask{ opponentPieces & OthelloBoard::getHorizontalMask() };
	BitBoard e_mask{ opponentPieces & OthelloBoard::getEdgeMask() };
	__m256i qmask = _mm256_set_epi64x(
		h_mask.to_ullong(),
		v_mask.to_ullong(),
		e_mask.to_ullong(),
		e_mask.to_ullong()
	);
	
	__m256i qpieces = _mm256_set1_epi64x(getPlayerPieces(player).to_ullong());
	__m256i qempty = _mm256_set1_epi64x(getEmptrySpaces().to_ullong());
	// Shift right WEST (1), NORTH (8), NORTH_EAST (7), NORTH_WEST (9)
	// Shift left: EAST (1), SOUTH (8), SOUTH_WEST (7), SOUTH_EAST (9)
	__m256i qshift = _mm256_set_epi64x(1, 8, 7, 9);

	// Shift right
	__m256i qflood = _mm256_and_si256(qmask, _mm256_srlv_epi64(qpieces, qshift));
	for (int flood{ 0 }; flood < 6; ++flood) {
		qflood = _mm256_or_si256(qflood, _mm256_and_si256(qmask, _mm256_srlv_epi64(qflood, qshift)));
	}

	__m256i qmoves = _mm256_and_si256(qempty, _mm256_srlv_epi64(qflood, qshift));

	// Shift left
	qflood = _mm256_and_si256(qmask, _mm256_sllv_epi64(qpieces, qshift));
	for (int flood{ 0 }; flood < 6; ++flood) {
		qflood = _mm256_or_si256(qflood, _mm256_and_si256(qmask, _mm256_sllv_epi64(qflood, qshift)));
	}

	qmoves = _mm256_or_si256(qmoves, _mm256_and_si256(qempty, _mm256_sllv_epi64(qflood, qshift)));

	uint64_t result = 0;
	result |= _mm256_extract_epi64(qmoves, 0);
	result |= _mm256_extract_epi64(qmoves, 1);
	result |= _mm256_extract_epi64(qmoves, 2);
	result |= _mm256_extract_epi64(qmoves, 3);

	return BitBoard(result);
}

BitBoard OthelloBoard::AVX512_genMoves(Player player) const
{
	BitBoard opponentPieces{ getOpponentPieces(player) };
	BitBoard v_mask{ opponentPieces & OthelloBoard::getVerticalMask() };
	BitBoard h_mask{ opponentPieces & OthelloBoard::getHorizontalMask() };
	BitBoard e_mask{ opponentPieces & OthelloBoard::getEdgeMask() };
	__m512i qmask = _mm512_set_epi64(
		h_mask.to_ullong(),
		v_mask.to_ullong(),
		e_mask.to_ullong(),
		e_mask.to_ullong(),
		h_mask.to_ullong(),
		v_mask.to_ullong(),
		e_mask.to_ullong(),
		e_mask.to_ullong()
	);
	__m512i qpieces = _mm512_set1_epi64(getPlayerPieces(player).to_ullong());
	__m512i qempty = _mm512_set1_epi64(getEmptrySpaces().to_ullong());
	// Shift amounts for each direction
	// First 4: right shifts (WEST, NORTH, NORTH_EAST, NORTH_WEST)
	// Last 4: left shifts (EAST, SOUTH, SOUTH_WEST, SOUTH_EAST)
	__m512i qshift = _mm512_set_epi64(1, 8, 7, 9, 1, 8, 7, 9);

	// Perform flood fill for all 8 directions simultaneously
	// First 4 lanes use right shift (srlv), last 4 lanes use left shift (sllv)
	__m512i qflood_right = _mm512_and_si512(qmask, _mm512_srlv_epi64(qpieces, qshift));
	__m512i qflood_left = _mm512_and_si512(qmask, _mm512_sllv_epi64(qpieces, qshift));

	// Combine right and left shifts using a mask
	// Lanes 0-3 get right shift results, lanes 4-7 get left shift results
	__mmask8 right_mask = 0x0F; // 00001111 - select first 4 lanes
	__m512i qflood = _mm512_mask_blend_epi64(right_mask, qflood_left, qflood_right);

	// Flood fill iterations
	for (int flood{ 0 }; flood < 6; ++flood) {
		qflood_right = _mm512_or_si512(qflood, _mm512_and_si512(qmask, _mm512_srlv_epi64(qflood, qshift)));
		qflood_left = _mm512_or_si512(qflood, _mm512_and_si512(qmask, _mm512_sllv_epi64(qflood, qshift)));
		qflood = _mm512_mask_blend_epi64(right_mask, qflood_left, qflood_right);
	}

	// Generate moves for all directions
	__m512i qmoves_right = _mm512_and_si512(qempty, _mm512_srlv_epi64(qflood, qshift));
	__m512i qmoves_left = _mm512_and_si512(qempty, _mm512_sllv_epi64(qflood, qshift));
	__m512i qmoves = _mm512_mask_blend_epi64(right_mask, qmoves_left, qmoves_right);

	// Combine all moves from all 8 directions
	uint64_t result = 0;
	__m256i lower = _mm512_extracti64x4_epi64(qmoves, 0);
	result |= _mm256_extract_epi64(lower, 0);
	result |= _mm256_extract_epi64(lower, 1);
	result |= _mm256_extract_epi64(lower, 2);
	result |= _mm256_extract_epi64(lower, 3);

	// Extract upper 256 bits (lanes 4-7)
	__m256i upper = _mm512_extracti64x4_epi64(qmoves, 1);
	result |= _mm256_extract_epi64(upper, 0);
	result |= _mm256_extract_epi64(upper, 1);
	result |= _mm256_extract_epi64(upper, 2);
	result |= _mm256_extract_epi64(upper, 3);
	return BitBoard(result);
}


/**
* @brief Sets the player's move on the board and captures opponent's pieces.
* @param player The player making the move (WHITE or BLACK).
* @param idx The index of the move (0-63) corresponding to the board position.
* @details This function updated both player's pieces and captures opponent's pieces and updates the score.
*/
void OthelloBoard::makeMove(Player player, size_t idx) {
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
}

void OthelloBoard::AVX2_makeMove(Player player, size_t idx)
{
	BitBoard opponentPieces{ getOpponentPieces(player) };
	BitBoard v_mask{ opponentPieces & OthelloBoard::getVerticalMask() };
	BitBoard h_mask{ opponentPieces & OthelloBoard::getHorizontalMask() };
	BitBoard e_mask{ opponentPieces & OthelloBoard::getEdgeMask() };
	__m256i qmask = _mm256_set_epi64x(
		h_mask.to_ullong(),
		v_mask.to_ullong(),
		e_mask.to_ullong(),
		e_mask.to_ullong()
	);

	BitBoard playerPieces{ getPlayerPieces(player) };
	__m256i qpieces = _mm256_set1_epi64x(playerPieces.to_ullong());

	BitBoard new_disk{1ULL << idx };
	playerPieces |= new_disk;
	__m256i qnew_disk = _mm256_set1_epi64x(new_disk.to_ullong());

	// Shift right WEST (1), NORTH (8), NORTH_EAST (7), NORTH_WEST (9)
	// Shift left: EAST (1), SOUTH (8), SOUTH_WEST (7), SOUTH_EAST (9)
	__m256i qshift = _mm256_set_epi64x(1, 8, 7, 9);

	// Shift right directions
	__m256i qflood = _mm256_and_si256(qmask, _mm256_srlv_epi64(qnew_disk, qshift));
	for (int i = 0; i < 6; ++i) {
		qflood = _mm256_or_si256(qflood, _mm256_and_si256(qmask, _mm256_srlv_epi64(qflood, qshift)));
	}
	__m256i qcaptured_disks = _mm256_setzero_si256();
	__m256i qbounding_disk = _mm256_and_si256(qpieces, _mm256_srlv_epi64(qflood, qshift));
	// Create mask for lanes where bounding disk exists (non-zero)
	__m256i qmask_valid = _mm256_cmpeq_epi64(qbounding_disk, _mm256_setzero_si256());
	qmask_valid = _mm256_xor_si256(qmask_valid, _mm256_set1_epi64x(-1)); // Invert to get valid lanes
	qcaptured_disks = _mm256_or_si256(qcaptured_disks, _mm256_and_si256(qflood, qmask_valid));

	// Shift left directions
	qflood = _mm256_and_si256(qmask, _mm256_sllv_epi64(qnew_disk, qshift));
	for (int i = 0; i < 6; ++i) {
		qflood = _mm256_or_si256(qflood, _mm256_and_si256(qmask, _mm256_sllv_epi64(qflood, qshift)));
	}

	qbounding_disk = _mm256_and_si256(qpieces, _mm256_sllv_epi64(qflood, qshift));
	// Create mask for lanes where bounding disk exists (non-zero)
	qmask_valid = _mm256_cmpeq_epi64(qbounding_disk, _mm256_setzero_si256());
	qmask_valid = _mm256_xor_si256(qmask_valid, _mm256_set1_epi64x(-1)); // Invert to get valid lanes
	qcaptured_disks = _mm256_or_si256(qcaptured_disks, _mm256_and_si256(qflood, qmask_valid));

	// Combine all captured disks from all directions
	uint64_t captured_disks = 0;
	captured_disks |= _mm256_extract_epi64(qcaptured_disks, 0);
	captured_disks |= _mm256_extract_epi64(qcaptured_disks, 1);
	captured_disks |= _mm256_extract_epi64(qcaptured_disks, 2);
	captured_disks |= _mm256_extract_epi64(qcaptured_disks, 3);

	BitBoard captured_disks_bb{ captured_disks };
	playerPieces |= captured_disks_bb;
	opponentPieces ^= captured_disks_bb;

	m_black = (player == Player::BLACK) ? playerPieces : opponentPieces;
	m_white = (player == Player::WHITE) ? playerPieces : opponentPieces;
}

void OthelloBoard::AVX512_makeMove(Player player, size_t idx)
{

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
BitBoard OthelloBoard::getPlayerPieces(Player player) const
{
	return (player == Player::WHITE) ? m_white : m_black;
}

/**
* @brief Gets the opponent's pieces for the given player.
* @param player The player for whom to get the opponent's pieces (WHITE or BLACK).
* @return A BitBoard representing the opponent's pieces.
*/
BitBoard OthelloBoard::getOpponentPieces(Player player) const
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


void OthelloBoard::switchPlayer()
{
	m_currentPlayer = static_cast<Player>(std::to_underlying(m_currentPlayer) ^ 1);
}

void OthelloBoard::setState(const BitBoard& black, const BitBoard& white)
{
	m_black = black;
	m_white = white;
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
BitBoard OthelloBoard::floodFill(const BitBoard& mask, const BitBoard& pieces, Direction direction) const
{
	BitBoard flood = mask & shift(const_cast<BitBoard&>(const_cast<BitBoard&>(pieces)), direction);
	for (int i = 0; i < 6; ++i) {
		flood |= mask & shift(flood, direction);
	}
	return flood;
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
