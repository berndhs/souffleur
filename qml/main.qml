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
  property bool isPhone: false
  property real rowHeight: 42
  property real nickWidth: mainWidth * 0.25 
  property color mainColor: "#f0f0c0"
  x: isPortrait ? (isInverted ? (width - height) * 0.5 : (height - width) * 0.5) : 0
  property real mainWidth: isPortrait ? height : width
  property real mainHeight: isPortrait ? width : height
  property real orientMargin: (isPortrait ? Math.abs (width - height) * 0.5 : 0)


  signal quit ()
  signal saveNewEvent (string uuid, string title, string time, string description, 
                       string command, bool audible, real repeatMins)
  signal deleteEvent (string uuid)
  
  function setAboutText (theText) {
    aboutBox.text = theText
  }
  
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
  
  Rectangle {
    id: brandingBox
    width: buttonRow.width
    height: buttonRow.height
    color: "#eeee99"
    radius: height * 0.5
    anchors { top: mainBox.top; horizontalCenter: mainBox.horizontalCenter }
    Row {
      id:buttonRow
      spacing:32
      anchors { top: brandingBox.top; horizontalCenter: brandingBox.horizontalCenter }
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
      Rectangle {
        id: aboutItem
        width: Math.max (nameText.width, subheadingText.width)
        height: aboutColumn.height
        color: "transparent"
        Column {
          id: aboutColumn
          Text {
            id: nameText
            font.weight: Font.Bold
            text: "Souffleur"
          }
          Text {
            id: subheadingText
            text: qsTr ("Scheduling")
          }

        }
        MouseArea {
          anchors.fill: aboutItem
          onPressAndHold: {
            aboutBox.visible = !aboutBox.visible
          }
        }
      }
      
      ChoiceButton {
        id:addButton
        labelText: qsTr("+")
        font.weight: Font.Bold
        topColor: "#99ee99"
        height:mainBox.rowHeight
        radius: height * 0.5
        onPressed: {
          console.log ("add button pressed")
          newItemEdit.startNew ()
        }
      }
    }
  }

  About {
    id: aboutBox
    width: mainBox.mainWidth * 0.7
    height: mainBox.mainHeight - brandingBox.height
    visible: false
    radius: 12
    z: eventList.z + 1
    color: "#eeffee"
    border.color: "#ddeedd"; border.width: 1
    opacity: 0.9
    anchors {
      top: brandingBox.bottom; topMargin: brandingBox.height
      horizontalCenter: brandingBox.horizontalCenter
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
        Rectangle {
          id: repeatIconBox
          width: 0.5* mainBox.rowHeight
          height: mainBox.rowHeight
          color: "transparent"
          Image {
            id: repeatImage
            width: 0.5 * mainBox.rowHeight
            height: 0.5 * mainBox.rowHeight
            anchors.top: parent.top
            source: eventRepeating ? ":/icons/R.png" : ""
          }
        }
        Text {
          id: eventTimeText
          text: eventWhen
          height:mainBox.rowHeight
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
          
          MouseArea {
            anchors.fill: parent
            onClicked: {
              itemModify.visible = !itemModify.visible
              console.log (" changed modify visible " + itemModify.visible)
            }
          }
          
        }
        Text {
          id: eventWhatText
          text: eventWhat 
          width: Math.max (mainBox.nickWidth, normalDelegateRow.restWidth)
          wrapMode:Text.Wrap
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
        
      }
      ItemModify {
        id: itemModify
        anchors {
          top: eventListDelegate.bottom;
          horizontalCenter: eventListDelegate.horizontalCenter 
        }
        visible: false
        z: eventListDelegate.z + 1
        onVisibleChanged: {
          console.log (" itemModify visible now " + visible + " for index " + index)
        }
        onWantChange: {
          newItemEdit. startModify (eventUuid, eventTitle, eventWhen, eventWhat, 
                                 eventCommand, eventAudible, eventRepeatMinutes) 
        }
        onWantDelete: {
          mainBox.deleteEvent (eventUuid)
        }
      }
    }
  }
  
  ListView {
    id:eventList
    model:cppEventListModel
    delegate: eventListDelegate
    width: mainBox.mainWidth
    height: mainBox.mainHeight - brandingBox.height
    currentIndex: -1
    clip: true
    anchors {
      top:brandingBox.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
  }
  ItemEdit {
    id: newItemEdit
    visible: false
    width: mainBox.mainWidth * 0.9
    height: mainBox.mainHeight - brandingBox.height
    isPhone: mainBox.isPhone
    anchors {
      top: brandingBox.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
    z: eventList.z+1
    onSaveEvent: {
      mainBox.saveNewEvent (theUuid, theTitle, theTime, theDescription, 
                            command, audible, repeatMinutes)
    }
  }
  Component.onCompleted: {
    orientationWatcher.start()
  }
}
