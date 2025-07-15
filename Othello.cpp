// Othello.cpp : Defines the entry point for the application.
//
#include <memory>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
import controller;
import file_repository;
import db_repository;
import tui;
import gui;

int main(int argc, char* argv[]) {
	QGuiApplication app(argc, argv);
	OthelloBoard board;

	GUIView view;
	//TUIView view;
	auto repository = std::make_unique<DBRepository>();
	Controller controller(board, view, std::move(repository));

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("guiView", &view);
	engine.load(QUrl(QStringLiteral("qrc:/include/view/gui.qml")));
	controller.initializeGame();
	return app.exec();
}