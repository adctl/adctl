#ifndef ADMOBBANNERCTL_H
#define ADMOBBANNERCTL_H

#include <QObject>
#include <QPoint>

class IQtAdMobBanner;
class IQtAdMobInterstitial;
class QTimer;
class QAndroidJniObject;

class AdMobCtl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool bannerVisible READ bannerVisible WRITE setBannerVisible)
    Q_PROPERTY(bool interstitialEnabled READ interstitialEnabled WRITE setInterstitialEnabled)

    Q_PROPERTY(QPoint bannerPosition READ bannerPosition WRITE setBannerPosition)
    Q_PROPERTY(int bannerHeight READ bannerHeight NOTIFY bannerHeightChanged)
    Q_PROPERTY(int bannerWidth READ bannerWidth NOTIFY bannerHeightChanged)
    Q_PROPERTY(QString unitId WRITE setUnitId)
    Q_PROPERTY(QString sadUnitId WRITE setSadUnitId)

public:
    explicit AdMobCtl(QObject *parent = 0);
    ~AdMobCtl();

signals:
    void showed();
    void  bannerHeightChanged(int height);
    void  bannerWidthChanged(int width);

public slots:
    void showedTimerSlot();

    void setBannerVisible(const bool bannerVisible);
    bool bannerVisible() const;

    bool interstitialEnabled() const;
    void setInterstitialEnabled(bool interstitialEnabled);

    void setBannerPosition(const QPoint bannerPosition);
    QPoint bannerPosition() const;

    int bannerHeight() const;

    int bannerWidth() const;

    void setUnitId(const QString &unitId);
    void setSadUnitId(const QString &unitId);
    void init();

private:
    IQtAdMobBanner *m_Banner;
    IQtAdMobInterstitial *m_Interstitial;
    bool m_bannerVisible, m_interstitialEnabled, m_sadEnabled;
    QPoint m_position;
    QString m_unitId, m_sadId;
    bool m_initialized = false, m_setunitId = false;
    QTimer *showedTimer;
    int cachedBannerHeight, cachedBannerWidth;
    QAndroidJniObject *m_Activity;
    int m_adMode;
};

#endif // ADMOBBANNERCTL_H
