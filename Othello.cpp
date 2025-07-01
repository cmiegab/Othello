// Othello.cpp : Defines the entry point for the application.
//
import tui;
import board;

int main() {
	OthelloBoard board;
	Player player = Player::BLACK;
	printBoard(board, player);
	board.setMoves(player, 19); // Example move at index 27
	player = Player::WHITE;
	printBoard(board, player);
	board.setMoves(player, 20); // Example move at index 19
	player = Player::BLACK;
	printBoard(board, player);
	board.setMoves(player, 21); // Example move at index 20
	return 0;
}