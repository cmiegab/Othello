// controller.cppm
module;
#include <vector>
#include <string>
#include <optional>
export module controller;
export import board;
export import tui;

/**
export struct GameConfig {
	Player startingPlayer{ Player::BLACK };
	bool autoPlay{ false };
	std::vector<size_t> predefinedMoves;
	bool showHelp{ false };
	std::string gameMode{ "interactive" };
};


export class Controller {
public:
	Controller();
	void startGame();
	void switchPlayer();
	bool isGameOver() const;

private:
	OthelloBoard m_board;
	Player m_currentPlayer{ Player::BLACK };
	GameConfig m_config;
	bool m_gameRunning{ true };
};

*/