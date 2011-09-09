import QtQuick 1.0

import moui.geuzen.utils.static 1.0

Rectangle {
  id: mainBox
  width: 1024
  height: 1024
  property real nickWidth: width
  property real timeWidth: width
  property real descriptionWidth: width
  property real rowHeight: 64
  property real restHeight: height - 2.0 * rowHeight
  color: "blue"
  
  DateTimeChecker {
    id: dateChecker
  }
  
  VisualItemModel {
    id:itemPartsModel
    Rectangle {
      id: itemNick
      width: mainBox.nickWidth
      height: mainBox.rowHeight
      color: itemNickText.focus ? "white" : Qt.lighter (mainBox.color, 1.2)
      TextInput {
        id: itemNickText
        activeFocusOnPress: true
        focus: true
        anchors { left: parent.left; verticalCenter: parent.verticalCenter }
        text: "title"
      }
      MouseArea {
        anchors.fill:parent
        onPressed: {
          itemNickText.forceActiveFocus()
        }
      }
    }
    Rectangle {
      id: itemTime
      width: mainBox.timeWidth
      height: mainBox.rowHeight
      color: itemTimeText.focus ? "white" : Qt.lighter (mainBox.color, 1.3)
      property bool isValid: true
      TextInput {
        id: itemTimeText
        activeFocusOnPress: true
        anchors { left: parent.left; verticalCenter: parent.verticalCenter }
        text: Qt.formatDateTime (new Date(),"yyyy-MM-dd hh:mm:ss")
        color: itemTime.isValid ? "black" : "red"
        font.italic: !itemTime.isValid
        onAccepted: {
          itemTime.isValid = dateChecker.isValid (text)
        }
        Keys.onTabPressed: {
          itemTime.isValid = dateChecker.isValid (text)
        }
      }
      MouseArea {
        anchors.fill:parent
        onPressed: {
          itemTimeText.forceActiveFocus()
        }
      }
    }
    Rectangle {
      id: itemDescription
      width: mainBox.descriptionWidth
      height: itemDescriptionText.height
      color: itemDescriptionText.focus ? "white" : Qt.lighter (mainBox.color, 1.4)
      TextEdit {
        id: itemDescriptionText
        activeFocusOnPress: true
        width: parent.width
        wrapMode: Text.WordWrap
        anchors { left: parent.left; verticalCenter: parent.verticalCenter }
        text: "description"
      }
    }
  }
  ListView {
    id: itemList
    model: itemPartsModel
    height: parent.height
    width: parent.width
    anchors {
      top: mainBox.top
      horizontalCenter: mainBox.horizontalCenter
    }
  }
} 
