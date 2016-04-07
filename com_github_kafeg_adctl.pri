ADMOB_PATH = $$PWD/3rd/QtAdMob
GOOGLE_ANALITICS_PATH = $$PWD/3rd/qt-google-analytics

include($$ADMOB_PATH/QtAdMob.pri)
include($$GOOGLE_ANALITICS_PATH/qt-google-analytics.pri)

QT += qml quick widgets

HEADERS += $$PWD/adctl.h $$PWD/platform/AdCtl_platform_interface.h \
    $$PWD/utils.h
SOURCES += $$PWD/adctl.cpp $$PWD/platform/AdCtl_platform_interface.cpp \
    $$PWD/utils.cpp

RESOURCES += $$PWD/adctl.qrc

INCLUDEPATH += $$PWD
INCLUDEPATH += $$ADMOB_PATH
INCLUDEPATH += $$GOOGLE_ANALITICS_PATH

OTHER_FILES += README.md

#In case you want to implement another load callback for JNI_ONLOAD
#For more info AndroidMain.cpp
#DEFINES += ANDROIDMAIN_CUSTOM_LOADCALLBACK

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
ANDROID_SOURCES = $$PWD"/platform/android"

ios|android{
    ## GPGS C++ Implementation
    HEADERS += \
        $$PWD/platform/gpgscpp/AndroidMain.h \
        $$PWD/platform/gpgscpp/StateManager.h \
        $$PWD/platform/gpgscpp/SelfDetachingJNIEnv.h

    SOURCES += \
        $$PWD/platform/gpgscpp/AndroidMain.cpp \
        $$PWD/platform/gpgscpp/StateManager.cpp \
        $$PWD/platform/gpgscpp/SelfDetachingJNIEnv.cpp \
        $$PWD/platform/gpgscpp/AndroidJNI.cpp

        INCLUDEPATH += $$PWD/platform/gpgscpp/

        android:{
            INCLUDEPATH +=  $$PWD/3rd/gpg-cpp-sdk/android/include
            INCLUDEPATH +=  $$PWD/3rd/gpg-cpp-sdk/android/include/gpg

            equals(ANDROID_TARGET_ARCH,armeabi-v7a) {
                LIBS += -L$$PWD/3rd/gpg-cpp-sdk/android/lib/gnustl/armeabi-v7a -lgpg
            }
            equals(ANDROID_TARGET_ARCH, x86) {
                LIBS += -L$$PWD/3rd/gpg-cpp-sdk/android/lib/gnustl/x86 -lgpg
            }
            equals(ANDROID_TARGET_ARCH,armeabi) {}

        }
        ios:{
            LIBS+= $$PWD/3rd/gpg-cpp-sdk/ios/gpg.bundle
            LIBS+= -F$$PWD/3rd/gpg-cpp-sdk/ios -framework gpg
            LIBS+= $$PWD/3rd/google_signin_ios/GoogleSignIn.bundle
            LIBS+= -F$$PWD/3rd/google_signin_ios/ -framework GoogleSignIn
            LIBS+= $$PWD/3rd/google_signin_ios/GooglePlus.bundle
            LIBS+= -F$$PWD/3rd/google_signin_ios/ -framework GooglePlus
            LIBS+= -F$$PWD/3rd/google_signin_ios/ -framework GoogleOpenSource
            INCLUDEPATH += $$PWD/3rd/SDK-iOS/libs/
            LIBS+= $$PWD/3rd/SDK-iOS/libs/libSADView.a


            gpgFiles.files = $$PWD/3rd/gpg-cpp-sdk/ios/gpg.bundle \
                             $$PWD/3rd/google_signin_ios/GoogleSignIn.bundle \
                             $$PWD/3rd/google_signin_ios/GooglePlus.bundle
            gpgFiles.path = .
            QMAKE_BUNDLE_DATA += gpgFiles


            LIBS+= \
                -framework AddressBook \
                -framework AssetsLibrary \
                -framework CoreData \
                -framework CoreLocation \
                -framework CoreMotion \
                -framework CoreText \
                -framework Foundation \
                -framework MediaPlayer \
                -framework QuartzCore \
                -framework Security \
                -framework SystemConfiguration \
                -framework CoreGraphics
        }
}

android {
    INCLUDEPATH += $$PWD/platform/android
    HEADERS += $$PWD/platform/android/AdCtl_platform.h
    SOURCES += $$PWD/platform/android/AdCtl_platform.cpp

    android:QT += androidextras gui-private
    android:DISTFILES += $$ANDROID_SOURCES/src/org/dreamdev/QtAdMob/QtAdMobActivity.java \
                         $$ANDROID_SOURCES/src/com/google/example/games/basegameutils/BaseGameUtils.java \
                         $$ANDROID_SOURCES/src/com/google/example/games/basegameutils/GameHelper.java \
                         $$ANDROID_SOURCES/src/com/google/example/games/basegameutils/GameHelperUtils.java \
                         $$ANDROID_SOURCES/src/ru/forsk/AdCtl/AdCtlActivity.java \
                         $$ANDROID_SOURCES/res/values/strings.xml \
                         $$ANDROID_SOURCES/project.properties \
                         $$ANDROID_SOURCES/libs/*
    # As maybe you thought, when you create a project most of time what is needed is only
    # the library, but Adctl tries to make you use their AdCtlActivity as default. I want to handle
    # from my own class this is a workaround for compiling java sources from different paths.
    # consider putting the
    #http://imaginativethinking.ca/what-the-heck-how-do-i-share-java-code-between-qt-android-projects/
    adctlJavaFilesPath = $$system_path($$ANDROID_SOURCES/src/)
    androidBuildOutputDir = $$system_path( $$_PRO_FILE_PWD_/android/ )
    copyAdctlJavaFiles.commands = $(COPY_DIR) $${adctlJavaFilesPath} $${androidBuildOutputDir}

    adMobJavaFilesPath = $$system_path($$PWD/3rd/QtAdMob/platform/android/src/)
    copyAdMobJavaFiles.commands = $(COPY_DIR) $${adMobJavaFilesPath} test/$${androidBuildOutputDir}

    admobResFilePath  = $$system_path($$PWD/3rd/QtAdMob/platform/android/google-play-services_lib/src/main/res/)
    #copyAdMobResFiles.commands = $(COPY_DIR) $${admobResFilePath} $${androidBuildOutputDir}

    first.depends = $(first) copyAdctlJavaFiles copyAdMobJavaFiles copyAdMobResFiles
    export(first.depends)
    export(copyAdctlJavaFiles.commands)
    export(copyAdMobJavaFiles.commands)
    export(copyAdMobResFiles.commands)
    QMAKE_EXTRA_TARGETS += first copyAdctlJavaFiles copyAdMobJavaFiles copyAdMobResFiles

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
    support_lib.files = $$PWD/3rd/android-support-library-archive/v4/android-support-v4.jar
    support_lib.path = /libs


    #AdCtl
    adctl1.files = $$PWD/platform/android/project.properties
    adctl1.path = /

    adctl2.files = $$PWD/platform/android/src/*
    adctl2.path = /src

    INSTALLS += admob1 admob2 startad1 adctl1 adctl2
#    INSTALLS += support_lib #removed because using gradle things get autoincluded
}

DISTFILES += \
    $$PWD/platform/android/src/ru/forsk/AdCtl/AdCtlUtil.java




