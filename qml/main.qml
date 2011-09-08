import QtQuick 1.0

Rectangle {
  width: 480
  height: 480
  id: mainBox
  color: "red"
  visible: true
  property real mainWidth: width
  property real mainHeight: height
  property real rowHeight: 42
  signal quit ()
  Row {
    id:buttonRow
    ChoiceButton {
      id:quitButton
      labelText: qsTr ("Quit")
      topColor: "#00ccff"
      onClicked: { mainBox.quit() }
      height:mainBox.rowHeight
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
      Row {
        id: eventListRow
        Text {
          id: eventTitleText
          text: eventTitle + " "
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
        Text {
          id: eventTimeText
          text: eventWhen + " "
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
        Text {
          id: eventWhatText
          text: eventWhat + " "
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
      }
      MouseArea {
        anchors.fill:parent
        onClicked: {
          eventList.currentIndex = index
          console.log ("clicked to make index " + eventList.currentIndex)
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
}
