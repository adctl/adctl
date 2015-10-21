#include "adctl.h"
#include "QtAdMobBanner.h"
#include "QtAdMobInterstitial.h"
#include <QtQml>
#include <QDebug>
#include <QTimer>
#include "ganalytics.h"
#include <QScreen>
#include <QApplication>

#if (__ANDROID_API__ >= 9)

#include <android/api-level.h>
#include <QAndroidJniObject>
#include <QPA/QPlatformNativeInterface.h>


#endif

AdCtl::AdCtl(QObject *parent) : QObject(parent)
{
    m_AdMobBanner = CreateQtAdMobBanner();
    m_AdMobInterstitial = CreateQtAdMobInterstitial();

#if (__ANDROID_API__ >= 9)

    QPlatformNativeInterface* interface = QApplication::platformNativeInterface();
    jobject activity = (jobject)interface->nativeResourceForIntegration("QtActivity");
    if (activity)
    {
        m_Activity = new QAndroidJniObject(activity);
    }

#endif

    //mm and dp
#ifdef Q_OS_ANDROID
    //  BUG with dpi on some androids: https://bugreports.qt-project.org/browse/QTBUG-35701
    //  Workaround:
    QAndroidJniObject qtActivity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resources = qtActivity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    int density = displayMetrics.getField<int>("densityDpi");
#else
    QScreen *screen = qApp->primaryScreen();
    float density = screen->physicalDotsPerInch();
#endif
    m_mm = density / 25.4;
    m_pt =  1;

    //engine.rootContext()->setContextProperty("adctlMM",density / 25.4);
    //engine.rootContext()->setContextProperty("adctlPT", 1);

    double scale = density < 180 ? 1 :
                   density < 270 ? 1.5 :
                   density < 360 ? 2 : 3;

    m_dp = scale;
    //engine.rootContext()->setContextProperty("adctlDP", scale);
}

AdCtl::~AdCtl()
{
    delete m_AdMobBanner;
    delete m_AdMobInterstitial;
    delete m_Activity;
}

void AdCtl::init()
{
    if (m_AdInitialized) { qDebug() << "AdMob alredy initialized!"; return; }

#if (__ANDROID_API__ >= 9)

    QAndroidJniObject param1 = QAndroidJniObject::fromString(m_StartAdId);
    m_Activity->callMethod<void>("SetStartAdId", "(Ljava/lang/String;)V", param1.object<jstring>());

#endif

    if (m_AdMobBannerEnabled) {
        m_AdMobBanner->Initialize();
        m_AdMobBanner->SetSize(IQtAdMobBanner::Banner);
        m_AdMobBanner->SetUnitId(m_AdMobId);
        m_AdMobBanner->AddTestDevice("DCE0DB737EC089D97AB4EFCBA2F9B322");
        m_AdMobBanner->Show();
    }

    if (m_AdMobInterstitialEnabled) {
        m_AdMobInterstitial->LoadWithUnitId(m_AdMobId);
        m_AdMobInterstitial->AddTestDevice("DCE0DB737EC089D97AB4EFCBA2F9B322");
    }

    if (m_StartAdBannerEnabled) {
#if (__ANDROID_API__ >= 9)
        m_Activity->callMethod<void>("InitializeStartAdBanner");
#endif
    }

    if (m_GAnalyticsEnabled) {
        gAnalytics = new GAnalytics(m_GAnalyticsId);
    }

    adctlTimer = new QTimer(this);
    connect(adctlTimer, SIGNAL(timeout()), this, SLOT(adctlTimerSlot()));
    adctlTimer->start(500);

    m_AdInitialized = true;
}

float AdCtl::dp()
{
    return m_dp;
}
float AdCtl::mm()
{
    return m_mm;
}
float AdCtl::pt()
{
    return m_pt;
}

