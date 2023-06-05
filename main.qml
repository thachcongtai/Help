import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import com.thai.class 1.0

Window {
    visible: true
    id:root
    width: 1894
    height: 1280
    property int gridColumns
    property int gridRows: 3
    property real minCellWidth: 400
    property real minCellHeigh: 55
    property bool clearCount: false

    function isTimeBetween(date, h1, m1, h2, m2) {
      var h = date.getHours();
      var m = date.getMinutes();
      return (h1 < h || h1 === h && m1 <= m) && (h < h2 || h === h2 && m <= m2);
    }

    function gridViewEdit(){
        var column = Math.min(Math.floor(view.width/minCellWidth),lineModel.rowCount())
//        console.log(lineModel.rowCount())
        gridRows =Math.ceil(lineModel.rowCount()/column)
        gridColumns = Math.ceil(lineModel.rowCount()/gridRows)
//        console.log(gridColumns,gridRows,column)
        view.height = gridRows*minCellHeigh
        view.cellWidth = view.width/gridColumns
        view.cellHeight = minCellHeigh
//
//        console.log(root.height,rectHeader.height,gridRect.height,root.height-rectHeader.height-view.height)
//        console.log(gridColumns,view.cellWidth,view.cellHeight,view.width,view.height,root.width,gridRows)
    }

    onWidthChanged: {
        gridViewEdit()
    }
    Component.onCompleted: {
        console.log("Window completed")
    }

    Timer{
        id: globalTimer
        repeat: true
        running: true
        interval: 250
        property int count: 0
    }

    ColumnLayout{
        spacing: 0
        anchors.fill: parent
        Rectangle{
            id:rectBanner
            Layout.fillWidth: true
//            Layout.fillHeight: true
            height: 320
            Banner
            {
                id: banner
                anchors.fill: parent
                titlePointSize: 35
                strTitle:"HỆ THỐNG THEO DÕI VẬT LIỆU LAZER"
            }
        }

        Rectangle{
            //header rectangle
            id:rectHeader
            Layout.fillWidth: true
            width: parent.width
            height: 80
            Row{
                anchors.fill: parent
                Repeater{
                    model: gridColumns

                    delegate: GridRowHeader{
                        width: view.cellWidth
                    }

                }
            }
        }

        Rectangle{
            // grid rectangle
            id:gridRect
            width: parent.width
            Layout.fillWidth: true
            Layout.fillHeight: true
//            height: view.height
//            Layout.fillHeight: true
//            height: 880
            GridView {
                id: view
                width: parent.width

                keyNavigationWraps: false
                interactive: false
                flow: GridView.FlowTopToBottom
                model: lineModel
                delegate: GridCellDelegate{
                    id:gridDelegate
                    width: view.cellWidth
                    height:view.cellHeight

                    //int property
                    line: mLineID
                    mtimeFront: mTimeFront
                    mtimePBA: mTimePba
                    countFront: mCountFront
                    countPBA: mCountPba
                    stateFront: mStateFront
                    statePBA: mStatePba
                    active: mActive
                    textPointSize: 20

                    onSetActive: {
                        mActive = result
                        mStateFront =1
                        mStatePba =1
                        lineModel.resetTimePba(index)
                        lineModel.resetTimeFront(index)
                        mtimeFront = mTimeFront
                        mtimePBA = mTimePba
                    }

                    onResetCountFront: lineModel.resetCountFront(index)
                    onResetCountPba: lineModel.resetCountPba(index)
                    onResetTimePba:{
                        console.log(index)
                        lineModel.resetTimePba(index)
                    }
                    onResetTimeFront: {
                        console.log(index)
                        lineModel.resetTimeFront(index)
                    }
                    Component.onCompleted: {
                        gridViewEdit()
                    }
                    Connections{
                        target: globalTimer
                        onTriggered:
                        {
                            if(active==true)
                            {
                                if(mStateFront!=0)
                                {
                                    mtimeFront = mTimeFront
                                }
                                else
                                {
                                    mtimeFront =""
                                }
                                if(mStatePba!=0)
                                {
                                    mtimePBA = mTimePba
                                }
                                else
                                {
                                    mtimePBA = ""
                                }
                            }
                            else
                            {
                                mtimeFront =""
                                mtimePBA = ""
                            }

                            globalTimer.count++
                            if(globalTimer.count>5)
                            {
                                lineModel.checkLineIsConnected()
                                if(isTimeBetween(new Date(),07,30,07,30)||isTimeBetween(new Date(),19,30,19,30))
                                {
                                    if(clearCount===false)
                                    {
                                        console.log("clear count"+index)
                                        clearCount=true
                                        lineModel.clearCount()
                                    }
                                }
                                else{
                                    clearCount=false
                                }

                                globalTimer.count=0
                            }
                        }


                    }
                }
                Component.onCompleted: {
                    console.log("View completed")
                }

            }
            Component.onCompleted: {
                console.log("Rect completed")
            }
        }
    }





}
