import QtQuick 2.0
import QtQuick.Dialogs 1.2

Rectangle{

    property string txtUid
    property int line
    property string mtimeFront
    property int countFront
    property string mtimePBA
    property int countPBA
    property bool active:true
    property int refWidth:350
    property int refMargin: 5
    property string msgTitle: "Confirm?"
    property string msgTxt: "Do you want change status"
    property int textPointSize: 15
    property bool completed : false

    signal setActive(bool result)
    signal resetTimeFront()
    signal resetTimePba()
    signal resetCountFront()
    signal resetCountPba()

    property int stateFront
    property int statePBA

    function nextStagePBA(){
        if(statePBA===3) statePBA=0
        else statePBA +=1
    }

    id:root

    onWidthChanged: {

        if(completed && width>refWidth)
        {
            textPointSize = Math.round(frontTimeRect.width/12)
        }
    }
    Component.onCompleted: {
        var timer = Qt.createQmlObject("import QtQuick 2.0; Timer {}", root)
        timer.interval = 300
        timer.repeat = false
        timer.triggered.connect(function(){
            textPointSize = Math.round(frontTimeRect.width/12)
            completed = true;
            timer.destroy();
        })
        timer.start()
    }



    Row {
        id: row
        anchors.rightMargin: root.refMargin
        anchors.leftMargin: root.refMargin
        anchors.fill: parent
        CustomBorderRect{
            borderColor: "black"
            borderWidth: 1
            rBorderwidth: 0
            tBorderwidth: 0
            id: lineIdRect
            width: 80*(root.width)/refWidth
            color: active?"white":"gray"

            anchors.bottom: parent.bottom
            anchors.top: parent.top
            Text {
                text: "Line "+line
                anchors.fill: parent
                font.pointSize: textPointSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    root.msgTxt ="bạn có muốn thay đổi trạng thái của line " +root.line
                    var dlg = msgDialog.createObject(root)
                    dlg.accepted.connect(function(){
                        setActive(!active)
                    })
                    dlg.rejected.connect(function(){

                    })
                    dlg.visible = true
                }
            }

        }

        CustomBorderRect{
            borderColor: "black"
            borderWidth: 1
            rBorderwidth: 0
            tBorderwidth: 0
            id: frontTimeRect
            width: 180*(root.width)/refWidth
            height: root.height;
            color: "gray"
            Text {
                id: frontTimeText
                text: mtimeFront
                font.pointSize: textPointSize
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                enabled: root.active
                onClicked: {
                    root.msgTxt ="Bạn có muốn đặt lại thời gian của line " +root.line
                    var dlg = msgDialog.createObject(root)
                    dlg.accepted.connect(function(){
                        resetTimeFront()
                    })
                    dlg.visible = true
                }

            }

            states:[
                State {
                    name: "Front not connect"
                    when: stateFront === 0 || active ===false
                    PropertyChanges {
                        target: frontTimeRect
                        color: "gray"
                    }
                    PropertyChanges {
                        target: frontCountRect
                        color: "gray"
                    }
                },
                State {
                    name: "Front calling"
                    when: stateFront === 1
                    PropertyChanges {
                        target: frontTimeRect
                        color: "yellow"
                    }
                    PropertyChanges {
                        target: frontCountRect
                        color: "yellow"
                    }
                },
                State {
                    name: "Front AGV"
                    when: stateFront === 2
                    PropertyChanges {
                        target: frontTimeRect
                        color: "purple"
                    }
                    PropertyChanges {
                        target: frontCountRect
                        color: "purple"
                    }
                },
                State {
                    name: "Front FullBox"
                    when: stateFront === 3
                    PropertyChanges {
                        target: frontTimeRect
                        color: "green"
                    }
                    PropertyChanges {
                        target: frontCountRect
                        color: "green"
                    }
                }]


        }
        CustomBorderRect{
            borderColor: "black"
            borderWidth: 1
            rBorderwidth: 0
            tBorderwidth: 0
            id: frontCountRect
            width: 2*40*(root.width)/refWidth
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            color: "gray"
            Text {
                id: frontTimeCount
                text: countFront
                font.pointSize: textPointSize
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                enabled: root.active
                onClicked: {
                    root.msgTxt ="Bạn có muốn đặt lại số lượt của line " +root.line
                    var dlg = msgDialog.createObject(root)
                    dlg.accepted.connect(function(){
                        resetCountFront()
                    })
                    dlg.visible = true
                }
            }


        }
//        CustomBorderRect{
//            borderColor: "black"
//            borderWidth: 1
//            rBorderwidth: 0
//            tBorderwidth: 0
//            id: pbaTimeRect
//            width: 90*(root.width)/refWidth
//            height: root.height;
//            color: "gray"
//            Text {
//                id: pbaTimeText
//                text: mtimePBA
//                font.pointSize: textPointSize
//                anchors.fill: parent
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//            }
//            MouseArea{
//                enabled: root.active
//                anchors.fill: parent
//                onClicked: {
//                    root.msgTxt ="Bạn có muốn đặt lại thời gian của line " +root.line
//                    var dlg = msgDialog.createObject(root)
//                    dlg.accepted.connect(function(){
//                        resetTimePba()
//                    })
//                    dlg.visible = true
//                }
//            }
//            states:[
//                State {
//                    name: "PBA not connect"
//                    when: statePBA === 0 || active ===false
//                    PropertyChanges {
//                        target: pbaTimeRect
//                        color: "gray"
//                    }
//                    PropertyChanges {
//                        target: pbaCountRect
//                        color: "gray"
//                    }
//                },
//                State {
//                    name: "PBA calling"
//                    when: statePBA === 1
//                    PropertyChanges {
//                        target: pbaTimeRect
//                        color: "yellow"
//                    }
//                    PropertyChanges {
//                        target: pbaCountRect
//                        color: "yellow"
//                    }
//                },
//                State {
//                    name: "PBA AGV"
//                    when: statePBA === 2
//                    PropertyChanges {
//                        target: pbaTimeRect
//                        color: "purple"
//                    }
//                    PropertyChanges {
//                        target: pbaCountRect
//                        color: "purple"
//                    }
//                },
//                State {
//                    name: "PBA FullBox"
//                    when: statePBA === 3
//                    PropertyChanges {
//                        target: pbaTimeRect
//                        color: "green"
//                    }
//                    PropertyChanges {
//                        target: pbaCountRect
//                        color: "green"
//                    }
//                }]
//        }
//        CustomBorderRect{
//            borderColor: "black"
//            borderWidth: 1
//            tBorderwidth: 0
//            //            rBorderwidth: 0
//            id: pbaCountRect
//            width: 40*(root.width)/refWidth
//            anchors.bottom: parent.bottom
//            anchors.top: parent.top
//            color: "gray"
//            Text {
//                id: pbaTimeCount
//                text: countPBA
//                font.pointSize: textPointSize
//                anchors.fill: parent
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//            }
//            MouseArea{
//                enabled: root.active
//                anchors.fill: parent
//                onClicked: {
//                    root.msgTxt ="Bạn có muốn đặt lại số lượt của line " +root.line
//                    var dlg = msgDialog.createObject(root)
//                    dlg.accepted.connect(function(){
//                        resetCountPba()
//                    })
//                    dlg.visible = true
//                }
//            }
//        }
    }


    Component{
        id: msgDialog

        MessageDialog {
            title: root.msgTitle
            text: root.msgTxt
            onVisibleChanged: if(!visible) destroy(1)
            standardButtons: StandardButton.Cancel | StandardButton.Ok
        }
    }
}


