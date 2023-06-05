import QtQuick 2.0
import QtQuick.Layouts 1.3
Item {
    id: root
    property string strTitle:"Hệ thống theo dõi vật liệu LAZER"
    property int titlePointSize: 35
    width: 500
    height: 300

    Rectangle
    {
        id: outSizeRect
        color: "skyblue"
        border.width: 1
        anchors.fill: root
        anchors.margins: 5

        ColumnLayout{
            id: column
            anchors.fill: parent
            Rectangle{
                Layout.fillWidth: true
                height: root.height/3
                color: "transparent"
                Text {
                    id: title
                    text: strTitle
                    font.pointSize: titlePointSize
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent

                }
            }


            ListView
            {
                id: mlistView
                orientation: ListView.Horizontal
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: modelBanner
                delegate: Rectangle
                {
                    id:delegate
                    height: mlistView.height
                    width: mlistView.width/4
                    color:"transparent"
                    Row{
                        width: parent.width
                        height: parent.height/2
                        anchors.centerIn: parent
                        Rectangle{
                            width: parent.width/3
                            height: parent.height

                            color:"transparent"
                            Rectangle{
                                id:displayRect
                                anchors.fill: parent
                                anchors.margins: parent.height/4
                                border.width: 1
                                color: colorState

                            }
                        }
                        Text {
                            width: parent.width*2/3
                            height: parent.height
                            id: displayText
                            text: txtState
                            font.bold: true
                            font.pointSize: 25
                            wrapMode: Text.WordWrap
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }

            }
        }
    }
    ListModel{
        id: modelBanner
        ListElement{txtState:"Đủ vật liệu sản xuất";colorState:"green"}
        ListElement{txtState:"Cần cung cấp vật liệu";colorState:"yellow"}
        ListElement{txtState:"AGV đang di chuyển";colorState:"purple"}
        ListElement{txtState:"Line off";colorState:"gray"}
    }

}
