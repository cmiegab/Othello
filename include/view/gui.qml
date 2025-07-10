import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
	id: mainWindow
	visible: true
	width: 800
	height: 600
	title: "Othello"
	Column {
		anchors.centerIn: parent
		spacing: 10
		Button {
			text: "Show Help"
			onClicked: guiView.showHelp()
		}
		Button {
			text: "Save Game"
			onClicked: guiView.saveGame()
		}
		Button {
			text: "Load Game"
			onClicked: guiView.loadGame()
		}
	}
}