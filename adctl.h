#ifndef ADMOBBANNERCTL_H
#define ADMOBBANNERCTL_H

#include <QObject>
#include <QPoint>
#include <QVariant>
#include <QSize>
#include <QtQml> //place here to avoid include for registration

#define REGISTER_ADCTL qmlRegisterType<AdCtl>("adctl",1,0,"AdCtl")

class IQtAdMobBanner;
class IQtAdMobInterstitial;
class QTimer;
class AdCtl_platform_interface;
class GAnalytics;

class AdCtl : public QObject
{
    Q_OBJECT

    //enabled properties
    Q_PROPERTY(bool isInitialized READ initialized)
    Q_PROPERTY(bool adMobBannerEnabled READ AdMobBannerEnabled WRITE setAdMobBannerEnabled NOTIFY adMobBannerEnabledChanged)
    Q_PROPERTY(bool adMobIinterstitialEnabled READ AdMobIinterstitialEnabled WRITE setAdMobIinterstitialEnabled NOTIFY adMobIinterstitialEnabledChanged)
    Q_PROPERTY(bool startAdBannerEnabled READ StartAdBannerEnabled WRITE setStartAdBannerEnabled NOTIFY startAdBannerEnabledChanged)
    Q_PROPERTY(bool gAnalyticsEnabled READ GAnalyticsEnabled WRITE setGAnalyticsEnabled NOTIFY gAnalyticsEnabledChanged)

    //AdMob width and height
    Q_PROPERTY(int adMobBannerHeight READ AdMobBannerHeight NOTIFY adMobBannerHeightChanged)
    Q_PROPERTY(int adMobBannerWidth READ AdMobBannerWidth NOTIFY adMobBannerWidthChanged)

    //AdMob position
    Q_PROPERTY(QPoint adMobBannerPosition READ AdMobBannerPosition WRITE setAdMobBannerPosition)

    //StartAd width and height
    Q_PROPERTY(int startAdBannerHeight READ StartAdBannerHeight NOTIFY startAdBannerHeightChanged)
    Q_PROPERTY(int startAdBannerWidth READ StartAdBannerWidth NOTIFY startAdBannerWidthChanged)

    //StartAd position
    Q_PROPERTY(QPoint startAdBannerPosition READ StartAdBannerPosition WRITE setStartAdBannerPosition)
    //StartAd size
    Q_PROPERTY(QSize startAdBannerSize READ StartAdBannerSize WRITE setStartAdBannerSize)

    //Get real positions
    Q_PROPERTY(int adMobBannerRealX READ adMobBannerRealX)
    Q_PROPERTY(int AdMobBannerRealY READ adMobBannerRealY)
    Q_PROPERTY(int startAdBannerRealX READ startAdBannerRealX)
    Q_PROPERTY(int startAdBannerRealY READ startAdBannerRealY)

    Q_PROPERTY(bool adMobBannerVisible READ AdMobBannerVisible WRITE setAdMobBannerVisible NOTIFY adMobBannerVisibleChanged)
    Q_PROPERTY(bool adMobIinterstitialVisible READ AdMobIinterstitialVisible WRITE setAdMobIinterstitialVisible NOTIFY adMobIinterstitialVisibleChanged)
    Q_PROPERTY(bool startAdBannerVisible READ StartAdBannerVisible WRITE setStartAdBannerVisible NOTIFY startAdBannerVisibleChanged)

    //ID's for banners and analytics
    Q_PROPERTY(QString adMobBannerId READ getBannerAdMobId WRITE setAdMobBannerId NOTIFY adMobBannerIdChanged)
    Q_PROPERTY(QString adMobInterstitialId READ getInterstitialAdMobId WRITE setAdMobInterstitialId NOTIFY adMobInterstitialIdChanged)
    Q_PROPERTY(QString startAdId READ getStartAdId WRITE setStartAdId NOTIFY startAdIdChanged)
    Q_PROPERTY(QString gAnalyticsId READ getGAnaliticsId WRITE setGAnalyticsId NOTIFY gAnalyticsIdChanged)

