// controller.cppm
module;
#include <vector>
#include <string>
#include <optional>
#include <memory>
export module controller;
export import board;
export import view;
export import repository;

export class Controller {
public:
	Controller(OthelloBoard& board, View& view, std::unique_ptr<IRepository> repository);
	void startGame();
private:
	OthelloBoard& m_board;
	View& m_view;
	std::unique_ptr<IRepository> m_repository;
	bool m_gameRunning;
	bool m_skipDisplayUpdate = false;

	void gameLoop();
	void handleCommand(const ParsedCommand& command);
	void makeMove(size_t idx);
	void updateDisplay(const BitBoard& moves);

	// Save and load methods
	void saveGame();
	void loadGame();
	GameState getCurrentGameState() const;
	void setGameState(const GameState& state) const;

};