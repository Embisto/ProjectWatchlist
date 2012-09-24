import QtQuick 1.0

MouseArea {

  width:25
  height:25
  smooth:true
  onReleased: NumberAnimation { target: addButtonActive; property: "opacity"; to: 0; duration: 100;}
  onPressed: NumberAnimation { target: addButtonActive; property: "opacity"; to: 1; duration: 100;}

    Image {
        source:"qrc:../..///img/rightButton.png"
           }

    Image {

    id:addButtonActive
    opacity:0
    source:"qrc:../..///img/rightButton_Active.png"

}}