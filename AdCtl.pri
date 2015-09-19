include(3rd/QtAdMob/QtAdMob/QtAdMob.pri)
include(3rd/qt-google-analytics/qt-google-analytics.pri)

QT += qml quick widgets

HEADERS += $$PWD/adctl.h
SOURCES += $$PWD/adctl.cpp

android {
  #ANDROID_PACKAGE_SOURCE_DIR = $$PWD/3rd/GATutorial-QML/android_data
  QT += androidextras gui-private
  OTHER_FILES += \
    $$PWD/platform/android/src/ru/forsk/AdCtl/AdCtlActivity.java
  #AdMob
  DISTFILES += $$PWD/3rd/QtAdMob/QtAdMob/platform/android/src/org/dreamdev/QtAdMob/QtAdMobActivity.java \
    $$PWD/3rd/GATutorial-QML/android_data/src/com/lasconic/QGoogleAnalytics.java

  copydata.commands += $(COPY_DIR) $$PWD/3rd/SDK-Android/lib/StartADLib-1.0.1.jar $$ANDROID_PACKAGE_SOURCE_DIR/libs/;
  copydata.commands += $(COPY_DIR) $$PWD/3rd/QtAdMob/QtAdMob/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;
  copydata.commands += $(COPY_DIR) $$PWD/platform/android/project.properties $$ANDROID_PACKAGE_SOURCE_DIR;
  copydata.commands += $(COPY_DIR) $$PWD/platform/android/src $$ANDROID_PACKAGE_SOURCE_DIR;
  copydata.CONFIG += no_link target_predeps
  first.depends = $(first) copydata
  first.CONFIG += no_link target_predeps
  export(first.depends)
  export(copydata.commands)
  QMAKE_EXTRA_TARGETS += first copydata
}

ios {
  QMAKE_LFLAGS += -framework SystemConfiguration -framework CoreData -framework AdSupport -ObjC
  LIBS += -L$$PWD/ios/libs -lGoogleAnalyticsServices -lAdIdAccess -lsqlite3.0
  OBJECTIVE_SOURCES += $$PWD/3rd/GATutorial-QML/ios/iosanalytics.mm
  QMAKE_CXXFLAGS += -fobjc-arc
  QMAKE_LFLAGS += -ObjC
  QT += gui_private
  OBJECTIVE_SOURCES += google/ios/iosanalytics.mm
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
    -framework CoreMedia \
    L$$PWD/google/ios/libs -lGoogleAnalyticsServices -lAdIdAccess -lsqlite3.0
}

RESOURCES += $$PWD/3rd/GATutorial-QML/qml.qrc

INCLUDEPATH += $$PWD/3rd/GATutorial-QML/ \
                $$PWD/3rd/QtAdMob/QtAdMob

