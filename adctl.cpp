#include "adctl.h"
#include "QtAdMobBanner.h"
#include "QtAdMobInterstitial.h"
#include <QDebug>
#include <QTimer>
#include "ganalytics.h"

#include "AdCtl_platform.h"
AdCtl* AdCtl::m_instance = nullptr;

AdCtl* AdCtl::getInstance(){
    if(!m_instance){
        m_instance = new AdCtl;
    }
    return m_instance;
}

AdCtl::AdCtl(QObject *parent) : QObject(parent)
{
    m_AdMobBanner = CreateQtAdMobBanner();
    m_AdMobInterstitial = CreateQtAdMobInterstitial();
    m_gpgsSignedIn = false;

    gpgsTimer = new QTimer(this);
    gpgsTimer->setInterval(1000);
    connect(gpgsTimer,SIGNAL(timeout()), this, SLOT(isGPGSSignedIn()));
    //gpgsTimer->start();

    m_platform = new AdCtl_platform;
    m_platform->setAdCtlObject(this);
    cacheAdMobBannerHeight = 0;
    cacheAdMobBannerWidth = 0;

    cacheStartAdBannerHeight = 0;
    cacheStartAdBannerWidth = 0;

    //mm and dp
    float density = m_platform->getDensity();
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
    delete m_platform;
}

void AdCtl::init()
{
    if (m_AdInitialized) { qDebug() << "AdMob alredy initialized!"; return; }

    m_platform->setStartAdId(m_StartAdId);

    if (m_AdMobBannerEnabled) {
        m_AdMobBanner->Initialize();
        m_AdMobBanner->SetSize(IQtAdMobBanner::Banner);
        m_AdMobBanner->SetUnitId(m_BannerAdMobId);

        foreach(const QString &testDevice, m_testDevices) {
            m_AdMobBanner->AddTestDevice(testDevice);
        }

        m_AdMobBanner->Show();
    }

    if (m_AdMobInterstitialEnabled) {
        m_AdMobInterstitial->LoadWithUnitId(m_InterstitialAdMobId);
        m_AdMobInterstitial->AddTestDevice("DCE0DB737EC089D97AB4EFCBA2F9B322");

        foreach(const QString &testDevice, m_testDevices) {
            m_AdMobInterstitial->AddTestDevice(testDevice);
        }
    }

    if (m_StartAdBannerEnabled) {
        m_platform->initStartAd();
    }

    if (m_GAnalyticsEnabled) {
        gAnalytics = new GAnalytics(m_GAnalyticsId);
    }

    //this timer is required!
    adctlTimer = new QTimer(this);
    connect(adctlTimer, SIGNAL(timeout()), this, SLOT(adctlTimerSlot()));
    adctlTimer->start();

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

void AdCtl::setTestDevices(const QStringList &testDevices)
{
    if(testDevices!=m_testDevices){
        m_testDevices = testDevices;
        emit testDevicesChanged();
    }
}

//This slot called by timer. Why? Because we don't know,
//when target platform is currently initialize our banners.
//When banners alredy initialized, we stop this timer and reposition our banners to setted by user positions.
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
        m_AdMobBannerShowHideTrigger = true;
        m_AdMobWidthAlredyGreatThanZero = true;
        emit adMobBannerShowed();
    }

    if ((StartAdBannerWidth() > 0) && !m_StartAdWidthAlredyGreatThanZero) {
        m_isStartAdBannerShowed = true;
        m_StartAdBannerShowHideTrigger = true;
        m_StartAdWidthAlredyGreatThanZero = true;
        emit startAdBannerShowed();
    }

    if ((m_AdMobWidthAlredyGreatThanZero || !m_AdMobBannerEnabled)
            && (m_StartAdWidthAlredyGreatThanZero || !m_StartAdBannerEnabled)) {
        adctlTimer->stop();
        adctlTimer->start(2000);
    }
}

bool AdCtl::initialized() const
{
    return m_AdInitialized;
}

//AdMob banner enabled
bool AdCtl::AdMobBannerEnabled() const
{
    return m_AdMobBannerEnabled;
}

bool AdCtl::AdMobBannerIsLoaded() const
{
    if (m_AdMobBannerEnabled) {
        return m_AdMobBanner->IsLoaded();
    } else {
        return false;
    }
    return false;
}

