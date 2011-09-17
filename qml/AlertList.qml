import QtQuick 1.0


Rectangle {
  id: mainBox
  property real mainWidth: 1024
  property real nickWidth: 512
  property real rowHeight: 42
  property color mainColor: "yellow"
  property alias model: listViewContainer.model
  property alias currentIndex: listViewContainer.currentIndex
  property alias count: listViewContainer.count
  
  signal wantClose ()
  signal wantClear ()
  signal wantClearEvent (string uuid)
  
  function doCheck () {
   // console.log ("qml checking PastEventList " + count)
    console.log (" qml PastEventList model " + model)
    console.log (" PastEventList box w/h " + width + " / " + height)
    console.log (" PastEventList view w/h " + listViewContainer.width + "/" + listViewContainer.height)
    console.log (" PastEventList content w/h " + listViewContainer.contentWidth +"/" + listViewContainer.contentHeight)
  }
  
  Rectangle {
    id: headerBox
    width: mainBox.mainWidth
    height: mainBox.rowHeight
    color: mainBox.mainColor
    Row {
      spacing: 16
      anchors.centerIn: parent
      Rectangle {
        id: titleBox
        width: titleBoxText.width * 1.5
        height: mainBox.rowHeight
        color: "transparent"
        Text {
          id: titleBoxText
          anchors.centerIn: parent
          font.bold: true
          text: qsTr ("Recent Alerts")
        }
        MouseArea {
          anchors.fill: parent
          onPressAndHold: {
            mainBox.wantClose ()
          }
        }
      }
      ChoiceButton {
        id: clearButton
        labelText: qsTr ("Clear")
        height: mainBox.rowHeight
        radius: height * 0.5
        topColor: "#aaff00"
        onPressed: {
          mainBox.wantClear ()
        }
      }
    }


  }
  
  ListView {  
    id: listViewContainer
    width: mainBox.width
    height: mainBox.height - headerBox.height
    anchors {
      top: headerBox.bottom
      horizontalCenter: mainBox.horizontalCenter
    }

    delegate: Rectangle {
      id:normalDelegateRow
      
      height:eventListRow.height
      width:mainBox.mainWidth
      color: isCurrent ? Qt.lighter (mainBox.mainColor, 1.2) : mainBox.mainColor
      property bool isCurrent: false
      property real restWidth: width - eventTimeText.width 
                               - eventTitleText.width 
                               - (eventListRow.itemCount - 1) * eventListRow.spacing
        
      Flow {
        id: eventListRow
        spacing: 4
        width: normalDelegateRow.width
        property int itemCount: 4
        ChoiceButton {
          id: clearEventButton
          labelText: qsTr ("Clear")
          height:mainBox.rowHeight
          topColor:"#aaff00"
          radius: height * 0.5
          onPressed: {
            wantClearEvent (eventUuid)
          }
        }

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
        Row {
          id: eventWhatRow
          Rectangle {
            id: eventWhatIndent
            width: 16; height: eventWhatBox.height
            color: "transparent"            
          }
          Rectangle {
            id: eventWhatBox
            width: listViewContainer.width - eventWhatIndent.width
            height: Math.max (eventWhatText.height, mainBox.rowHeight)
            color: Qt.darker (normalDelegateRow.color, 1.1)
            border.color: Qt.lighter (color); border.width: 2
            
            Text {
              id: eventWhatText
              text: eventWhat 
              width:eventWhatBox.width
              wrapMode:Text.Wrap
              anchors { leftMargin: 3; rightMargin: 3 }
              font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
            }
          }          
        }
      }
    }       
  }
}

