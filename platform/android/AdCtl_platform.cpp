#include "AdCtl_platform.h"
#include <QString>
#include <QDebug>
#include <QGuiApplication>
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <android/api-level.h>
#include <qpa/qplatformnativeinterface.h>
#include "StateManager.h"
#include "utils.h"

AdCtl_platform::AdCtl_platform(){
    m_Activity=0;
#if (__ANDROID_API__ >= 9)
    QPlatformNativeInterface* interface = QGuiApplication::platformNativeInterface();
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
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

void AdCtl_platform::setStartAdId(const QString& id){
    QAndroidJniObject param1 = QAndroidJniObject::fromString(id);
    m_Activity->callMethod<void>("SetStartAdId", "(Ljava/lang/String;)V", param1.object<jstring>());
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

void AdCtl_platform::setStartAdBannerSize(const int width, const int height){
    m_Activity->callMethod<void>("SetStartAdBannerSize", "(II)V", width, height);
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

void AdCtl_platform::setStartAdBannerPosition(const int x, const int y){
    m_Activity->callMethod<void>("SetStartAdBannerPosition", "(II)V", x, y);
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

int AdCtl_platform::startAdBannerHeight() const{
    return m_Activity->callMethod<int>("GetStartAdBannerHeight");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

int AdCtl_platform::startAdBannerWidth() const{
    return m_Activity->callMethod<int>("GetStartAdBannerWidth");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

int AdCtl_platform::startAdBannerX() const{
    return m_Activity->callMethod<int>("GetStartAdBannerX");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

int AdCtl_platform::startAdBannerY() const{
    return m_Activity->callMethod<int>("GetStartAdBannerY");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

void AdCtl_platform::showStartAd(){
    m_Activity->callMethod<void>("ShowStartAdBanner");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

void AdCtl_platform::hideStartAd(){
    m_Activity->callMethod<void>("HideStartAdBanner");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}

float AdCtl_platform::getDensity() const{
    QAndroidJniObject qtActivity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resources = qtActivity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    int density = displayMetrics.getField<int>("densityDpi");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
    return density;
}

int AdCtl_platform::adMobBannerX() const{
    int xval = m_Activity->callMethod<int>("GetAdMobBannerX");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
    return xval;
}

int AdCtl_platform::adMobBannerY() const{
    int yval = m_Activity->callMethod<int>("GetAdMobBannerY");
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
    return yval;
}

bool AdCtl_platform::isGPGSSignedIn() const{
    return StateManager::GetGameServices()->IsAuthorized();
}

void AdCtl_platform::signInGPGS(){
    StateManager::BeginUserInitiatedSignIn();
}

void AdCtl_platform::getLeaderBoardScore(const QString &leaderboardId){

    gpg::LeaderboardManager::FetchScoreSummaryCallback
            callback = [&](const gpg::LeaderboardManager::FetchScoreSummaryResponse& response)
    {
        uint64_t score;
        QString lID;
        int status = static_cast<int>(response.status);
        if(response.status == gpg::ResponseStatus::VALID ||
                response.status == gpg::ResponseStatus::VALID_BUT_STALE){
            score = response.data.CurrentPlayerScore().Value();
            lID = QString(response.data.LeaderboardId().c_str());
        }
        if(m_adctl_obj) {

            if(Util::javaScriptFuncExists(m_adctl_obj,"scoreGPG")){
                QMetaObject::invokeMethod(m_adctl_obj, "scoreGPG", Qt::DirectConnection,
                           Q_ARG(QVariant, QVariant(lID)),
                           Q_ARG(QVariant, QVariant(score)),
                           Q_ARG(QVariant, QVariant(status)));
            }

        }
    };
    StateManager::GetGameServices()->Leaderboards().FetchScoreSummary(leaderboardId.toStdString(),gpg::LeaderboardTimeSpan::ALL_TIME,
                                                                      gpg::LeaderboardCollection::PUBLIC,callback);
}

void AdCtl_platform::submitScoreGPGS(const QString& leaderBoardId, int score){
    StateManager::SubmitHighScore(leaderBoardId.toStdString().c_str(),score);
}

void AdCtl_platform::unlockAchievementGPGS(const QString& achievementId){
    StateManager::UnlockAchievement(achievementId.toStdString().c_str());
}

void AdCtl_platform::showLeaderboard(const QString &leaderboardId){
    StateManager::GetGameServices()->Leaderboards().ShowUI(leaderboardId.toStdString(),[](gpg::UIStatus const &){});
}

void AdCtl_platform::showLeaderboardGPGS(){
    StateManager::ShowLeaderboards();
}

void AdCtl_platform::showAchievementsGPGS(){
    StateManager::GetGameServices()->Achievements().ShowAllUI([](const gpg::UIStatus& status){

    });
}

void AdCtl_platform::shareImage(QString path) {
    qDebug() << "Share android_platform path ="<<path;
    QAndroidJniObject jContent = QAndroidJniObject::fromString(path);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>(
        "ru/forsk/adctl/AdCtlUtil",
        "shareIntent",
        "(Ljava/lang/String;"
              "Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
        jContent.object<jstring>(),
        activity.object<jobject>()
   );
    // Checking exceptions
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Printing exception message
        env->ExceptionDescribe();

        // Clearing exceptions
        env->ExceptionClear();
    }
}
