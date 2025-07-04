export module repository;
export import board;

export struct GameState {
	Player currentPlayer;
	BitBoard blackPieces;
	BitBoard whitePieces;
};

export class IRepository {
public:
	virtual ~IRepository() = default;
	// Save the current game state
	virtual void saveGame(const GameState& state) = 0;
	// Load latest game state
	virtual GameState loadGame() = 0;
};
