import QtQuick 2.0

Rectangle {
    property int refWidth:350
    property int refMargin: 5
    property color colorHeader: "skyblue"
    id:root
    height: 80
    width : 340

    Row {
        id: row
        anchors.rightMargin: root.refMargin
        anchors.leftMargin: root.refMargin
        anchors.topMargin: root.refMargin
        anchors.fill: parent
        CustomBorderRect{
            id: lineIdRect
            color: colorHeader
            borderColor: "black"
            borderWidth: 1
            rBorderwidth: 0
            width: 80*(root.width)/refWidth
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            Text {
                text: "Line"
                anchors.fill: parent
                font.pointSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        CustomBorderRect{
            id: frontRect
            borderWidth: 0
            width: 2*130*(root.width)/refWidth
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            Column{
                anchors.fill: parent
                CustomBorderRect{
                    color: colorHeader
                    borderColor: "black"
                    borderWidth: 1
                    rBorderwidth: 0
                    bBorderwidth: 0
                    width: frontRect.width
                    height: frontRect.height/2
                    Text {
                        text: "Back GLass"
                        anchors.fill: parent
                        font.pointSize: 15
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row{
                    width: frontRect.width
                    CustomBorderRect{
                        color: colorHeader
                        borderColor: "black"
                        borderWidth: 1
                        width: frontRect.width*90/130
                        height: frontRect.height/2
                        Text {
                            text: "Time"
                            anchors.fill: parent
                            font.pointSize: 15
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    CustomBorderRect{
                        color: colorHeader
                        borderColor: "black"
                        tBorderwidth:  1
                        bBorderwidth: 1
                        width: frontRect.width*40/130
                        height: frontRect.height/2
                        Text {
                            text: "C"
                            anchors.fill: parent
                            font.pointSize: 15
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
//        CustomBorderRect{
//            id: pbaRect
//            borderWidth: 0
//            width: 130*(root.width)/refWidth
//            anchors.bottom: parent.bottom
//            anchors.top: parent.top
//            Column{
//                anchors.fill: parent
//                CustomBorderRect{
//                    color: colorHeader
//                    borderColor: "black"
//                    borderWidth: 1
//                    bBorderwidth: 0
//                    width: pbaRect.width
//                    height: pbaRect.height/2
//                    Text {
//                        text: "PBA"
//                        anchors.fill: parent
//                        font.pointSize: 15
//                        horizontalAlignment: Text.AlignHCenter
//                        verticalAlignment: Text.AlignVCenter
//                    }
//                }
//                Row{
//                    width: pbaRect.width
//                    CustomBorderRect{
//                        color: colorHeader
//                        borderColor: "black"
//                        borderWidth: 1
//                        width: pbaRect.width*90/130
//                        height: pbaRect.height/2
//                        Text {
//                            text: "Time"
//                            anchors.fill: parent
//                            font.pointSize: 15
//                            horizontalAlignment: Text.AlignHCenter
//                            verticalAlignment: Text.AlignVCenter
//                        }
//                    }
//                    CustomBorderRect{
//                        color: colorHeader
//                        borderColor: "black"
//                        tBorderwidth:  1
//                        bBorderwidth: 1
//                        rBorderwidth: 1
//                        width: pbaRect.width*40/130
//                        height: pbaRect.height/2
//                        Text {
//                            text: "C"
//                            anchors.fill: parent
//                            font.pointSize: 15
//                            horizontalAlignment: Text.AlignHCenter
//                            verticalAlignment: Text.AlignVCenter
//                        }
//                    }
//                }
//            }
//        }
    }

}
