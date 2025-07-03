// controller.cppm
module;
#include <vector>
#include <string>
#include <optional>
#include <memory>
export module controller;
export import board;
export import tui;

export class Controller {
public:
	Controller(OthelloBoard& board, TUIView& view);
	void startGame();
private:
	OthelloBoard& m_board;
	TUIView& m_view;
	Player m_currentPlayer;
	bool m_gameRunning;

	void gameLoop();
	void handleCommand(const ParsedCommand& command);
	void makeMove(size_t idx);
	void switchPlayer();
	void updateDisplay(const BitBoard& moves);
	bool checkGameOver();
};