import QtQuick 2.0

QtObject{
    property Item item:Item{
        x:0
        y:0
        width:100
        height:100
        visible:false
    }
    property bool visible:item.visible
    property real x:item.x
    property real y:item.y
    property real width:item.width
    property real height:item.height
    readonly property rect targetRect:item.mapToItem(null,x-item.x,y-item.y,width,height)
//    function refreshPosition(){
//        targetRect=Qt.binding(function(){item.mapToItem(null,x-item.x,y-item.y,width,height)})
//    }
}