void AdCtl::setAdMobBannerEnabled(const bool AdMobBannerEnabled)
{
    if(m_AdMobBannerEnabled != AdMobBannerEnabled)
    {
        m_AdMobBannerEnabled = AdMobBannerEnabled;
        emit adMobBannerEnabledChanged();
    }
}

//AdMob interstitial enabled
bool AdCtl::AdMobIinterstitialEnabled() const
{
    return m_AdMobInterstitialEnabled;
}

void AdCtl::setAdMobIinterstitialEnabled(bool AdMobIinterstitialEnabled)
{
    if(m_AdMobInterstitialEnabled != AdMobIinterstitialEnabled)
    {
        m_AdMobInterstitialEnabled = AdMobIinterstitialEnabled;
        emit adMobIinterstitialEnabledChanged();
    }
}

bool AdCtl::AdMobIinterstitialIsLoaded() const
{
    if (m_AdMobInterstitialEnabled) {
        return m_AdMobInterstitial->IsLoaded();
    } else {
        return false;
    }
    return false;
}

//StartAd banner enabled
bool AdCtl::StartAdBannerEnabled() const
{
    return m_StartAdBannerEnabled;
}

void AdCtl::setStartAdBannerEnabled(bool StartAdBannerEnabled)
{
    if(m_StartAdBannerEnabled != StartAdBannerEnabled)
    {
        m_StartAdBannerEnabled = StartAdBannerEnabled;
        emit startAdBannerEnabledChanged();
    }
}

//GAnalytics enabled
bool AdCtl::GAnalyticsEnabled() const
{
    return m_GAnalyticsEnabled;
}

void AdCtl::setGAnalyticsEnabled(bool GAnalyticsEnabled)
{
    if(m_GAnalyticsEnabled != GAnalyticsEnabled)
    {
        m_GAnalyticsEnabled = GAnalyticsEnabled;
        emit gAnalyticsEnabledChanged();
    }
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
    return m_platform->startAdBannerWidth();
}

int AdCtl::StartAdBannerHeight() const
{
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return 0; }
    return m_platform->startAdBannerHeight();
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
    //qDebug() << "StartAdBannerSize C++" << m_StartAdBannerSize;
    //if (!m_AdInitialized || !m_StartAdBannerEnabled) { return; }
    m_platform->setStartAdBannerSize(StartAdBannerSize.width(),StartAdBannerSize.height());
}

QSize AdCtl::StartAdBannerSize() const
{
    return m_StartAdBannerSize;
}

int AdCtl::adMobBannerRealX()
{
    return m_platform->adMobBannerX();
}

int AdCtl::adMobBannerRealY()
{
    return m_platform->adMobBannerY();
}

int AdCtl::startAdBannerRealX()
{
    return m_platform->startAdBannerX();
}

int AdCtl::startAdBannerRealY()
{
    return m_platform->startAdBannerY();
}

void AdCtl::setStartAdBannerPosition(const QPoint position)
{
    m_StartAdBannerPosition = position;
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return; }
    m_platform->setStartAdBannerPosition(position.x(),position.y());
}

//set ids
void AdCtl::setAdMobBannerId(const QString &BannerAdMobId)
{
    if (!m_BannerAdMobId.isEmpty()) { qDebug() << "AdMob ID alredy set!"; return; }
    m_BannerAdMobId = BannerAdMobId;
}

void AdCtl::setAdMobInterstitialId(const QString &InterstitialAdMobId)
{
    if (!m_InterstitialAdMobId.isEmpty()) { qDebug() << "AdMob ID alredy set!"; return; }
    m_InterstitialAdMobId = InterstitialAdMobId;
}

void AdCtl::setStartAdId(const QString &StartAdId)
{
    if (!m_StartAdId.isEmpty()) { qDebug() << "StartAd ID alredy set!"; return; }
    m_StartAdId = StartAdId;
    m_platform->setStartAdId(StartAdId);
    emit startAdIdChanged();
}

void AdCtl::setGAnalyticsId(const QString &GAnalyticsId)
{
    if (!m_GAnalyticsId.isEmpty()) { qDebug() << "GAnalytics ID alredy set!"; return; }
    m_GAnalyticsId = GAnalyticsId;
    emit gAnalyticsIdChanged();
}

QString AdCtl::getInterstitialAdMobId() const
{
    return m_InterstitialAdMobId;
}

QString AdCtl::getBannerAdMobId() const
{
    return m_BannerAdMobId;
}

QString AdCtl::getStartAdId() const
{
    return m_StartAdId;
}

