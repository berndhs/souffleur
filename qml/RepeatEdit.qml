import QtQuick 1.0

Rectangle {
  id:mainBox
  width: 400
  height: isPhone ? 64 : 42
  property real value: daysBox.value * 1440
                      + hoursBox.value * 60
                      + minsBox.value
  property bool accepted: false
  property bool isPhone: false
  property real arrowHeight: isPhone ? 22 : 12
  property alias timeDays: daysBox.daysValue
  property alias timeHours: hoursBox.hoursValue
  property alias timeMinutes: minsBox.minsValue
  
  function setValues (theDays, theHours, theMins) {
    daysBox.daysValue = theDays
    daysBox.value = theDays.toFixed(0)
    hoursBox.hoursValue = theHours
    hoursBox.value = theHours.toFixed(0)
    minsBox.minsValue = theMins
    minsBox.value = theMins.toFixed(0)
  }
  
  signal acceptedValue (real value)
  
  Flow {
    SpinBox {
      id: daysBox
      label: "Days"
      value:  daysValue.toFixed (0)
      property int daysValue: 0
      iconHeight: mainBox.arrowHeight
      onUp: {
        daysValue += 1
        value = daysValue.toFixed(0)
        mainBox.accepted = false
      }
      onDown: {
        daysValue -= 1
        if (daysValue < 0) { daysValue = 0 }
        value = daysValue.toFixed(0)
        mainBox.accepted = false
      }
    }
    SpinBox {
      id: hoursBox
      label: "Hours"
      value: hoursValue.toFixed (0)
      property int hoursValue: 0
      iconHeight: mainBox.arrowHeight
      onUp: {
        hoursValue += 1
        value = hoursValue.toFixed(0)
        mainBox.accepted = false
      }
      onDown: {
        hoursValue -= 1
        if (hoursValue < 0) { hoursValue = 0 }
        value = hoursValue.toFixed(0)
        mainBox.accepted = false
      }
    }
    SpinBox {
      id: minsBox
      label: "Mins"
      value: minsValue.toFixed (0)
      property int minsValue: 0
      iconHeight: mainBox.arrowHeight
      onUp: {
        minsValue += 1
        value = minsValue.toFixed(0)
        mainBox.accepted = false
      }
      onDown: {
        minsValue -= 1
        if (minsValue < 0) { minsValue = 0 }
        value = minsValue.toFixed(0)
        mainBox.accepted = false
        console.log ("mins: " + minsValue)
      }
    }
    Image {
      id: checkedIcon
      visible: mainBox.accepted
      source: ":/icons/marked.png"
      width: mainBox.height
      height: mainBox.height
      fillMode:Image.Stretch
    }

    ChoiceButton {
      id: okButton
      labelText: qsTr ("OK")
      height: mainBox.height
      radius: height * 0.5
      topColor: "#99ee99"
      onClicked: {
        mainBox.acceptedValue (mainBox.value)
        mainBox.accepted = true
        console.log ("repeat value " + mainBox.value + " accpeted ")
      }
    }
  }
}
