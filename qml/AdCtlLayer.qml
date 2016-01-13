import QtQuick 2.0
import adctl 1.0

AdCtl{
    adMobBannerEnabled:adMobBanner!==null
    adMobBannerVisible:adMobBannerEnabled ? adMobBanner.visible : false
    adMobBannerPosition:adMobBannerEnabled ? Qt.point(adMobBanner.targetRect.x,adMobBanner.targetRect.y) : Qt.point(0,0)

    adMobIinterstitialEnabled:adMobInterstitial!==null
    adMobIinterstitialVisible:adMobIinterstitialEnabled ? adMobInterstitial.visible : false

    startAdBannerEnabled:startAdBanner!==null
    startAdBannerVisible:startAdBannerEnabled ? startAdBanner.visible : false
    startAdBannerPosition:startAdBannerEnabled ? Qt.point(startAdBanner.targetRect.x,startAdBanner.targetRect.y) : Qt.point(0,0)
    startAdBannerSize:startAdBannerEnabled ? Qt.size(startAdBanner.targetRect.width,startAdBanner.targetRect.height) : Qt.size(0,0)

    property AdCtlBanner adMobBanner
    property AdCtlBanner adMobInterstitial
    property AdCtlBanner startAdBanner

    Component.onCompleted:{
        init()
    }
}

