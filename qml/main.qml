import QtQuick 1.0

Rectangle {
  width: 480
  height: 480
  id: mainBox
  color: "#eeeebb"
  visible: true
  property real mainWidth: width
  property real mainHeight: height
  property real rowHeight: 42
  property real nickWidth: mainWidth * 0.25
  signal quit ()
  Row {
    id:buttonRow
    spacing:32
    ChoiceButton {
      id:quitButton
      labelText: qsTr ("Quit")
      topColor: "#00ccff"
      height:mainBox.rowHeight
      radius: height * 0.5
      onPressed: { mainBox.quit() }
    }
    ChoiceButton {
      id:addButton
      labelText: (qsTr("+"))
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
        Rectangle {
          width:mainBox.mainWidth * 0.6667
          anchors { left: parent.left; leftMargin: mainBox.nickWidth * 0.5 }
          height:1
          color: "white"
          visible:index > 0
        }
        
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
    height: mainBox.mainHeight * 0.75
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
  }
}
