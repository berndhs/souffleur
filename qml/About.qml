import QtQuick 1.0

Rectangle {
  id: aboutBox
  objectName: "AboutBox"
  width: 1024
  height: 1024
  color: "#f0f0f0"

  property alias text: aboutText.text
  property alias font: aboutText.font
  property alias wrapMode: aboutText.wrapMode

  Text {
    id: aboutText
    text: "About This Program\nAuthors"
    wrapMode: Text.Wrap
    anchors { centerIn: parent }
  }
  MouseArea {
    anchors.fill: parent
    onPressAndHold: {
      aboutBox.visible = false
    }
  }
}