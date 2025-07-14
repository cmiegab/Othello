// controller.cppm
module;
#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <QObject>
export module controller;
export import board;
export import view;
export import repository;

export class Controller: public QObject {
	Q_OBJECT
public:
	Controller(OthelloBoard& board, View& view, std::unique_ptr<IRepository> repository);
	void initializeGame();

private slots:
	void onSaveRequested();
	void onLoadRequested();
	void onMoveRequest(size_t idx);

private:
	OthelloBoard& m_board;
	View& m_view;
	std::unique_ptr<IRepository> m_repository;

	void updateGameState();

	GameState getCurrentGameState() const;
	void setGameState(const GameState& state) const;

};

#include "controller.moc"