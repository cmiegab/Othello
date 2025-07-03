// Othello.cpp : Defines the entry point for the application.
//
import controller;

int main() {
	OthelloBoard board;
	TUIView view;
	Controller controller(board, view);
	controller.startGame();
	return 0;
}