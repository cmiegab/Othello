module;
#include <sqlite3.h>
#include <string>
#include <memory>
export module db_repository;
export import repository;

export class DBRepository : public IRepository {
public:
	void saveGame(const GameState& state) override;
	GameState loadGame() override;
};