module;
#include <QObject>
export module gui;
export import view;

export class GUIView : public View {
	Q_OBJECT
	Q_PROPERTY(Player currentPlayer READ getCurrentPlayer NOTIFY currentPlayerChanged)
	Q_PROPERTY(size_t blackScore READ getBlackScore NOTIFY scoreChanged)
	Q_PROPERTY(size_t whiteScore READ getWhiteScore NOTIFY scoreChanged)
public:
	GUIView();
	//Getters
	size_t getBlackScore() const { return m_blackScore; }
	size_t getWhiteScore() const { return m_whiteScore; }
	Player getCurrentPlayer() const { return m_currentPlayer; }

	void messageSkip(Player player) override;
	void invalidMove() override;
	void updateDisplay(const OthelloBoard& board, const BitBoard& validMoves) override;

	Q_INVOKABLE void saveGame();
	Q_INVOKABLE void loadGame();

signals:
	void scoreChanged();
	void currentPlayerChanged();

private:
	Player m_currentPlayer{};
	size_t m_blackScore{};
	size_t m_whiteScore{};
};

#include "gui.moc" 