ADMOB_PATH = $$PWD/3rd/QtAdMob
GOOGLE_ANALITICS_PATH = $$PWD/3rd/qt-google-analytics

include($$ADMOB_PATH/QtAdMob.pri)
include($$GOOGLE_ANALITICS_PATH/qt-google-analytics.pri)

QT += qml quick widgets

HEADERS += $$PWD/adctl.h $$PWD/platform/AdCtl_platform_interface.h
SOURCES += $$PWD/adctl.cpp $$PWD/platform/AdCtl_platform_interface.cpp
RESOURCES += $$PWD/adctl.qrc

RESOURCES += \
    $$PWD/com_github_kafeg_adctl.qrc

INCLUDEPATH += $$ADMOB_PATH
INCLUDEPATH += $$GOOGLE_ANALITICS_PATH
INCLUDEPATH += $$PWD/

OTHER_FILES += README.md

!ios {
    !android {
        INCLUDEPATH += $$PWD/platform/other
        HEADERS += $$PWD/platform/other/AdCtl_platform.h
    }
}

ios {
    INCLUDEPATH += $$PWD/platform/ios
    HEADERS += $$PWD/platform/ios/AdCtl_platform.h
    OBJECTIVE_SOURCES += $$PWD/platform/ios/AdCtl_platform.mm
    DIRS = $$IOS_PACKAGE_DIR/libs
}

android {
    INCLUDEPATH += $$PWD/platform/android
    HEADERS += $$PWD/platform/android/AdCtl_platform.h
    SOURCES += $$PWD/platform/android/AdCtl_platform.cpp
    android:QT += androidextras gui-private
    android:FILES += $$ANDROID_PACKAGE_SOURCE_DIR/src/org/dreamdev/QtAdMob/QtAdMobActivity.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/BaseGameUtils.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/GameHelper.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/GameHelperUtils.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/ru/forsk/AdCtl/AdCtlActivity.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/res/values/strings.xml \
                         $$ANDROID_PACKAGE_SOURCE_DIR/project.properties \
                         $$ANDROID_PACKAGE_SOURCE_DIR/libs/*

    #SRC direcotory copy rules (Work in Windows and Linux. If this code not work in your project - please check setting or contact author)
    #=========================

    #AdMob
    admob1.files = $$PWD/3rd/QtAdMob/platform/android/google-play-services_lib/*
    admob1.path = /google-play-services_lib

    admob2.files = $$PWD/3rd/QtAdMob/platform/android/src/*
    admob2.path = /src

    #StartAd
    startad1.files = $$PWD/3rd/SDK-Android/lib/StartADLib-1.0.1.jar
    startad1.path = /libs


    #PlayServices
    playservice1.files = $$PWD/3rd/android-support-library-archive/v4/android-support-v4.jar
    playservice1.path = /libs


    #AdCtl
    adctl1.files = $$PWD/platform/android/project.properties
    adctl1.path = /

    adctl2.files = $$PWD/platform/android/src/*
    adctl2.path = /src

    INSTALLS += admob1 admob2 startad1 playservice1 adctl1 adctl2
}
