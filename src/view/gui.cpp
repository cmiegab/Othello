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

