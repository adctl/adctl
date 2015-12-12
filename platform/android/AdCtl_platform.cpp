#include "AdCtl_platform.h"
#include <QString>

#include <android/api-level.h>
#include <QAndroidJniObject>
#include <qpa/qplatformnativeinterface.h>

AdCtl_platform::AdCtl_platform(){
    m_Activity=0;
#if (__ANDROID_API__ >= 9)
    QPlatformNativeInterface* interface = QApplication::platformNativeInterface();
    jobject activity = (jobject)interface->nativeResourceForIntegration("QtActivity");
    if (activity)
    {
        m_Activity = new QAndroidJniObject(activity);
    }
#endif
    if(!m_Activity)
        qFatal("Invalid android api level");
}

AdCtl_platform::~AdCtl_platform(){
    delete m_Activity;
}

void AdCtl_platform::initStartAd(){
    m_Activity->callMethod<void>("InitializeStartAdBanner");
}

void AdCtl_platform::setStartAdId(const QString& id){
    QAndroidJniObject param1 = QAndroidJniObject::fromString(m_StartAdId);
    m_Activity->callMethod<void>("SetStartAdId", "(Ljava/lang/String;)V", param1.object<jstring>());
}

void AdCtl_platform::setStartAdBannerSize(const int width, const int height){
    m_Activity->callMethod<void>("SetStartAdBannerSize", "(II)V", StartAdBannerSize.width(), StartAdBannerSize.height());
}

void AdCtl_platform::setStartAdBannerPosition(const int x, const int y){
    m_Activity->callMethod<void>("SetStartAdBannerPosition", "(II)V", position.x(), position.y());
}

int AdCtl_platform::startAdBannerHeight() const{
    return m_Activity->callMethod<int>("GetStartAdBannerHeight");
}

int AdCtl_platform::startAdBannerWidth() const{
    return m_Activity->callMethod<int>("GetStartAdBannerWidth");
}

int AdCtl_platform::startAdBannerX() const{
    return m_Activity->callMethod<int>("GetStartAdBannerX");
}

int AdCtl_platform::startAdBannerY() const{
    return m_Activity->callMethod<int>("GetStartAdBannerY");
}

void AdCtl_platform::showStartAd(){
    m_Activity->callMethod<void>("ShowStartAdBanner");
}

void AdCtl_platform::hideStartAd(){
    m_Activity->callMethod<void>("HideStartAdBanner");
}

float AdCtl_platform::getDensity() const{
    QAndroidJniObject qtActivity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resources = qtActivity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    int density = displayMetrics.getField<int>("densityDpi");
    return density;
}

int AdCtl_platform::adMobBannerX() const{
    return m_Activity->callMethod<int>("GetAdMobBannerX");
}

int AdCtl_platform::adMobBannerY() const{
    return m_Activity->callMethod<int>("GetAdMobBannerY");
}

bool AdCtl_platform::isGPGSSignedIn() const{
    return m_Activity->callMethod<jboolean>("getSignedInGPGS");
}

void AdCtl_platform::signInGPGS(){
    m_Activity->callMethod<void>("loginGPGS");
}

void AdCtl_platform::submitScoreGPGS(const QString& leaderBoardId, int score){
    QAndroidJniObject param1 = QAndroidJniObject::fromString(leaderBoardId);
    m_Activity->callMethod<void>("submitScoreGPGS", "(Ljava/lang/String;I)V", param1.object<jstring>(), score);
}

void AdCtl_platform::unlockAchievementGPGS(const QString& achievementId){
    QAndroidJniObject param1 = QAndroidJniObject::fromString(achievementId);
    m_Activity->callMethod<void>("unlockAchievementGPGS", "(Ljava/lang/String;)V", param1.object<jstring>());
}

void AdCtl_platform::showLeaderboardGPGS(){
    m_Activity->callMethod<void>("getLeaderboardGPGS");
}

void AdCtl_platform::showAchievementsGPGS(){
    m_Activity->callMethod<void>("getAchievementsGPGS");
}
