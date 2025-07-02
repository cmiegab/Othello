import controller;
#include <iostream>
#include <optional>
#include <string>

/*
Controller::Controller() : m_board{}, m_currentPlayer{Player::BLACK} {
}

void Controller::switchPlayer() {
	m_currentPlayer = (m_currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
}


void Controller::startGame() {
	std::cout << "Starting Othello game...\n";
	printHelp();
	while(m_gameRunning) {
		updateDisplay();
		std::cout << "\nCurrent player: " << (m_currentPlayer == Player::BLACK ? "BLACK (b)" : "WHITE (w)") << "\n";
		std::cout << "Enter move (e.g., 'a1', '1a', '1A') or command ('h' for help, 'q' to quit): ";
		std::string input;
		std::getline(std::cin, input);

		for (auto& c : input) {
			c = std::tolower(c); // Normalize input to lowercase
		}

		if (input == "q" || input == "quit") {
			m_gameRunning = false;
			std::cout << "Exiting the game.\n";
			break;
		} else if (input == "h" || input == "help") {
			m_config.showHelp = true;
			continue;
		}


		if (m_config.showHelp) {
			printHelp();
			m_config.showHelp = false; // Reset after showing help
		}
		std::string input;
		std::cout << "Player " << (m_currentPlayer == Player::BLACK ? "Black" : "White") << ", enter your move (e.g., A1, B2) or 'h' for help: ";
		std::getline(std::cin, input);
		if (input == "q" || input == "quit") {
			m_gameRunning = false;
			break;
		} else if (input == "h" || input == "help") {
			m_config.showHelp = true;
			continue;
		}
		else {
			auto position = parseMove(input);
			if (position.has_value()) {
				if (makeMove(position.value())) {
					switchPlayer();
				} else {
					std::cout << "Invalid move. Try again.\n";
				}
			} else {
				std::cout << "Invalid input format. Please enter a valid move (e.g., A1, B2).\n";
			}

		}
		if (isGameOver()) {
			updateDisplay();
			std::cout << "Final Score - Black: " << std::get<0>(m_score) << ", White: " << std::get<1>(m_score) << "\n";
		}
	}

}


bool Controller::isGameOver() const {
	// Check if there are no valid moves left for both players
	return 0;
}

*/