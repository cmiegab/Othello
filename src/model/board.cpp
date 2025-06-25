import board;


BitBoard& BitBoard::setBit(uint8_t row, uint8_t col, bool value) {
  if (value) {
    m_bits.set(bitIndex(row, col));
  } else {
    m_bits.reset(bitIndex(row, col));
  }
  return *this;
}

bool BitBoard::getBit(uint8_t row, uint8_t col) {
  return m_bits.test(bitIndex(row, col));
}


// Implementation of OthelloBoard member functions

OthelloBoard::OthelloBoard() {
  m_white.setBit(3, 3);
  m_black.setBit(3, 4);
  m_black.setBit(4, 3);
  m_white.setBit(4, 4);
}

/*
OthelloBoard::getEmptySpacesBitBoard() {
    return ~(m_wh
      )
}

*/