    //list of test devices
    Q_PROPERTY(QStringList testDevices READ getTestDevices WRITE setTestDevices NOTIFY testDevicesChanged)

    //Google Play Game Services
    Q_PROPERTY(bool gpgsSignedIn READ isGPGSSignedIn WRITE setGPGSSignedIn NOTIFY gpgsSignedInChanged)

public:
    explicit AdCtl(QObject *parent = 0);
    ~AdCtl();

    // Do not forget to declare your class to the QML system.
    static void declareQML() {
        qmlRegisterType<AdCtl>("ru.forsk.adctl", 1, 0, "AdCtl");
    }

signals:

    //StartAd and AdMob showed and sizes
    void  adMobBannerShowed();
    void  startAdBannerShowed();
    void  adMobBannerHeightChanged(int height);
    void  adMobBannerWidthChanged(int width);
    void  startAdBannerHeightChanged(int height);
    void  startAdBannerWidthChanged(int width);
    void  gpgsSignedInChanged(bool gpgsSignedIn);

    //ids changed
    void adMobInterstitialIdChanged();
    void adMobBannerIdChanged();
    void startAdIdChanged();
    void gAnalyticsIdChanged();

    //visibility changed
    void adMobBannerVisibleChanged();
    void adMobIinterstitialVisibleChanged();
    void startAdBannerVisibleChanged();

    //enabled changed
    void adMobBannerEnabledChanged();
    void adMobIinterstitialEnabledChanged();
    void startAdBannerEnabledChanged();
    void gAnalyticsEnabledChanged();

    //Google Play Games
    //This is used when a score is returned for a leaderboad.(qstring,uint64,int)
    void scoreGPG(QVariant leaderboardID,QVariant score,QVariant gpgStatus);

    //test devices changed
    void testDevicesChanged();
public slots:

    //init library with ids and bool flags
    void init();

    //Timer for control, update and emit properties changes for banners (for first run)
    void adctlTimerSlot();

    bool initialized() const;

    //AdMob banner enabled
    void setAdMobBannerEnabled(const bool AdMobBannerEnabled);
    bool AdMobBannerEnabled() const;
    bool AdMobBannerIsLoaded() const;

    //AdMob interstitial enabled
    bool AdMobIinterstitialEnabled() const;
    void setAdMobIinterstitialEnabled(bool AdMobIinterstitialEnabled);
    bool AdMobIinterstitialIsLoaded() const;

    //StartAd banner enabled
    void setStartAdBannerEnabled(const bool StartAdBannerEnabled);
    bool StartAdBannerEnabled() const;

    //GAnalytics banner enabled
    void setGAnalyticsEnabled(const bool GAnalyticsEnabled);
    bool GAnalyticsEnabled() const;

    //AdMob width and height
    int AdMobBannerHeight() const;
    int AdMobBannerWidth() const;

    //StartAd width and height
    int StartAdBannerHeight() const;
    int StartAdBannerWidth() const;

    //AdMob set position
    void setAdMobBannerPosition(const QPoint AdMobBannerPosition);
    QPoint AdMobBannerPosition() const;

    //StartAd set position
    void setStartAdBannerPosition(const QPoint StartAdBannerPosition);
    QPoint StartAdBannerPosition() const;

    //StartAd set size
    void setStartAdBannerSize(const QSize StartAdBannerSize);
    QSize StartAdBannerSize() const;

    //Get Real Positions
    int adMobBannerRealX();
    int adMobBannerRealY();
    int startAdBannerRealX();
    int startAdBannerRealY();

    //ids
    void setAdMobBannerId(const QString &BannerAdMobId);
    void setAdMobInterstitialId(const QString &InterstitialAdMobId);
    void setStartAdId(const QString &StartAdId);
    void setGAnalyticsId(const QString &GAnalyticsId);
    QString getInterstitialAdMobId()const;
    QString getBannerAdMobId()const;
    QString getStartAdId()const;
    QString getGAnaliticsId()const;

