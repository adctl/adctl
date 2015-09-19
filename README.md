# adctl
Qt for Google Analytics, Google AdMob, Google Play services (auth and achievements) and StartAd.mobi framework

Howto integrate library to project:

1. Add submodule to your project:

cd $$PROJECT_ROOT/mobile
git submodule add https://github.com/kafeg/adctl.git
git submodule update --init --recursive

2. include .pri in your project:

#AdCtl: Google Analytics, AdMob, StartAD.mobi
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/mobile/android
include(mobile/adctl/AdCtl.pri)

3. Copy file mobile/adctl/3rd/GATutorial-QML/android_data/res/xml/analytics.xml to $$ANDROID_PACKAGE_SOURCE_DIR/res/xml/

4. Replace in $$ANDROID_PACKAGE_SOURCE_DIR/res/xml/analytics.xml 'YOUR TRACKING ID' to your Google Analytics tracking id.

5. Run project. On every run into your $$ANDROID_PACKAGE_SOURCE_DIR/ directory will be copied files from 3rd libraries (see AdCtl.pri copydata for details)

Howto interact with library from C++:

TODO

Howto interact with library from Qml:

TODO
