import controller;
import board;
import tui;
import repository;
#include <iostream>
#include <optional>
#include <string>


Controller::Controller(OthelloBoard& board, TUIView& view, std::unique_ptr<IRepository> repository) : m_board(board), m_view(view), m_repository(std::move(repository)), m_gameRunning(true)
{
}

void Controller::startGame()
{
	gameLoop();
}

void Controller::gameLoop()
{
	while (m_gameRunning) {
		BitBoard validMoves = m_board.genMoves(m_board.getCurrentPlayer());
		if (!m_board.hasValidMove(validMoves)) {
			m_board.switchPlayer();
			validMoves = m_board.genMoves(m_board.getCurrentPlayer());
			if (!m_board.hasValidMove(validMoves)) {
				m_gameRunning = false;
				break;
			}
		}
		updateDisplay(validMoves);
		std::string input = m_view.getPlayerInput();
		ParsedCommand command = m_view.parseCommandLineInput(input);
		handleCommand(command);
	}
	if (checkGameOver()) {
		m_view.displayScore(m_board);
	}

}

void Controller::handleCommand(const ParsedCommand& command) {
    switch (command.type) {
    case CommandType::HELP:
        m_view.showHelp();
        break;

    case CommandType::QUIT:
        m_gameRunning = false;
        std::cout << "Thanks for playing!\n";
        break;

    case CommandType::SAVE:
		saveGame();
        break;

    case CommandType::LOAD:
		loadGame();
        break;

    case CommandType::MOVE:
        if (command.moveIndex.has_value()) {
            makeMove(command.moveIndex.value());
        }
        else {
			std::cout << "Invalid Input.\n";
        }
        break;

    case CommandType::INVALID:
		std::cout << "InvalidInput.\n";
        break;
    }
}

void Controller::makeMove(size_t idx) {
    // Generate valid moves for current player
    BitBoard validMoves = m_board.genMoves(m_board.getCurrentPlayer());

    // Check if the move is valid
    if (!m_board.isValidMove(validMoves, idx)) {
		std::cout << "Invalid move. Please try again.\n";
        return;
    }

    // Make the move
    m_board.makeMove(m_board.getCurrentPlayer(), idx);

    // Switch to the other player
    m_board.switchPlayer();
}

void Controller::updateDisplay(const BitBoard& moves) {
    // Display the board
    system("cls");
    m_view.updateBoard(m_board, moves);
	// Display the current player
	m_view.displayCurrentPlayer(m_board.getCurrentPlayer());
    // Display current scores
    m_view.displayScore(m_board);
    
}

bool Controller::checkGameOver() {
    // Check if either player has valid moves
	Player blackPlayer = Player::BLACK;
	Player whitePlayer = Player::WHITE;
    BitBoard blackMoves = m_board.genMoves(blackPlayer);
    BitBoard whiteMoves = m_board.genMoves(whitePlayer);

    return !m_board.hasValidMove(blackMoves) && !m_board.hasValidMove(whiteMoves);
}

void Controller::saveGame()
{
    GameState state = getCurrentGameState();
    m_repository->saveGame(state);
}

void Controller::loadGame()
{
    GameState state = m_repository->loadGame();
    setGameState(state);
}

GameState Controller::getCurrentGameState() const
{
    return GameState{
        m_board.getCurrentPlayer(),
        m_board.getWhitePieces(),
        m_board.getBlackPieces()
       };
}

void Controller::setGameState(const GameState& state) const
{
	m_board.setCurrentPlayer(state.currentPlayer);
	m_board.setState(state.blackPieces, state.whitePieces);
}
