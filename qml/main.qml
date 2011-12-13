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
  
  function selectHelp () {
    console.log ("user wants help")
  }
  
  function doAlert () {
    alertList.visible = true
    alertList.startTimer ()
  }
  
  GeuzenOrientation {
    id: orientationWatcher
    onRotationChange: {
      mainBox.isPortrait = portrait
      mainBox.rotation = rotation
      mainBox.isInverted = inverted
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
      spacing:20
      anchors { top: brandingBox.top; horizontalCenter: brandingBox.horizontalCenter }
      ChoiceButton {   
        id: helpButton
        width: mainBox.rowHeight
        height: mainBox.rowHeight
        topColor: "#00ccff"
        radius: height * 0.5
        labelText: "?"
        font.bold:true
        font.pointSize: quitButton.font.pointSize + 2
        onPressed: selectHelp()
      }
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
          onClicked: {
            alertList.visible = !alertList.visible
            alertList.raisedManually = alertList.visible
            alertList.stopTimer ()
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
          eventList.visible = false
          itemEdit.startNew ()
        }
      }
      Image {
        id: maintainButton
        height: mainBox.rowHeight
        width: mainBox.rowHeight
        source: ":/icons/gear.png"
        MouseArea {
          anchors.fill: parent
          onPressed: {
            console.log ("want maintain")
          }
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
    opacity: 0.95
    anchors {
      top: brandingBox.bottom; topMargin: brandingBox.height
      horizontalCenter: brandingBox.horizontalCenter
    }
  }
 
  MainEventList {
    id:eventList
    model:cppEventListModel
    width: mainBox.mainWidth
    height: mainBox.mainHeight - brandingBox.height
    mainWidth: width
    nickWidth: mainBox.nickWidth
    rowHeight: mainBox.rowHeight
    mainColor: mainBox.mainColor
    currentIndex: -1
    clip: true
    anchors {
      top:brandingBox.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
    onDeleteEvent: {
      mainBox.deleteEvent (uuid)
    }
    onWantModify: {
      eventList.visible = false
      itemEdit.startModify (uuid, title, when, what, command,
                            audible, repeatMinutes)
    }
  }
  
  AlertList {
    id: alertList
    visible: false
    model:cppPastEventListModel
    width: mainBox.mainWidth * 0.8
    height: mainBox.mainHeight - brandingBox.height
    mainWidth: width
    nickWidth: mainBox.nickWidth
    rowHeight: mainBox.rowHeight
    mainColor: "#f0f070"
    currentIndex: -1
    clip: true
    anchors {
      top:brandingBox.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
    
    onWantClose: {
      visible = false
      //hideTimer.stop ()
    }
    onWantClear: {
      cppPastEventListModel.clear ()
    }
    onWantClearEvent:{
      cppPastEventListModel.clearEvent (uuid)
    }
    
  }
  
  ItemEdit {
    id: itemEdit
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
      eventList.visible = true
      mainBox.saveNewEvent (theUuid, theTitle, theTime, theDescription, 
                            command, audible, repeatMinutes)
    }
    onCancelled: {
      eventList.visible = true
    }
  }
  Component.onCompleted: {
    orientationWatcher.start()
  }
}
