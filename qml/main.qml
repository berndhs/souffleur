import QtQuick 1.0
import moui.geuzen.utils.static 1.0

Rectangle {
  width: 4096
  height: 4096
  id: mainBox
  color: "#f0f0cc"
  visible: true
  rotation: 0
  property bool isPortrait: false
  property bool isInverted: false
  property real rowHeight: 42
  property real nickWidth: mainWidth * 0.25  
  x: isPortrait ? (isInverted ? (width - height) * 0.5 : (height - width) * 0.5) : 0
  property real mainWidth: isPortrait ? height : width
  property real mainHeight: isPortrait ? width : height
  property real orientMargin: (isPortrait ? Math.abs (width - height) * 0.5 : 0)


  signal quit ()
  signal saveNewEvent (string title, string time, string description, string command, bool audible)
  
  GeuzenOrientation {
    id: orientationWatcher
    onRotationChange: {
      mainBox.isPortrait = portrait
      mainBox.rotation = rotation
      mainBox.isInverted = inverted
      console.log ("orientation port " + mainBox.isPortrait)
      console.log ("main box x " + mainBox.x + " y " + mainBox.y)
      console.log ("rect size: " + mainBox.width + ", " + mainBox.height)
      console.log ("my size  : " + mainWidth + ", " + mainHeight)
    }
  }
  
  Row {
    id:buttonRow
    spacing:32
    anchors { top: mainBox.top; horizontalCenter: mainBox.horizontalCenter }
    ChoiceButton {
      id:quitButton
      labelText: qsTr ("Quit")
      topColor: "#00ccff"
      height:mainBox.rowHeight
      radius: height * 0.5
      onPressed: {
        console.log ("quit pressed")
        mainBox.quit() 
      }
    }
    ChoiceButton {
      id:addButton
      labelText: qsTr("+")
      topColor: "#e7e755"
      height:mainBox.rowHeight
      radius: height * 0.5
      onPressed: {
        console.log ("add button pressed")
        newItemEdit.visible = !newItemEdit.visible
      }
    }
  }
  
  Component {
    id:eventListDelegate
    Rectangle {
      id:normalDelegateRow
      height:mainBox.rowHeight
      width:mainBox.mainWidth
      color:Qt.lighter (mainBox.color,isCurrent ? 1.5 : 1.2)
      property bool isCurrent: index == eventList.currentIndex
      Column {
        
        Row {
          id: eventListRow
          Text {
            id: eventTitleText
            text: eventTitle 
            width: mainBox.nickWidth
            height:mainBox.rowHeight
            font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
          }
          Text {
            id: eventTimeText
            text: eventWhen
            height:mainBox.rowHeight
            font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
          }
          Text {
            id: eventWhatText
            text: eventWhat 
            height:mainBox.rowHeight
            width:normalDelegateRow.width - eventTitleText.width - eventTimeText.width
            font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
          }
        }
        MouseArea {
          anchors.fill:eventListRow
          onPressed: {
            eventList.currentIndex = index
            console.log ("clicked to make index " + eventList.currentIndex)
          }
        }
      }
    }
  }

  ListView {
    id:eventList
    model:cppEventListModel
    delegate: eventListDelegate
    width: mainBox.mainWidth
    height: mainBox.mainHeight - buttonRow.height
    currentIndex: -1
    clip: true
    anchors {
      top:buttonRow.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
  }
  ItemEdit {
    id: newItemEdit
    visible: false
    width: mainBox.mainWidth * 0.9
    height: mainBox.mainHeight - buttonRow.height
    anchors {
      top: buttonRow.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
    z: eventList.z+1
    onSaveEvent: {
      mainBox.saveNewEvent (theTitle, theTime, theDescription, command, audible)
    }
  }
  Component.onCompleted: {
    orientationWatcher.start()
  }
}
