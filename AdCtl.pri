include(3rd/QtAdMob/QtAdMob/QtAdMob.pri)
include(3rd/qt-google-analytics/qt-google-analytics.pri)

QT += qml quick widgets

HEADERS += $$PWD/adctl.h
SOURCES += $$PWD/adctl.cpp

INCLUDEPATH += $$PWD/3rd/QtAdMob/QtAdMob

OTHER_FILES += README.md

android {
  #ANDROID_PACKAGE_SOURCE_DIR = $$PWD/3rd/GATutorial-QML/android_data
  QT += androidextras gui-private
  OTHER_FILES += $$PWD/platform/android/src/ru/forsk/AdCtl/AdCtlActivity.java \
    $$PWD/platform/android/src/com/google/example/games/basegameutils/* \
    $$PWD/README.md
  #AdMob
  DISTFILES += $$PWD/3rd/QtAdMob/QtAdMob/platform/android/src/org/dreamdev/QtAdMob/QtAdMobActivity.java

#  #Google Play Services
#  DISTFILES += $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/java/com/google/example/games/basegameutils/BaseGameUtils.java \
#    $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/java/com/google/example/games/basegameutils/GameHelper.java \
#    $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/java/com/google/example/games/basegameutils/GameHelperUtils.java


  #make dirs
  DIRS = $$ANDROID_PACKAGE_SOURCE_DIR/libs
#    $$ANDROID_PACKAGE_SOURCE_DIR/src \
#    $$ANDROID_PACKAGE_SOURCE_DIR/src/com \
#    $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google \
#    $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example \
#    $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games \
#    $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils
  for(DIR, DIRS) {
    mkcommands += $$DIR
  }
  createDirs.commands = $(MKDIR) $$mkcommands
  first.depends += createDirs
  QMAKE_EXTRA_TARGETS += createDirs

  #copy platform specific data

  #AdMob
  copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/QtAdMob/platform/android/google-play-services_lib $$ANDROID_PACKAGE_SOURCE_DIR;
  copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/QtAdMob/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;

  #StartAd
  copydata.commands += $(COPY_DIR) $$PWD/3rd/SDK-Android/lib/StartADLib-1.0.1.jar $$ANDROID_PACKAGE_SOURCE_DIR/libs/;

  #PlayServices
  copydata.commands += $(COPY_DIR) $$PWD/3rd/android-support-library-archive/v4/android-support-v4.jar $$ANDROID_PACKAGE_SOURCE_DIR/libs/;

#  we replace files but in original not found R
#  copydata.commands += $(COPY_DIR) $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/java/com/google/example/games/basegameutils/BaseGameUtils.java $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/;
#  copydata.commands += $(COPY_DIR) $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/java/com/google/example/games/basegameutils/GameHelper.java $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/;
#  copydata.commands += $(COPY_DIR) $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/java/com/google/example/games/basegameutils/GameHelperUtils.java $$ANDROID_PACKAGE_SOURCE_DIR/src/com/google/example/games/basegameutils/;
  copydata.commands += $(COPY_DIR) $$PWD/3rd/android-basic-samples/BasicSamples/libraries/BaseGameUtils/src/main/res/values $$ANDROID_PACKAGE_SOURCE_DIR/res;

  #AdCtl
  copydata.commands += $(COPY_DIR) $$PWD/platform/android/project.properties $$ANDROID_PACKAGE_SOURCE_DIR;
  copydata.commands += $(COPY_DIR) $$PWD/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;
  copydata.CONFIG += no_link target_predeps

  #Run data copy and dirs creation
  first.depends += $(first) copydata
  first.CONFIG += no_link target_predeps
  export(first.depends)
  export(copydata.commands)
  QMAKE_EXTRA_TARGETS += first copydata

}

ios {
  QMAKE_CXXFLAGS += -fobjc-arc
  QMAKE_LFLAGS += -ObjC
  QT += gui_private
  LIBS += -F $$PWD/QtAdMob/platform/ios/GoogleMobileAds \
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

