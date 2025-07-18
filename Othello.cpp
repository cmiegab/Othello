﻿// Othello.cpp : Defines the entry point for the application.
//
#include <memory>
#include <QCoreApplication>
import controller;
import file_repository;
import db_repository;
import tui;

int main(int argc, char* argv[]) {
	QCoreApplication app(argc, argv);
	OthelloBoard board;
	TUIView view;
	auto repository = std::make_unique<DBRepository>();
	Controller controller(board, view, std::move(repository));
	controller.initializeGame();
	return 0;
}