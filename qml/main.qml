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
    Row {
      id: eventListRow
      Text {
        id: eventTitleText
        text: eventTitle
      }
      Text {
        id: eventTimeText
        text: eventWhen
      }
      Text {
        id: eventWhatText
        text: eventWhat
      }
    }
  }
  Component {
    id:eventListCurrentDelegate
    Row {
      id: eventListRow
      Text {
        id: eventTitleText
        text: eventTitle
        color: "red"
      }
      Text {
        id: eventTimeText
        text: eventWhen
        color: "red"
      }
      Text {
        id: eventWhatText
        text: eventWhat
        color: "red"
      }
    }
  }

  ListView {
    id:eventList
    model:cppEventListModel
    delegate: eventListDelegate
    highlight:eventListCurrentDelegate
    width: mainBox.mainWidth
    height: mainBox.mainHeight * 0.75
    clip: true
    anchors {
      top:buttonRow.bottom
      horizontalCenter: mainBox.horizontalCenter
    }
  }
}
