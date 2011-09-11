import QtQuick 1.0
import moui.geuzen.utils.static 1.0

Rectangle {
  width: 1024
  height: 1024
  id: mainBox
  color: "transparent"
  visible: true
  rotation: 0
  property bool isPortrait: false
  property bool isInverted: false
  property real rowHeight: 42
  property real nickWidth: mainWidth * 0.25 
  property color mainColor: "#f0f0c0"
  x: isPortrait ? (isInverted ? (width - height) * 0.5 : (height - width) * 0.5) : 0
  property real mainWidth: isPortrait ? height : width
  property real mainHeight: isPortrait ? width : height
  property real orientMargin: (isPortrait ? Math.abs (width - height) * 0.5 : 0)


  signal quit ()
  signal saveNewEvent (string title, string time, string description, 
                       string command, bool audible, real repeatMins)
  
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
      font.weight: Font.Bold
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
      height:eventListRow.height
      width:mainBox.mainWidth
      color:Qt.lighter (mainBox.mainColor,isCurrent ? 1.5 : 1.2)
      property bool isCurrent: index == eventList.currentIndex
      property real restWidth: width - eventTimeText.width 
                               - audibleIconBox.width - eventTitleText.width 
                               - (eventListRow.itemCount - 1) * eventListRow.spacing
      
      
      Flow {
        id: eventListRow
        spacing: 4
        //height: Math.max (mainBox.rowHeight, eventWhatText.height)
        width: normalDelegateRow.width
        property int itemCount: 4
        Text {
          id: eventTitleText
          text: eventTitle 
          width: mainBox.nickWidth
          height:mainBox.rowHeight
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
        Rectangle {
          id: audibleIconBox
          width: 0.5* mainBox.rowHeight
          height: mainBox.rowHeight
          color: "transparent"
          Image {
            id: audibleImage
            width: 0.5 * mainBox.rowHeight
            height: 0.5 * mainBox.rowHeight
            anchors.top: parent.top
            source: eventAudible ? ":/icons/bell.png" : ""
          }
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
          width: Math.max (mainBox.nickWidth, normalDelegateRow.restWidth)
          wrapMode:Text.WrapAtWordBoundaryOrAnywhere
          //height:mainBox.rowHeight
          //width:normalDelegateRow.width - eventTitleText.width - eventTimeText.width
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
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
    isPhone: mainBox.isPhone
    anchors {
      top: buttonRow.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
    z: eventList.z+1
    onSaveEvent: {
      mainBox.saveNewEvent (theTitle, theTime, theDescription, 
                            command, audible, repeatMinutes)
    }
  }
  Component.onCompleted: {
    orientationWatcher.start()
  }
}
