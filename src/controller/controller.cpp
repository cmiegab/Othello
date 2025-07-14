module;
#include <iostream>
#include <optional>
#include <QString>
#include <QObject>
#include <QCoreApplication>
import board;
import view;
import repository;
module controller;

Controller::Controller(OthelloBoard& board, View& view, std::unique_ptr<IRepository> repository) : m_board(board), m_view(view), m_repository(std::move(repository)), m_gameRunning(true)
{
	QObject::connect(&m_view, &View::quitRequested, this, &Controller::onQuitRequested);
	QObject::connect(&m_view, &View::saveRequested, this, &Controller::onSaveRequested);
	QObject::connect(&m_view, &View::loadRequested, this, &Controller::onLoadRequested);
	QObject::connect(&m_view, &View::moveRequested, this, &Controller::onMoveRequest);
}

void Controller::initializeGame()
{
    updateGameState();
    QCoreApplication::exec();
}

void Controller::updateGameState()
{
    if (!m_gameRunning) {
        QCoreApplication::quit();
        return;
    }
	BitBoard validMoves = m_board.genMoves(m_board.getCurrentPlayer());
	if (!m_board.hasValidMove(validMoves)) {
		m_view.messageSkip(m_board.getCurrentPlayer());
		m_board.switchPlayer();
		validMoves = m_board.genMoves(m_board.getCurrentPlayer());
		if (!m_board.hasValidMove(validMoves)) {
			m_gameRunning = false;
            m_view.displayScore(m_board);
            m_view.messageEndGame();
            QCoreApplication::quit();
            return;
		}
	}
	m_view.updateDisplay(m_board, validMoves);
}


void Controller::makeMove(size_t idx) {
    // Generate valid moves for current player
    BitBoard validMoves = m_board.genMoves(m_board.getCurrentPlayer());

    // Check if the move is valid
    if (!m_board.isValidMove(validMoves, idx)) {
		m_view.invalidMove();
        return;
    }
    // Make the move
    m_board.makeMove(m_board.getCurrentPlayer(), idx);

    // Switch to the other player
    m_board.switchPlayer();
}

void Controller::onSaveRequested()
{
    GameState state = getCurrentGameState();
    m_repository->saveGame(state);
    updateGameState();
}

void Controller::onLoadRequested()
{
    GameState state = m_repository->loadGame();
    setGameState(state);
    updateGameState();
}

void Controller::onMoveRequest(size_t idx) {
    makeMove(idx);
    updateGameState();
}

void Controller::onQuitRequested() {
    m_gameRunning = false;
    m_view.messageEndGame();
    updateGameState();
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
