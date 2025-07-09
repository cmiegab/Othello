import controller;
import board;
import tui;
import repository;
#include <iostream>
#include <optional>
#include <QString>


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
			m_view.messageSkip(m_board.getCurrentPlayer());
			m_board.switchPlayer();
			validMoves = m_board.genMoves(m_board.getCurrentPlayer());
			if (!m_board.hasValidMove(validMoves)) {
				m_gameRunning = false;
                system("cls");
                m_view.displayScore(m_board);
				break;
			}
		}
        if (!m_skipDisplayUpdate) {
            system("cls");
        }
        updateDisplay(validMoves);
		QString input = m_view.getPlayerInput();
		ParsedCommand command = m_view.parseCommandLineInput(input);
        m_skipDisplayUpdate = false;
		handleCommand(command);
	}
}

void Controller::handleCommand(const ParsedCommand& command) {
    switch (command.type) {
    case CommandType::HELP:
        m_view.showHelp();
		m_skipDisplayUpdate = true;
        break;

    case CommandType::QUIT:
        m_gameRunning = false;
		system("cls");
		m_view.messageEndGame();
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
			m_view.messageInvalidInput();
        }
        break;

    case CommandType::INVALID:
        m_view.messageInvalidInput();
        break;
    }
}

void Controller::makeMove(size_t idx) {
    // Generate valid moves for current player
    BitBoard validMoves = m_board.genMoves(m_board.getCurrentPlayer());

    // Check if the move is valid
    if (!m_board.isValidMove(validMoves, idx)) {
		m_view.messageInvalidInput();
        return;
    }

    // Make the move
    m_board.makeMove(m_board.getCurrentPlayer(), idx);

    // Switch to the other player
    m_board.switchPlayer();
}

void Controller::updateDisplay(const BitBoard& moves) {
    // Display the board
	m_view.displayMessage();
    m_view.updateBoard(m_board, moves);
	// Display the current player
	m_view.displayCurrentPlayer(m_board.getCurrentPlayer());
    // Display current scores
    m_view.displayScore(m_board);
    
}

void Controller::saveGame()
{
    m_view.messageSavingGame();
    GameState state = getCurrentGameState();
    m_repository->saveGame(state);
}

void Controller::loadGame()
{
    m_view.messageLoadGame();
    GameState state = m_repository->loadGame();
    setGameState(state);
}

GameState Controller::getCurrentGameState() const
{
    return GameState{
        m_board.getCurrentPlayer(),
        m_board.getBlackPieces(),
        m_board.getWhitePieces()
       };
}

void Controller::setGameState(const GameState& state) const
{
	m_board.setCurrentPlayer(state.currentPlayer);
	m_board.setState(state.blackPieces, state.whitePieces);
}
