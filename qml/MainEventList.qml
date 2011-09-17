import QtQuick 1.0

ListView {  
  id: listViewItem

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
      width:listViewItem.mainWidth
      color: isCurrent ? Qt.lighter (listViewItem.mainColor, 1.2) : listViewItem.mainColor
      property bool isCurrent: index == eventList.currentIndex
      property real restWidth: width - eventTimeText.width 
                               - audibleIconBox.width - eventTitleText.width 
                               - (eventListRow.itemCount - 1) * eventListRow.spacing
      
      
      Flow {
        id: eventListRow
        spacing: 4
        //height: Math.max (listViewItem.rowHeight, eventWhatText.height)
        width: normalDelegateRow.width
        property int itemCount: 4
        Text {
          id: eventTitleText
          text: eventTitle 
          width: listViewItem.nickWidth
          height:listViewItem.rowHeight
          font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
        }
        Rectangle {
          id: audibleIconBox
          width: 0.5* listViewItem.rowHeight
          height: listViewItem.rowHeight
          color: "transparent"
          Image {
            id: audibleImage
            width: 0.5 * listViewItem.rowHeight
            height: 0.5 * listViewItem.rowHeight
            anchors.top: parent.top
            source: eventAudible ? ":/icons/bell.png" : ""
          }
        }
        Rectangle {
          id: repeatIconBox
          width: 0.5* listViewItem.rowHeight
          height: listViewItem.rowHeight
          color: "transparent"
          Image {
            id: repeatImage
            width: 0.5 * listViewItem.rowHeight
            height: 0.5 * listViewItem.rowHeight
            anchors.top: parent.top
            source: eventRepeating ? ":/icons/R.png" : ""
          }
        }
        Text {
          id: eventTimeText
          text: eventWhen
          height:listViewItem.rowHeight
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
            width: listViewItem.mainWidth
            height: Math.max (eventWhatText.height, listViewItem.rowHeight)
            color: Qt.darker (normalDelegateRow.color, 1.1)
            
            Text {
              id: eventWhatText
              text: eventWhat 
              width: Math.max (listViewItem.nickWidth, normalDelegateRow.restWidth)
              wrapMode:Text.Wrap
              font.weight: normalDelegateRow.isCurrent ? Font.Bold : Font.Normal
            }
          }

        }
      }
      ItemModify {
        id: itemModify
        height: listViewItem.rowHeight
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
          listViewItem.wantModify (eventUuid, eventTitle, eventWhen, eventWhat, 
                                   eventCommand, eventAudible, eventRepeatMinutes) 
        }
        onWantDelete: {
          listViewItem.deleteEvent (eventUuid)
        }
      }
    }
  }
}


