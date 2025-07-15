module;
#include <optional>
#include <QString>
#include <QObject>
export module view;
export import board;


export class View: public QObject {
	Q_OBJECT
public:
	virtual void messageSkip(Player player) = 0;
	virtual void invalidMove() = 0;
	virtual void updateDisplay(const OthelloBoard& board, const BitBoard& validMoves) = 0;
	virtual ~View() = default;

signals:
	void quitRequested();
	void saveRequested();
	void loadRequested();
	void moveRequested(size_t idx);
};

#include "view.moc"