// Othello.cpp : Defines the entry point for the application.
//
#include <memory>
import controller;
import file_repository;
import db_repository;
import tui;

int main() {
	OthelloBoard board;
	TUIView view;
	auto repository = std::make_unique<DBRepository>();
	Controller controller(board, view, std::move(repository));
	controller.startGame();
	return 0;
}