    void setAdMobBannerVisible(const bool visible);
    void setAdMobIinterstitialVisible(const bool visible);
    void setStartAdBannerVisible(const bool visible);
    bool AdMobBannerVisible()const;
    bool AdMobIinterstitialVisible()const;
    bool StartAdBannerVisible()const;

    //test devices
    QStringList getTestDevices()const;
    void setTestDevices(const QStringList &testDevices);

    //ctl methods
    void showAdMobBanner();
    void hideAdMobBanner();
    void showAdMobInterstitial();
    void showStartAdBanner();
    void hideStartAdBanner();

    //Google Analytics
    void sendGaAppView(const QString &screenName = QString());
    void sendGaEvent(const QString &category = QString(),
                     const QString &action = QString(),
                     const QString &label = QString(),
                     const QVariant &value = QVariant());
    void endGaSession();

    //Google Play Game Services
    bool isGPGSSignedIn();
    void setGPGSSignedIn(bool gpgsSignedIn);

    void signInGPGS();
    /**
    * @brief This will request the score of a leaderboard.
    * @note Thie result will be delivered with the @scoreGPG signal.
    */
    void getLeaderBoardScoreAsync(QString leaderboard);
    void submitScoreGPGS(QString leaderBoardId, int score);
    void unlockAchievementGPGS(QString achievementId);
    void showLeaderboardGPGS();
    void showLeaderboard(QString leaderboardId);
    void showAchievementsGPGS();

    //Util/tool/maybenotneededbutheretheyare
    void share(QString url);

    //dp, pt and mm
    float dp();
    float mm();
    float pt();
    static AdCtl* getInstance();

protected:
    static AdCtl* m_instance;
    //Timer for control, update and emit properties changes for banners
    QTimer *adctlTimer;

    //Pointers
    IQtAdMobBanner *m_AdMobBanner;
    IQtAdMobInterstitial *m_AdMobInterstitial;
    GAnalytics *gAnalytics;

    //Enabled control vars
    bool m_AdMobBannerEnabled = false;
    bool m_AdMobInterstitialEnabled = false;
    bool m_StartAdBannerEnabled = false;
    bool m_GAnalyticsEnabled = false;

    bool m_AdMobBannerVisible = false;
    bool m_AdMobInterstitialVisible = false;
    bool m_StartAdBannerVisible = false;

    //ids
    QString m_BannerAdMobId;
    QString m_InterstitialAdMobId;
    QString m_StartAdId;
    QString m_GAnalyticsId;

    //test devices
    QStringList m_testDevices;

    //initialized
    bool m_AdInitialized = false;

    //plarform-specific layer
    AdCtl_platform_interface *m_platform;

    //cache
    int cacheAdMobBannerHeight;
    int cacheAdMobBannerWidth;
    int cacheStartAdBannerHeight;
    int cacheStartAdBannerWidth;

    //positions and sizes
    QPoint m_AdMobBannerPosition;
    QPoint m_StartAdBannerPosition;
    QSize m_StartAdBannerSize;

    //is banners showed
    bool m_isAdMobBannerShowed = false;
    bool m_isStartAdBannerShowed = false;
    bool m_AdMobBannerShowHideTrigger = false;
    bool m_StartAdBannerShowHideTrigger = false;

    //banners alredy visible bools
    bool m_AdMobWidthAlredyGreatThanZero = false;
    bool m_StartAdWidthAlredyGreatThanZero = false;

    //Google Play Game Services
    bool m_gpgsSignedIn;
    QTimer *gpgsTimer;

    //density (simple Fix Qt android density bug)
    float m_dp;
    float m_pt;
    float m_mm;
};

#endif // ADMOBBANNERCTL_H
