ADMOB_PATH = $$PWD/3rd/QtAdMob
GOOGLE_ANALITICS_PATH = $$PWD/3rd/qt-google-analytics

include($$ADMOB_PATH/QtAdMob.pri)
include($$GOOGLE_ANALITICS_PATH/qt-google-analytics.pri)

QT += qml quick widgets

HEADERS += $$PWD/adctl.h $$PWD/platform/AdCtl_platform_interface.h
SOURCES += $$PWD/adctl.cpp $$PWD/platform/AdCtl_platform_interface.cpp
RESOURCES += $$PWD/adctl.qrc

INCLUDEPATH += $$ADMOB_PATH
INCLUDEPATH += $$GOOGLE_ANALITICS_PATH

OTHER_FILES += README.md

system(ver){
#windows build environment
    COMMAND_SEPARATOR = ' &'
} else {
    COMMAND_SEPARATOR = ';'
}

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

    DIRS = $$ANDROID_PACKAGE_SOURCE_DIR/libs

    COPYDIR_SUFFIX =

    # make required directories
    !exists ($$DIRS) {
        copydata.commands = $(MKDIR) $$shell_path($$DIRS);
    }

    #SRC direcotory copy rules (Work in Windows and Linux. If this code not work in your project - please check setting or contact author)
    #=========================

    #AdMob
    !exists ($$ANDROID_PACKAGE_SOURCE_DIR/google-play-services_lib) {
        copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/platform/android/google-play-services_lib $$ANDROID_PACKAGE_SOURCE_DIR;
        copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;
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
}

android|ios {
    copydata.CONFIG += no_link target_predeps
    copydata.commands = $$replace(copydata.commands,;,$$COMMAND_SEPARATOR)

    #Run data copy and dirs creation
    first.depends += $(first) copydata
    first.CONFIG += no_link target_predeps
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}
