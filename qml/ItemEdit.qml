import QtQuick 1.0

import moui.geuzen.utils.static 1.0

Rectangle {
  id: mainBox
  width: 1024
  height: 1024
  radius: 8
  clip: true
  property real frameWidth: 2
  property real nickWidth: width - 2*frameWidth
  property real timeWidth: width - 2*frameWidth
  property real descriptionWidth: width - 2*frameWidth
  property real rowHeight: 64
  property real restHeight: height - 2.0 * rowHeight
  
  color: "#f0f0ff"
  border.color: "#303050"
  border.width: frameWidth
  
  signal saveEvent (string theTitle, string theTime, string theDescription, 
                    string command,
                    bool  audible)
  
  function save (theTitle, theTime, theDescription, hasCommand, command, audible) {
    console.log ("save new event " + theTitle + "/" + theTime + "/" + theDescription
                 + "/" + hasCommand + "/" + command)
    visible = false
    if (!hasCommand) {
      command = ""
    }
    saveEvent (theTitle, theTime, theDescription, command, audible)
    console.log ("back from signal")
  }
  
  DateTimeChecker {
    id: dateChecker
  }
  
  VisualItemModel {
    id:itemPartsModel
    
    Rectangle {
      id: itemNick
      width: mainBox.nickWidth
      height: mainBox.rowHeight
      radius:mainBox.radius - 1
      color: itemNickText.focus ? "white" : Qt.darker (mainBox.color, 1.2)
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
      width: mainBox.timeWidth
      height: mainBox.rowHeight
      color: "transparent"
      radius:mainBox.radius - 1
      Row {
        id: timeRow
        spacing: 4
        height: mainBox.rowHeight

        Rectangle {
          id: itemTime
          width: mainBox.timeWidth - saveButton.width - timeRow.spacing - commandCheck.width
          height: mainBox.rowHeight
          color: itemTimeText.focus ? "white" : Qt.darker (mainBox.color, 1.3)
          radius:mainBox.radius - 1
          property bool isValid: true
          function checkDate () {
            itemTime.isValid = dateChecker.isValid (itemTimeText.text)
            return itemTime.isValid
          }
          TextInput {
            id: itemTimeText
            activeFocusOnPress: true
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            text: Qt.formatDateTime (new Date(),"yyyy-MM-dd hh:mm:ss")
            color: itemTime.isValid ? "black" : "red"
            font.italic: !itemTime.isValid
            onAccepted: {
              itemTime.checkDate ()
            }
            Keys.onTabPressed: {
              itemTime.checkDate ()
            }
          }
          MouseArea {
            anchors.fill:parent
            onPressed: {
              itemTimeText.forceActiveFocus()
            }
          }
        }
                
        ChoiceButton {
          id: saveButton
          labelText: qsTr ("Save")
          height: mainBox.rowHeight * 0.8
          radius: height * 0.5
          topColor: "#aaff99"
          anchors { verticalCenter: itemTime.verticalCenter }
          onPressed: {
            if (itemTime.checkDate()) {
              mainBox.save (itemNickText.text, 
                            itemTimeText.text, 
                            itemDescriptionText.text,
                            commandCheck.isChecked,
                            itemCommandText.text,
                            audioCheck.isChecked)
            }
          }
        }
      }
    }
    Row {
      CheckItem {
        id: commandCheck
        height: mainBox.rowHeight
        imageHeight: mainBox.rowHeight
        text: qsTr ("Run Command")
        onUserChanged: {
          if (checked) {
            itemCommandText.forceActiveFocus ()
          }
          console.log (" check command changed to " + isChecked)
          console.log (" command row height " + itemCommand.height)
        }
      }
      CheckItem {
        id: audioCheck
        height: mainBox.rowHeight
        imageHeight: mainBox.rowHeight
        text: qsTr ("Audible")
        onUserChanged: {
          console.log (" audible changed to " + isChecked)
        }
      }
    }
    Rectangle {
      id: itemCommand
      width: mainBox.width
      height: commandCheck.isChecked ? mainBox.rowHeight : 0
      visible: commandCheck.isChecked
      color: itemCommandText.focus ? "white" : Qt.darker (mainBox.color, 1.3)
      border.color: "green"; border.width: 2
      TextInput {
        id: itemCommandText
        activeFocusOnPress: true
        anchors { left: parent.left; verticalCenter: parent.verticalCenter }
        text: "command"
      }
      MouseArea {
        anchors.fill:parent
        onPressed: {
          itemCommandText.forceActiveFocus()
        }
      }
    }

    Rectangle {
      id: itemDescription
      width: mainBox.descriptionWidth
      height: itemDescriptionText.height
      color: itemDescriptionText.focus ? "white" : Qt.darker (mainBox.color, 1.4)
      radius:mainBox.radius - 1
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
    width: mainBox.mainWidth
    anchors {
      top: mainBox.top; topMargin: mainBox.frameWidth
      left: mainBox.left; leftMargin: mainBox.frameWidth
    }
  }
} 
