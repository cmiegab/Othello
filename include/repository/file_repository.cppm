export module file_repository;
export import repository;

export class FileRepository : public IRepository {
public:
	void saveGame(const GameState& state) override;
	GameState loadGame() override;
};