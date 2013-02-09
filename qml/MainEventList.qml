import QtQuick 1.0

ListView {  
  id: listViewContainer

  property real mainWidth: 1024
  property real nickWidth: 512
  property real rowHeight: 42
  property color mainColor: "yellow"
  signal deleteEvent (string uuid)
  signal wantModify (string uuid, string title, string when, string what, 
                     string command, bool audible, int repeatMinutes)

  
  delegate: Component {
    id:eventListDelegate
    Rectangle {
      id:normalDelegateRow
      height:eventListRow.height
      width:listViewContainer.width
      color: isCurrent ? Qt.lighter (listViewContainer.mainColor, 1.2) : listViewContainer.mainColor
      property bool isCurrent: index == eventList.currentIndex
      property real restWidth: width - eventTimeText.width 
                               - audibleIconBox.width - eventTitleText.width 
                               - (eventListRow.itemCount - 1) * eventListRow.spacing
      
      
      Flow {
        id: eventListRow
        spacing: 4
        //height: Math.max (listViewContainer.rowHeight, eventWhatText.height)
        width: normalDelegateRow.width
        property int itemCount: 4
        Text {
          id: eventTitleText
          text: eventTitle 
          width: listViewContainer.nickWidth
          height:listViewContainer.rowHeight
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
        Rectangle {
          id: audibleIconBox
          width: 0.5* listViewContainer.rowHeight
          height: listViewContainer.rowHeight
          color: "transparent"
          Image {
            id: audibleImage
            width: 0.5 * listViewContainer.rowHeight
            height: 0.5 * listViewContainer.rowHeight
            anchors.top: parent.top
            source: eventAudible ? ":/icons/bell.png" : ""
          }
        }
        Rectangle {
          id: repeatIconBox
          width: 0.5* listViewContainer.rowHeight
          height: listViewContainer.rowHeight
          color: "transparent"
          Image {
            id: repeatImage
            width: 0.5 * listViewContainer.rowHeight
            height: 0.5 * listViewContainer.rowHeight
            anchors.top: parent.top
            source: eventRepeating ? ":/icons/R.png" : ""
          }
        }
        Text {
          id: eventTimeText
          text: eventWhen
          height:listViewContainer.rowHeight
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
          
          MouseArea {
            anchors.fill: parent
            onClicked: {
              itemModify.visible = !itemModify.visible
              console.log (" changed modify visible " + itemModify.visible)
            }
          }
          
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
            height: Math.max (eventWhatText.height, listViewContainer.rowHeight)
            color: Qt.darker (normalDelegateRow.color, 1.1)
            border.color: Qt.lighter (color); border.width: 2
            Text {
              id: eventWhatText
              text: eventWhat 
              width: eventWhatBox.width
              anchors { leftMargin: 3; rightMargin: 3 }
              wrapMode:Text.Wrap
              font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
            }
          }

        }
      }
      ItemModify {
        id: itemModify
        height: listViewContainer.rowHeight
        width: childrenRect.width
        anchors {
          top: normalDelegateRow.verticalCenter
          horizontalCenter: normalDelegateRow.horizontalCenter 
        }
        visible: false
        z: normalDelegateRow.z + 1
        onVisibleChanged: {
          console.log (" itemModify visible now " + visible + " for index " + index)
        }
        onWantChange: {
          listViewContainer.wantModify (eventUuid, eventTitle, eventWhen, eventWhat, 
                                   eventCommand, eventAudible, eventRepeatMinutes) 
        }
        onWantDelete: {
          listViewContainer.deleteEvent (eventUuid)
        }
      }
    }
  }
}