void AdCtl::adctlTimerSlot()
{
    //signals AdMob
    if (cacheAdMobBannerHeight != AdMobBannerHeight()) {
        cacheAdMobBannerHeight = AdMobBannerHeight();
        emit adMobBannerHeightChanged(AdMobBannerHeight());
    }
    if (cacheAdMobBannerWidth != AdMobBannerWidth()) {
        cacheAdMobBannerWidth = AdMobBannerWidth();
        emit adMobBannerWidthChanged(AdMobBannerWidth());
    }

    //signals StartAd
    if (cacheStartAdBannerHeight != StartAdBannerHeight()) {
        cacheStartAdBannerHeight = StartAdBannerHeight();
        emit startAdBannerHeightChanged(StartAdBannerHeight());
    }
    if (cacheStartAdBannerWidth != StartAdBannerWidth()) {
        cacheStartAdBannerWidth = StartAdBannerWidth();
        emit startAdBannerWidthChanged(StartAdBannerWidth());
    }

    //qDebug() << "Banners state" << AdMobBannerWidth() << StartAdBannerWidth()
    //         << adctlTimer->interval() << m_isStartAdBannerShowed << m_isAdMobBannerShowed
    //         << StartAdBannerHeight() << m_AdInitialized << m_StartAdBannerEnabled;

    //signal for firsttime reposition of banners
    if ((AdMobBannerWidth() > 0) && !m_AdMobWidthAlredyGreatThanZero) {
        m_isAdMobBannerShowed = true;
        m_AdMobWidthAlredyGreatThanZero = true;
        emit adMobBannerShowed();
    }

    if ((StartAdBannerWidth() > 0) && !m_StartAdWidthAlredyGreatThanZero) {
        m_isStartAdBannerShowed = true;
        m_StartAdWidthAlredyGreatThanZero = true;
        emit startAdBannerShowed();
    }

    if ((m_AdMobWidthAlredyGreatThanZero || !m_AdMobBannerEnabled)
            && (m_StartAdWidthAlredyGreatThanZero || !m_StartAdBannerEnabled)) {
        adctlTimer->stop();
        adctlTimer->start(2000);
    }
}

//AdMob banner enabled
bool AdCtl::AdMobBannerEnabled() const
{
    return m_AdMobBannerEnabled;
}

void AdCtl::setAdMobBannerEnabled(const bool AdMobBannerEnabled)
{
    m_AdMobBannerEnabled = AdMobBannerEnabled;
}

//AdMob interstitial enabled
bool AdCtl::AdMobIinterstitialEnabled() const
{
    return m_AdMobInterstitialEnabled;
}

void AdCtl::setAdMobIinterstitialEnabled(bool AdMobIinterstitialEnabled)
{
    m_AdMobInterstitialEnabled = AdMobIinterstitialEnabled;
}

//StartAd banner enabled
bool AdCtl::StartAdBannerEnabled() const
{
    return m_StartAdBannerEnabled;
}

void AdCtl::setStartAdBannerEnabled(bool StartAdBannerEnabled)
{
    m_StartAdBannerEnabled = StartAdBannerEnabled;
}

//GAnalytics enabled
bool AdCtl::GAnalyticsEnabled() const
{
    return m_GAnalyticsEnabled;
}

void AdCtl::setGAnalyticsEnabled(bool GAnalyticsEnabled)
{
    m_GAnalyticsEnabled = GAnalyticsEnabled;
}

//AdMob width and height
int AdCtl::AdMobBannerWidth() const
{
    if (!m_AdInitialized  || !m_AdMobBannerEnabled) { return 0; }
    return m_AdMobBanner->GetSizeInPixels().width();
}

int AdCtl::AdMobBannerHeight() const
{
    if (!m_AdInitialized || !m_AdMobBannerEnabled) { return 0; }
    return m_AdMobBanner->GetSizeInPixels().height();
}

//StartAd width and height
int AdCtl::StartAdBannerWidth() const
{
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return 0; }
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<int>("GetStartAdBannerWidth");
#endif
}

int AdCtl::StartAdBannerHeight() const
{
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return 0; }
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<int>("GetStartAdBannerHeight");
#endif
}

//AdMob banner position
QPoint AdCtl::AdMobBannerPosition() const
{
    return m_AdMobBannerPosition;
}

void AdCtl::setAdMobBannerPosition(const QPoint position)
{
    m_AdMobBannerPosition = position;
    if (!m_AdInitialized || !m_AdMobBannerEnabled) { return; }
    m_AdMobBanner->SetPosition(m_AdMobBannerPosition);
}

//StartAd banner position
QPoint AdCtl::StartAdBannerPosition() const
{
    return m_StartAdBannerPosition;
}

