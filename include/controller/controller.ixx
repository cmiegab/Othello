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
	Controller();
	void startGame();
private:
	void gameLoop();
	void handleCommand(const ParsedCommand& command);
	void makeMove(size_t idx);
	void switchPlayer();
	void updateDisplay();
	bool checkGameOver();
	std::unique_ptr<OthelloBoard> m_board;
	std::unique_ptr<TUIView> m_view;
	Player m_currentPlayer;
	bool m_gameRunning;
};