import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 600
    height: 700
    title: "Othello"
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20
        
        // Top buttons row
        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            
            Button {
                text: "Save"
                Layout.preferredWidth: 80
                onClicked: guiView.saveGame()
            }
            
            Button {
                text: "Load"
                Layout.preferredWidth: 80
                onClicked: guiView.loadGame()
            }
            
            Button {
                text: "Quit"
                Layout.preferredWidth: 80
                onClicked: guiView.quitGame()
            }
        }
        
        // Message display area
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#f0f0f0"
            border.color: "#cccccc"
            border.width: 1
            radius: 5
            
            Text {
                id: messageText
                anchors.centerIn: parent
                text: guiView.currentMessage
                font.pointSize: 12
                wrapMode: Text.WordWrap
            }
        }
        
        // Game board grid
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 400
            color: "#228B22"
            border.color: "#000000"
            border.width: 2
            
            Grid {
                id: gameGrid
                anchors.centerIn: parent
                rows: 8
                columns: 8
                spacing: 1
                
                Repeater {
                    model: 64
                    
                    Rectangle {
                        width: 45
                        height: 45
                        color: "#228B22"
                        border.color: "#000000"
                        border.width: 1
                        
                        property int cellIndex: index
                        property bool hasBlackPiece: guiView.hasBlackPiece(index)
                        property bool hasWhitePiece: guiView.hasWhitePiece(index)
                        property bool isValidMove: guiView.isValidMove(index)
                        
                        // Game piece or valid move indicator
                        Rectangle {
                            anchors.centerIn: parent
                            width: hasBlackPiece || hasWhitePiece ? 35 : (isValidMove ? 10 : 0)
                            height: width
                            radius: width / 2
                            color: hasBlackPiece ? "#000000" : 
                                   hasWhitePiece ? "#FFFFFF" : 
                                   isValidMove ? "#FFD700" : "transparent"
                            border.color: hasWhitePiece ? "#000000" : "transparent"
                            border.width: hasWhitePiece ? 1 : 0
                            visible: hasBlackPiece || hasWhitePiece || isValidMove
                        }
                        
                        // Click area for moves
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (parent.isValidMove) {
                                    guiView.makeMove(cellIndex)
                                }
                            }
                            cursorShape: parent.isValidMove ? Qt.PointingHandCursor : Qt.ArrowCursor
                        }
                        
                        // Column and row labels
                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.margins: 2
                            text: {
                                if (cellIndex < 8) {
                                    return String.fromCharCode(65 + (cellIndex % 8)) // A-H
                                }
                                if (cellIndex % 8 === 0) {
                                    return Math.floor(cellIndex / 8) + 1 // 1-8
                                }
                                return ""
                            }
                            font.pointSize: 8
                            color: "#000000"
                            visible: cellIndex < 8 || cellIndex % 8 === 0
                        }
                    }
                }
            }
        }
        
        // Current player indicator
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#e0e0e0"
            border.color: "#cccccc"
            border.width: 1
            radius: 5
            
            Text {
                anchors.centerIn: parent
                text: "Current Player: " + (guiView.currentPlayer === 0 ? "Black" : "White")
                font.pointSize: 14
                font.bold: true
            }
        }
        
        // Score display
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            color: "#d0d0d0"
            border.color: "#cccccc"
            border.width: 1
            radius: 5
            
            RowLayout {
                anchors.centerIn: parent
                spacing: 30
                
                Text {
                    text: "Black: " + guiView.blackScore
                    font.pointSize: 16
                    font.bold: true
                }
                
                Text {
                    text: "White: " + guiView.whiteScore
                    font.pointSize: 16
                    font.bold: true
                }
            }
        }
    }
}