void AdCtl::setStartAdBannerSize(const QSize StartAdBannerSize)
{
    m_StartAdBannerSize = StartAdBannerSize;
    qDebug() << "StartAdBannerSize C++" << m_StartAdBannerSize;
    //if (!m_AdInitialized || !m_StartAdBannerEnabled) { return; }
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("SetStartAdBannerSize", "(II)V", StartAdBannerSize.width(), StartAdBannerSize.height());
#endif
}

QSize AdCtl::StartAdBannerSize() const
{
    return m_StartAdBannerSize;
}

int AdCtl::adMobBannerRealX()
{
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<int>("GetAdMobBannerX");
#endif
}

int AdCtl::adMobBannerRealY()
{
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<int>("GetAdMobBannerY");
#endif
}

int AdCtl::startAdBannerRealX()
{
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<int>("GetStartAdBannerX");
#endif
}

int AdCtl::startAdBannerRealY()
{
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<int>("GetStartAdBannerY");
#endif
}

void AdCtl::setStartAdBannerPosition(const QPoint position)
{
    m_StartAdBannerPosition = position;
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return; }
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("SetStartAdBannerPosition", "(II)V", position.x(), position.y());
#endif
}

//set ids
void AdCtl::setAdMobId(const QString &AdMobId)
{
    if (!m_AdMobId.isEmpty()) { qDebug() << "AdMob ID alredy set!"; return; }
    m_AdMobId = AdMobId;
}

void AdCtl::setStartAdId(const QString &StartAdId)
{
    if (!m_StartAdId.isEmpty()) { qDebug() << "StartAd ID alredy set!"; return; }
    m_StartAdId = StartAdId;
}

void AdCtl::setGAnalyticsId(const QString &GAnalyticsId)
{
    if (!m_GAnalyticsId.isEmpty()) { qDebug() << "GAnalytics ID alredy set!"; return; }
    m_GAnalyticsId = GAnalyticsId;
}

void AdCtl::showAdMobBanner()
{
    m_AdMobBanner->Show();
}

void AdCtl::hideAdMobBanner()
{
    m_AdMobBanner->Hide();
}

void AdCtl::showStartAdBanner()
{
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("ShowStartAdBanner");
#endif
}

void AdCtl::hideStartAdBanner()
{
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("HideStartAdBanner");
#endif
}

void AdCtl::sendGaAppView(const QString &screenName)
{
    if (m_GAnalyticsEnabled) {
        gAnalytics->sendAppView(screenName);
    }
}

void AdCtl::sendGaEvent(const QString &category, const QString &action, const QString &label, const QVariant &value)
{
    if (m_GAnalyticsEnabled) {
        gAnalytics->sendEvent(category, action, label, value);
    }
}

void AdCtl::endGaSession()
{
    if (m_GAnalyticsEnabled) {
        gAnalytics->endSession();
    }
}

bool AdCtl::isGPGSSignedIn() const
{
#if (__ANDROID_API__ >= 9)
    return m_Activity->callMethod<jboolean>("getSignedInGPGS");
    //QAndroidJniObject param1 = QAndroidJniObject::fromString(m_StartAdId);
    //m_Activity->callMethod<void>("SetStartAdId", "(Ljava/lang/String;)V", param1.object<jstring>());
#endif
}

void AdCtl::signInGPGS()
{
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("loginGPGS");
#endif
}

void AdCtl::submitScoreGPGS(QString leaderBoardId, int score)
{
#if (__ANDROID_API__ >= 9)
    QAndroidJniObject param1 = QAndroidJniObject::fromString(leaderBoardId);
    m_Activity->callMethod<void>("submitScoreGPGS", "(Ljava/lang/String;I)V", param1.object<jstring>(), score);
#endif
}

void AdCtl::unlockAchievementGPGS(QString achievementId)
{
#if (__ANDROID_API__ >= 9)
    QAndroidJniObject param1 = QAndroidJniObject::fromString(achievementId);
    m_Activity->callMethod<void>("unlockAchievementGPGS", "(Ljava/lang/String;)V", param1.object<jstring>());
#endif
}

void AdCtl::showLeaderboardGPGS()
{
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("getLeaderboardGPGS");
#endif
}

void AdCtl::showAchievementsGPGS()
{
#if (__ANDROID_API__ >= 9)
    m_Activity->callMethod<void>("getAchievementsGPGS");
#endif
}

void AdCtl::showAdMobInterstitial()
{
    m_AdMobInterstitial->Show();
}
