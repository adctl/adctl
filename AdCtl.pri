ADMOB_PATH = $$PWD/3rd/QtAdMob/QtAdMob
GOOGLE_ANALITICS_PATH = $$PWD/3rd/qt-google-analytics

include($$ADMOB_PATH/QtAdMob.pri)
include($$GOOGLE_ANALITICS_PATH/qt-google-analytics.pri)

QT += qml quick widgets

HEADERS += $$PWD/adctl.h
SOURCES += $$PWD/adctl.cpp

INCLUDEPATH += $$ADMOB_PATH
INCLUDEPATH += $$GOOGLE_ANALITICS_PATH

OTHER_FILES += README.md

android
{
    android:QT += androidextras gui-private
    android:DISTFILES += $$ANDROID_PACKAGE_SOURCE_DIR/src/org/dreamdev/QtAdMob/QtAdMobActivity.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/BaseGameUtils.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/GameHelper.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/GameHelperUtils.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/src/ru/forsk/AdCtl/AdCtlActivity.java \
                         $$ANDROID_PACKAGE_SOURCE_DIR/res/values/strings.xml \
                         $$ANDROID_PACKAGE_SOURCE_DIR/AndroidManifest.xml \
                         $$ANDROID_PACKAGE_SOURCE_DIR/project.properties \
                         $$ANDROID_PACKAGE_SOURCE_DIR/libs/*

    # make required directories
    DIRS = $$ANDROID_PACKAGE_SOURCE_DIR/libs
    createDirs.commands = $(MKDIR) $$DIRS
    first.depends += createDirs
    android:QMAKE_EXTRA_TARGETS += createDirs

    #copy platform specific data

    #AdMob
    !exists ($$ANDROID_PACKAGE_SOURCE_DIR/google-play-services_lib) {
        copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/QtAdMob/platform/android/google-play-services_lib $$ANDROID_PACKAGE_SOURCE_DIR;
        copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/QtAdMob/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;
    }

    #StartAd
    !exists ($$ANDROID_PACKAGE_SOURCE_DIR/libs/StartADLib-1.0.1.jar) {
        copydata.commands += $(COPY_DIR) $$PWD/3rd/SDK-Android/lib/StartADLib-1.0.1.jar $$ANDROID_PACKAGE_SOURCE_DIR/libs/;
    }

    #PlayServices
    !exists ($$ANDROID_PACKAGE_SOURCE_DIR/libs/android-support-v4.jar) {
        copydata.commands += $(COPY_DIR) $$PWD/3rd/android-support-library-archive/v4/android-support-v4.jar $$ANDROID_PACKAGE_SOURCE_DIR/libs/;
    }

    #AdCtl
    !exists ($$ANDROID_PACKAGE_SOURCE_DIR/project.properties) {
        copydata.commands += $(COPY_DIR) $$PWD/platform/android/project.properties $$ANDROID_PACKAGE_SOURCE_DIR;
    }
    !exists ($$ANDROID_PACKAGE_SOURCE_DIR/src/ru/forsk) {
        copydata.commands += $(COPY_DIR) $$PWD/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;
    }
    copydata.CONFIG += no_link target_predeps

    #Run data copy and dirs creation
    first.depends += $(first) copydata
    first.CONFIG += no_link target_predeps
    export(first.depends)
    export(copydata.commands)
    android:QMAKE_EXTRA_TARGETS += first copydata
}

ios {
  ios:QMAKE_CXXFLAGS += -fobjc-arc
  ios:QMAKE_LFLAGS += -ObjC
  ios:QT += gui_private
  ios:LIBS += -F $$PWD/QtAdMob/platform/ios/GoogleMobileAds \
    -framework GoogleMobileAds \
    -framework AVFoundation \
    -framework AudioToolbox \
    -framework CoreTelephony \
    -framework MessageUI \
    -framework SystemConfiguration \
    -framework CoreGraphics \
    -framework AdSupport \
    -framework StoreKit \
    -framework EventKit \
    -framework EventKitUI \
    -framework CoreMedia
}
