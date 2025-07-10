// controller.cppm
module;
#include <vector>
#include <string>
#include <optional>
#include <memory>
export module controller;
export import board;
export import tui;
export import repository;

export class Controller {
public:
	Controller(OthelloBoard& board, TUIView& view, std::unique_ptr<IRepository> repository);
	void startGame();
private:
	OthelloBoard& m_board;
	TUIView& m_view;
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