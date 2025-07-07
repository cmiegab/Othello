module;
#include <stdexcept>
#include <sqlite3.h>
module db_repository;

namespace {
    constexpr const char* DB_NAME = "game_state.db";
    constexpr const char* CREATE_TABLE_SQL = R"(
        CREATE TABLE IF NOT EXISTS game_state (
            id INTEGER PRIMARY KEY,
            current_player INTEGER NOT NULL,
            black_pieces INTEGER NOT NULL,
            white_pieces INTEGER NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
}

void DBRepository::saveGame(const GameState& state) {
    sqlite3* db;
    int rc = sqlite3_open(DB_NAME, &db);

    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    }

    // Create table if it doesn't exist
    rc = sqlite3_exec(db, CREATE_TABLE_SQL, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to create table: " + std::string(sqlite3_errmsg(db)));
    }

    // Clear existing data (keeping only the latest state)
    const char* clearSql = "DELETE FROM game_state;";
    rc = sqlite3_exec(db, clearSql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to clear existing data: " + std::string(sqlite3_errmsg(db)));
    }

    // Insert new game state
    const char* insertSql = R"(
        INSERT INTO game_state (current_player, black_pieces, white_pieces) 
        VALUES (?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, insertSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, static_cast<int>(state.currentPlayer));
    sqlite3_bind_int64(stmt, 2, static_cast<sqlite3_int64>(state.blackPieces.to_ullong()));
    sqlite3_bind_int64(stmt, 3, static_cast<sqlite3_int64>(state.whitePieces.to_ullong()));

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw std::runtime_error("Failed to insert game state: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

GameState DBRepository::loadGame() {
    sqlite3* db;
    int rc = sqlite3_open(DB_NAME, &db);

    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    }

    // Query for the latest game state
    const char* selectSql = R"(
        SELECT current_player, black_pieces, white_pieces 
        FROM game_state 
        ORDER BY created_at DESC 
        LIMIT 1;
    )";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        GameState state;

        // Read current player
        int playerInt = sqlite3_column_int(stmt, 0);
        state.currentPlayer = static_cast<Player>(playerInt);

        // Read black pieces bitboard

        sqlite3_int64 blackPiecesInt = sqlite3_column_int64(stmt, 1);
        state.blackPieces = BitBoard(static_cast<uint64_t>(blackPiecesInt));

        // Read white pieces bitboard
        sqlite3_int64 whitePiecesInt = sqlite3_column_int64(stmt, 2);
        state.whitePieces = BitBoard(static_cast<uint64_t>(whitePiecesInt));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return state;
    }
    else if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw std::runtime_error("No saved game state found in database");
    }
    else {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw std::runtime_error("Failed to query game state: " + std::string(sqlite3_errmsg(db)));
    }
}