QString AdCtl::getGAnaliticsId() const
{
    return m_GAnalyticsId;
}

void AdCtl::setAdMobBannerVisible(const bool visible)
{
    if(m_AdMobBannerVisible!=visible) {
        m_AdMobBannerVisible=visible;
        if(visible) {
            showAdMobBanner();
        } else {
            hideAdMobBanner();
        }
        emit adMobBannerVisibleChanged();
    }
}

void AdCtl::setAdMobIinterstitialVisible(const bool visible)
{
    if(m_AdMobInterstitialVisible!=visible) {
        m_AdMobInterstitialVisible=visible;
        if(visible) {
            showAdMobInterstitial();
        }
        emit adMobIinterstitialVisibleChanged();
    }
}

void AdCtl::setStartAdBannerVisible(const bool visible)
{
    if(m_StartAdBannerVisible!=visible) {
        m_StartAdBannerVisible=visible;
        if(visible) {
            showStartAdBanner();
        } else {
            hideStartAdBanner();
        }
        emit startAdBannerVisibleChanged();
    }
}

bool AdCtl::AdMobBannerVisible() const
{
    return m_AdMobBannerVisible;
}

bool AdCtl::AdMobIinterstitialVisible() const
{
    return m_AdMobInterstitialVisible;
}

bool AdCtl::StartAdBannerVisible() const
{
    return m_StartAdBannerVisible;
}

QStringList AdCtl::getTestDevices() const
{
    return m_testDevices;
}

void AdCtl::showAdMobBanner()
{
    if (!m_AdInitialized || !m_AdMobBannerEnabled) { return; }

    if (!m_AdMobBannerShowHideTrigger) {
        m_AdMobBanner->Show();
        m_AdMobBannerShowHideTrigger = true;
    }
}

void AdCtl::hideAdMobBanner()
{
    if (!m_AdInitialized || !m_AdMobBannerEnabled) { return; }

    if (m_AdMobBannerShowHideTrigger) {
        m_AdMobBanner->Hide();
        m_AdMobBannerShowHideTrigger = false;
    }
}

void AdCtl::showStartAdBanner()
{
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return; }

    if (!m_StartAdBannerShowHideTrigger) {
        m_platform->showStartAd();
        m_StartAdBannerShowHideTrigger = true;
    }
}

void AdCtl::hideStartAdBanner()
{
    if (!m_AdInitialized || !m_StartAdBannerEnabled) { return; }

    if (m_StartAdBannerShowHideTrigger) {
        m_platform->hideStartAd();
        m_StartAdBannerShowHideTrigger = false;
    }
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

bool AdCtl::isGPGSSignedIn()
{
    bool checkGpgsSignedIn = m_platform->isGPGSSignedIn();
    //qDebug() << "GPGS SIGNED IN" << checkGpgsSignedIn << m_gpgsSignedIn;
    if (checkGpgsSignedIn != m_gpgsSignedIn) {
        setGPGSSignedIn(checkGpgsSignedIn);
        gpgsTimer->stop();
    }
    return m_gpgsSignedIn;
}

void AdCtl::setGPGSSignedIn(bool gpgsSignedIn)
{
    qDebug() << "Set GPGS Signed IN" << gpgsSignedIn << m_gpgsSignedIn;
    m_gpgsSignedIn = gpgsSignedIn;
    emit gpgsSignedInChanged(gpgsSignedIn);
}

void AdCtl::signInGPGS()
{
    m_platform->signInGPGS();
}

void AdCtl::getLeaderBoardScoreAsync(QString leaderboard)
{
    m_platform->getLeaderBoardScore(leaderboard);
}

void AdCtl::submitScoreGPGS(QString leaderBoardId, int score)
{
    m_platform->submitScoreGPGS(leaderBoardId,score);
}

void AdCtl::unlockAchievementGPGS(QString achievementId)
{
    m_platform->unlockAchievementGPGS(achievementId);
}

void AdCtl::showLeaderboardGPGS()
{
    m_platform->showLeaderboardGPGS();
}

void AdCtl::showLeaderboard(QString leaderboardId)
{
    m_platform->showLeaderboard(leaderboardId);
}

void AdCtl::showAchievementsGPGS()
{
    m_platform->showAchievementsGPGS();
}

void AdCtl::share(QString url)
{
    m_platform->shareImage(url);
}

void AdCtl::showAdMobInterstitial()
{
    if(m_AdMobInterstitialEnabled)
        m_AdMobInterstitial->Show();
}
