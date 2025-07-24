module;
#include <QObject>
module gui;

GUIView::GUIView() = default;

void GUIView::messageSkip(Player player)
{
}

void GUIView::invalidMove()
{
}

void GUIView::updateDisplay(const OthelloBoard& board, const BitBoard& validMoves)
{
	//get score
	auto [black, white] = board.getScore();
	m_blackScore = black;
	m_whiteScore = white;
	m_currentPlayer = board.getCurrentPlayer();
	m_blackPieces = board.getBlackPieces().to_ullong();
	m_whitePieces = board.getWhitePieces().to_ullong();
	m_validMoves = validMoves.to_ullong();

	emit scoreChanged();
	emit currentPlayerChanged();
}

void GUIView::saveGame()
{
	emit saveRequested();
}

void GUIView::loadGame()
{
	emit loadRequested();
}

Q_INVOKABLE bool GUIView::hasBlackPiece(int index) const
{
	return (m_blackPieces >> index) & 1;
}

Q_INVOKABLE bool GUIView::hasWhitePiece(int index) const
{
	return  (m_whitePieces >> index) & 1;
}

Q_INVOKABLE bool GUIView::isValidMove(int index) const
{
	return (m_validMoves >> index) & 1;
}

Q_INVOKABLE void GUIView::makeMove(int index)
{
	emit moveRequested(index);
}

