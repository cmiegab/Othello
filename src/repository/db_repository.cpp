module;
#include <stdexcept>
module db_repository;


void DBRepository::saveGame(const GameState& state) {
    // TODO: Implement database save functionality
    // For now, throw an exception to indicate it's not implemented
    throw std::runtime_error("Database save functionality not yet implemented");
}

GameState DBRepository::loadGame() {
    // TODO: Implement database load functionality
    // For now, throw an exception to indicate it's not implemented
    throw std::runtime_error("Database load functionality not yet implemented");
}