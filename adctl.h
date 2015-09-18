#ifndef ADMOBBANNERCTL_H
#define ADMOBBANNERCTL_H

#include <QObject>
#include <QPoint>

class IQtAdMobBanner;
class IQtAdMobInterstitial;
class QTimer;
class QAndroidJniObject;

class AdCtl : public QObject
{
    Q_OBJECT
    //enabled properties
    Q_PROPERTY(bool AdMobBannerEnabled READ AdMobBannerEnabled WRITE setAdMobBannerEnabled)
    Q_PROPERTY(bool AdMobIinterstitialEnabled READ AdMobIinterstitialEnabled WRITE setAdMobIinterstitialEnabled)
    Q_PROPERTY(bool StartAdBannerEnabled READ StartAdBannerEnabled WRITE setStartAdBannerEnabled)

    //AdMob width and height
    Q_PROPERTY(int AdMobBannerHeight READ AdMobBannerHeight NOTIFY AdMobBannerHeightChanged)
    Q_PROPERTY(int AdMobBannerWidth READ AdMobBannerWidth NOTIFY AdMobBannerWidthChanged)

    //StartAd width and height
    Q_PROPERTY(int StartAdBannerHeight READ StartAdBannerHeight NOTIFY StartAdBannerHeightChanged)
    Q_PROPERTY(int StartAdBannerWidth READ StartAdBannerWidth NOTIFY StartAdBannerWidthChanged)

    //AdMob position
    Q_PROPERTY(QPoint AdMobBannerPosition READ AdMobBannerPosition WRITE setAdMobBannerPosition)

    //StartAd position
    Q_PROPERTY(QPoint StartAdBannerPosition READ StartAdBannerPosition WRITE setStartAdBannerPosition)

    //ids
    Q_PROPERTY(QString AdMobId WRITE setAdMobId)
    Q_PROPERTY(QString StartAdId WRITE setStartAdId)

public:
    explicit AdCtl(QObject *parent = 0);
    ~AdCtl();

signals:
    void showed();
    void  AdMobBannerHeightChanged(int height);
    void  AdMobBannerWidthChanged(int width);
    void  StartAdBannerHeightChanged(int height);
    void  StartAdBannerWidthChanged(int width);

public slots:

    //init library with ids and bool flags
    void init();

    //Timer for control, update and emit properties changes for banners
    void adctlTimerSlot();

    //AdMob banner enabled
    void setAdMobBannerEnabled(const bool AdMobBannerEnabled);
    bool AdMobBannerEnabled() const;

    //AdMob interstitial enabled
    bool AdMobIinterstitialEnabled() const;
    void setAdMobIinterstitialEnabled(bool AdMobIinterstitialEnabled);

    //StartAd banner enabled
    void setStartAdBannerEnabled(const bool StartAdBannerEnabled);
    bool StartAdBannerEnabled() const;

    //AdMob width and height
    int AdMobBannerHeight() const;
    int AdMobBannerWidth() const;

    //StartAd width and height
    int StartAdBannerHeight() const;
    int StartAdBannerWidth() const;

    //AdMob position
    void setAdMobBannerPosition(const QPoint AdMobBannerPosition);
    QPoint AdMobBannerPosition() const;

    //StartAd position
    void setStartAdBannerPosition(const QPoint StartAdBannerPosition);
    QPoint StartAdBannerPosition() const;

    //ids
    void setAdMobId(const QString &AdMobId);
    void setStartAdId(const QString &StartAdId);

    //ctl methods
    void showAdMobBanner();
    void hideAdMobBanner();
    void showAdMobInterstitial();

private:
    //Timer for control, update and emit properties changes for banners
    QTimer *adctlTimer;

    //Pointers for AdMob
    IQtAdMobBanner *m_AdMobBanner;
    IQtAdMobInterstitial *m_AdMobInterstitial;

    //Enabled control vars
    bool m_AdMobBannerEnabled;
    bool m_AdMobInterstitialEnabled;
    bool m_StartAdBannerEnabled;

    //ids
    QString m_AdMobId;
    QString m_StartAdId;

    //initialized
    bool m_AdInitialized = false;

    //jni
    QAndroidJniObject *m_Activity;

    //cache
    int cacheAdMobBannerHeight;
    int cacheAdMobBannerWidth;
    int cacheStartAdBannerHeight;
    int cacheStartAdBannerWidth;

    //poritions
    QPoint m_AdMobBannerPosition;
    QPoint m_StartAdBannerPosition;

};

#endif // ADMOBBANNERCTL_H
