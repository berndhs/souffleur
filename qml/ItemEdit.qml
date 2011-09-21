import QtQuick 1.0

import moui.geuzen.utils.static 1.0

Rectangle {
  id: mainBox
  width: 1024
  height: 512
  radius: 8
 // clip: true
  property real frameWidth: 2
  property real nickWidth: width - 2*frameWidth
  property real timeWidth: width - 2*frameWidth
  property real descriptionWidth: width - 2*frameWidth
  property real rowHeight: 64
  property real restHeight: height - 2.0 * rowHeight
  property real repeatMinutes: 0
  property bool isPhone: false
  property string itemUuid: ""
  
  color: "#f0f0ff"
  
  signal saveEvent (string theUuid,
                    string theTitle, string theTime, string theDescription, 
                    string command,
                    bool  audible,
                    real repeatMinutes)
  signal cancelled ()
  
  function cancel () {
    visible = false
    cancelled ()
  }
  
  function save (theTitle, theTime, theDescription, hasCommand, command, audible) {
    console.log ("save new event " + theTitle + "/" + theTime + "/" + theDescription
                 + "/" + hasCommand + "/" + command)
    visible = false
    if (!hasCommand) {
      command = ""
    }
    if (!repeatCheck.isChecked) {
      repeatMinutes = 0
    }
    saveEvent (itemUuid, theTitle, theTime, theDescription, command, audible, repeatMinutes)
    console.log ("back from signal")
  }
  
  function startNew () {
    console.log (" start new event ")
    
    itemUuid = ""
    itemNickText.text = qsTr ("new event")
    itemTimeText.text = Qt.formatDateTime (new Date(),"yyyy-MM-dd hh:mm:ss")
    itemDescriptionText.text = qsTr ("event description")
    commandCheck.isChecked = false
    itemCommandText.text = ""
    audioCheck.isChecked = false
    repeatEdit.setValues (0,0,0)
    repeatEdit.accepted = false
    visible = true
    itemNickText.forceActiveFocus ()
    console.log ("New Edit " + itemNickText.text
                 + "\n     desc " + itemDescriptionText.text
                 + "\n     time " + itemTimeText.text
                 + "\n     run  " + commandCheck.isChecked
                 + "\n     audi " + audioCheck.isChecked)
  }
  function startModify (theUuid, theNick, theTime, theDescription, 
                        theCommand, theAudible, theRepeat) {
    console.log (" start mod old event " + theUuid + " delay " + theRepeat)
    itemUuid = theUuid
    itemNickText.text = theNick
    itemTimeText.text = theTime
    itemDescriptionText.text = theDescription
    itemCommandText.text = theCommand
    commandCheck.isChecked = theCommand.length > 0
    audioCheck.isChecked = theAudible
    repeatCheck.isChecked = theRepeat > 0
    if (theRepeat > 0) {
      setRepeatTimes (theRepeat)
    } else {
      repeatEdit.setValues (0,0,0)
    }
    repeatEdit.accepted = false
    visible = true
    console.log ("Old Edit " + itemNickText.text
                 + "\n     desc " + itemDescriptionText.text
                 + "\n     time " + itemTimeText.text
                 + "\n     run  " + commandCheck.isChecked
                 + "\n     audi " + audioCheck.isChecked)
  }
  
  function setRepeatTimes (minutes) {
    var hours = Math.floor ( minutes / 60)
    minutes -= hours * 60
    var days = Math.floor ( hours / 24)
    hours -= days * 24
    repeatEdit.setValues (days,hours,minutes)
  }
  
  DateTimeChecker {
    id: dateChecker
  }
  
  Flickable {
    id: itemList
    height: mainBox.height
    width: mainBox.width
    contentHeight: itemPartsContent.height
    contentWidth: itemPartsContent.width
    interactive: true
    clip: true
    flickableDirection: Flickable.HorizontalAndVerticalFlick
    boundsBehavior: Flickable.DragOverBounds
    
    anchors {
      top: mainBox.top; topMargin: mainBox.frameWidth
      left: mainBox.left; leftMargin: mainBox.frameWidth
    }
    
    Rectangle {
      id:itemPartsContent
      
      width: childrenRect.width
      height: childrenRect.height 
      color: "transparent"
      
      
      Column {
        id:itemPartsColumn
        spacing: 2
        
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
            selectByMouse: true
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            text: "title"
            Keys.onTabPressed: { itemTimeText.forceActiveFocus() }
            onAccepted: { itemTimeText.forceActiveFocus() }
          }
          CopyPasteMenu {
            id: itemNickCopyPaste
            anchors { top: itemNick.top; horizontalCenter: itemNick.horizontalCenter }
            visible: false
            opacity: 0.7
            z: itemNick.z + 1
            onWantCopy: {
              itemNickText.copy ()
              visible = false
            }
            onWantPaste: {
              itemNickText.paste ()
              visible = false
            }
            onWantSelect: {
              itemNickText.selectAll ()
              visible = false
            }
          }
          
          MouseArea {
            anchors.fill:parent
            onClicked: {
              itemNickText.forceActiveFocus()
            }
            onPressAndHold: {
              itemNickCopyPaste.visible = !itemNickCopyPaste.visible
            }
          }
        }
                
        Rectangle {
          id: timeFlowBox
          width: timeRow.width
          height: timeRow.height
          color: "transparent"
          radius:mainBox.radius - 1
          CopyPasteMenu {
            id: timeFlowCopyPaste
            anchors { top: timeFlowBox.top; horizontalCenter: timeFlowBox.horizontalCenter }
            visible: false
            opacity: 0.7
            z: timeFlowBox.z + 1
            onWantCopy: {
              itemTimeText.copy ()
              visible = false
            }
            onWantPaste: {
              itemTimeText.paste ()
              visible = false
            }
            onWantSelect: {
              itemTimeText.selectAll ()
              visible = false
            }
          }
          Flow {
            id: timeRow
            spacing: 4
            width: mainBox.width
            Rectangle {
              id: itemTime
              width: 400
              height: mainBox.rowHeight
              color: itemTimeText.focus ? "white" : Qt.darker (mainBox.color, 1.3)
              radius:mainBox.radius - 1
              property bool isValid: true
              function checkDate () {
                itemTime.isValid = true// dateChecker.isValid (itemTimeText.text)
                return itemTime.isValid
              }
              TextInput {
                id: itemTimeText
                activeFocusOnPress: true
                selectByMouse: true
                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                text: Qt.formatDateTime (new Date(),"yyyy-MM-dd hh:mm:ss")
                color: itemTime.isValid ? "black" : "red"
                font.italic: !itemTime.isValid
                onAccepted: {
                  if (itemTime.checkDate ()) {
                    itemDescriptionText.forceActiveFocus()
                  }
                }
                Keys.onTabPressed: {
                  if (itemTime.checkDate ()) {
                    itemDescriptionText.forceActiveFocus()
                  }
                }
              }
              MouseArea {
                anchors.fill:parent
                onPressed: {
                  itemTimeText.forceActiveFocus()
                }
                onPressAndHold: {
                  timeFlowCopyPaste.visible = !timeFlowCopyPaste.visible
                }
              }
            }
            
            ChoiceButton {
              id: saveButton
              labelText: qsTr ("Save")
              height: mainBox.rowHeight * 0.8
              width: labelWidth
              radius: height * 0.5
              topColor: "#aaff99"
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
            ChoiceButton {
              id:cancelButton
              labelText: qsTr ("Cancel")
              height: mainBox.rowHeight * 0.8
              width: labelWidth
              radius: height * 0.5
              topColor: "#aa99ff"
              onPressed: {
                mainBox.cancel ()
              }
            }
          }
        }
        
        Flow {
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
          CheckItem {
            id: repeatCheck
            height: mainBox.rowHeight
            imageHeight: mainBox.rowHeight
            text: qsTr ("Repeat")
            onUserChanged: {
              if (checked) {
                console.log (" repeat changed to " + isChecked)
              }
            }
          }
        }
        Rectangle {
          id: itemRepeat
          width: mainBox.width
          height: repeatCheck.isChecked ? repeatEdit.height : 0
          visible: repeatCheck.isChecked
          color: "white"
          RepeatEdit {
            id: repeatEdit
            isPhone: mainBox.isPhone
            onAcceptedValue: {
              mainBox.repeatMinutes = value
            }
          }
        }
        
        Rectangle {
          id: itemCommand
          width: mainBox.width
          height: commandCheck.isChecked ? mainBox.rowHeight : 0
          visible: commandCheck.isChecked
          color: itemCommandText.focus ? "white" : Qt.darker (mainBox.color, 1.3)
          TextInput {
            id: itemCommandText
            activeFocusOnPress: true
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            text: "command"
          }
          CopyPasteMenu {
            id: itemCommandCopyPaste
            anchors { top: itemCommand.top; horizontalCenter: itemCommand.horizontalCenter }
            visible: false
            opacity: 0.7
            z: itemCommandText.z + 1
            onWantCopy: {
              itemCommandText.copy ()
              visible = false
            }
            onWantPaste: {
              itemCommandText.paste ()
              visible = false
            }
            onWantSelect: {
              itemCommandText.selectAll ()
              visible = false
            }
          }
          MouseArea {
            anchors.fill:parent
            onClicked: {
              itemCommandText.forceActiveFocus()
            }
            onPressAndHold: {
              console.log (" command hold ")
              itemCommandCopyPaste.visible = !itemCommandCopyPaste.visible
            }
          }
        }
        
        Rectangle {
          id: itemDescription
          width: mainBox.descriptionWidth
          height: Math.max (itemDescriptionText.height, mainBox.rowHeight)
          color: itemDescriptionText.focus ? "white" : Qt.darker (mainBox.color, 1.4)
          radius:mainBox.radius - 1
          TextEdit {
            id: itemDescriptionText
            activeFocusOnPress: true
            selectByMouse: true
            width: parent.width
            wrapMode: Text.WordWrap
            function activate () {
              forceActiveFocus ()
              cursorPosition = text.length
            } 
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            text: "Description"
          }
          CopyPasteMenu {
            id: itemDescriptionCopyPaste
            anchors { top: itemDescription.top; horizontalCenter: itemDescription.horizontalCenter }
            visible: false
            opacity: 0.7
            z: itemDescriptionText.z + 1
            onWantCopy: {
              itemDescriptionText.copy ()
              visible = false
            }
            onWantPaste: {
              itemDescriptionText.paste ()
              visible = false
            }
            onWantSelect: {
              itemDescriptionText.selectAll ()
              visible = false
            }
          }
          MouseArea {
            anchors.fill:parent
            onClicked: {
              itemDescriptionText.activate()
            }
            onPressAndHold: {
              console.log (" command hold ")
              itemDescriptionCopyPaste.visible = !itemDescriptionCopyPaste.visible
            }
          }
        }
        
        
        Rectangle {
          id: tailSpacer
          height: mainBox.height - mainBox.rowHeight
          width: mainBox.width
          color: "transparent"
        }
        
        Rectangle {
          id: tailMarker
          height: 4
          width: mainBox.width
          color: "red"
        }
      }
      
    } 
  }
} 
