import QtQuick 2.0
import QtQuick.Window 2.0

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
    property rect targetRect:item.mapToItem(null,0,0,width,height)
    function refreshPosition(){
        targetRect=item.mapToItem(null,0,0,width,height)
    }
}

