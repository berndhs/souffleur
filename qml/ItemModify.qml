import QtQuick 1.0


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


Item {
  id: modifyItemMenu
  property real buttonHeight: 42
  property real buttonWidth: 84
  property real buttonRadius: buttonHeight * 0.5
  property color buttonTopShade: "#f03333"
  property color buttonBottomShade: "#f0f0f0"
  property bool enableCopy: true
  property bool enablePaste: true
  property bool enableSelect: true

  signal wantCopy ()
  signal wantChange ()
  signal wantDelete ()

  Gradient {
    id: buttonShade
    GradientStop { position: 0.00; color: modifyItemMenu.buttonTopShade }
    GradientStop { position: 1.00; color: modifyItemMenu.buttonBottomShade }
  }

  Flow { 
    ChoiceButton {
      id: modifyButton
      visible: modifyItemMenu.enablePaste
      width: modifyItemMenu.buttonWidth
      height: modifyItemMenu.buttonHeight
      radius: modifyItemMenu.buttonRadius
      labelText: qsTr ("Modify")
      gradient: buttonShade
      onClicked: {
        modifyItemMenu.visible = false
        modifyItemMenu.wantChange ()
      }
    }
    ChoiceButton {
      id: deleteButton
      visible: modifyItemMenu.enablePaste
      width: modifyItemMenu.buttonWidth
      height: modifyItemMenu.buttonHeight
      radius: modifyItemMenu.buttonRadius
      labelText: qsTr ("Delete")
      gradient: buttonShade
      onClicked: {
        modifyItemMenu.visible = false
        modifyItemMenu.wantDelete ()
      }
    }      
  }
} 
