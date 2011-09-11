// This file used to be part of Cosmographia. It is reused per LGPL as below
//
// Copyright (C) 2011 Chris Laurel <claurel@gmail.com>
//
// Cosmographia is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Cosmographia is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with Cosmographia. If not, see <http://www.gnu.org/licenses/>.

import QtQuick 1.0

FocusScope
{
  id: container
  width: column.width; height: column.height
  
  property real value: 0
  property alias label: labelBox.text
  property color color: "white"
  property color labelColor: "black"
  property color valueColor: "black"
  property real labelSpacing: 4
  property real iconPadding: 8
  property real iconHeight: 12
  
  signal up()
  signal down()
  
  Column
  {
    id: column
    spacing: 0
    
    Item {
      anchors.horizontalCenter: textRow.horizontalCenter
      width: upArrow.width + iconPadding; height: upArrow.height + iconPadding
      
      Image {
        id: upArrow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: textRow.width; height: container.iconHeight
        
        source: ":/icons/up.png"
        smooth: true
        opacity: container.focus ? 1 : 0.4
      }
      
      MouseArea {
        anchors.fill: parent
        onPressed: { container.focus = true; container.up() }
      }
    }
    
    Row {
      id: textRow
      spacing: container.labelSpacing
      Text {
        id:labelBox
        color: container.labelColor
        text: "Label"
      }
      
      Text {
        id: textBox
        color: container.valueColor
        text: container.value.toString()
        MouseArea {
          anchors.fill: parent
          onPressed: { container.focus = true }
        }
      }
    }
    
    Item {
      anchors.horizontalCenter: textRow.horizontalCenter
      width: downArrow.width + iconPadding; height: downArrow.height + iconPadding
      
      Image {
        id:  downArrow
        anchors.horizontalCenter: parent.horizontalCenter
        width: textRow.width; height: container.iconHeight
        
        source: ":/icons/down.png"
        smooth: true
        opacity: container.focus ? 1 : 0.4
      }
      
      MouseArea {
        anchors.fill: parent
        onPressed: { container.focus = true; container.down() }
      }
    }
  }
  
  Keys.onUpPressed: {
    container.up()
  }
  
  Keys.onDownPressed: {
    container.down()
  }
}

