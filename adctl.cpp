#include "adctl.h"
#include "QtAdMobBanner.h"
#include "QtAdMobInterstitial.h"
#include <QtQml>
#include <QDebug>
#include <QTimer>

#ifdef __ANDROID__
#include <android/api-level.h>
#endif

#ifdef __ANDROID_API__
#include <QAndroidJniObject>
#include <QPA/QPlatformNativeInterface.h>
#include <QApplication>
#endif

AdMobCtl::AdMobCtl(QObject *parent) : QObject(parent)
{
    m_Banner = CreateQtAdMobBanner();
    m_Interstitial = CreateQtAdMobInterstitial();

    #ifdef __ANDROID_API__
    QPlatformNativeInterface* interface = QApplication::platformNativeInterface();
    jobject activity = (jobject)interface->nativeResourceForIntegration("QtActivity");
    if (activity)
    {
        m_Activity = new QAndroidJniObject(activity);
    }
    #endif
}

AdMobCtl::~AdMobCtl()
{
    delete m_Banner;
    delete m_Interstitial;
}

void AdMobCtl::init()
{
    if (!m_initialized) {
        qDebug() << "Start AdMob initialization!";
        #ifdef __ANDROID_API__
        QAndroidJniObject param1 = QAndroidJniObject::fromString(m_sadId);
        m_Activity->callMethod<void>("SetSAdBannerUnitId", "(Ljava/lang/String;)V", param1.object<jstring>());
        #endif
        m_Banner->Initialize();
        m_Banner->SetSize(IQtAdMobBanner::Banner);

        m_Banner->SetUnitId(m_unitId);
        m_Interstitial->LoadWithUnitId(m_unitId);

        showedTimer = new QTimer(this);
        connect(showedTimer, SIGNAL(timeout()), this, SLOT(showedTimerSlot()));
        showedTimer->start(500);

        m_initialized = true;

        qDebug() << "Finish AdMob initialization! Show...";

        //        m_Banner->SetPosition(m_position);
        //        //m_Banner->AddTestDevice("DCE0DB737EC089D97AB4EFCBA2F9B322");
        //        if (m_bannerVisible) {
        //            m_Banner->Show();
        //        }

        //        if (m_interstitialEnabled)  {
        //            //m_Interstitial->AddTestDevice("DCE0DB737EC089D97AB4EFCBA2F9B322");
        //            m_Interstitial->Show();
        //        }

        //show();
    } else {
        qDebug() << "AdMob alredy initialized!";
    }
}

void AdMobCtl::setUnitId(const QString &unitId)
{
    qDebug() << "set unitId" << unitId << m_bannerVisible << m_interstitialEnabled;
    if (!m_setunitId) {
        m_unitId = unitId;
        m_setunitId = true;
    } else {
        qDebug() << "AdMob alredy set unit id!";
    }
}

void AdMobCtl::setSadUnitId(const QString &unitId)
{
    m_sadId = unitId;
}

//void AdMobCtl::setAdMode(AdMobCtl::AdMode mode)
//{
//    m_adMode = mode;
//}

void AdMobCtl::showedTimerSlot()
{
    if (cachedBannerHeight != bannerHeight()) {
        cachedBannerHeight = bannerHeight();
        emit bannerHeightChanged(bannerHeight());
    }
    if (cachedBannerWidth != bannerWidth()) {
        cachedBannerWidth = bannerWidth();
        emit bannerWidthChanged(bannerWidth());
    }
    if (m_Banner->IsShow() && (showedTimer->interval() == 500)) {
        emit showed();
        showedTimer->stop();
        showedTimer->start(2000);
    }
}

void AdMobCtl::setBannerVisible(const bool visible)
{
    m_bannerVisible = visible;
    if (m_bannerVisible) {
        if (m_initialized) m_Banner->Show();
    } else {
        if (m_initialized) m_Banner->Hide();
    }
}

bool AdMobCtl::bannerVisible() const
{
    return m_bannerVisible;
}

void AdMobCtl::setBannerPosition(const QPoint position)
{
    m_position = position;
    if (m_initialized) m_Banner->SetPosition(m_position);
}

QPoint AdMobCtl::bannerPosition() const
{
    return m_position;
}

int AdMobCtl::bannerWidth() const
{
    if (m_initialized) {
        return m_Banner->GetSizeInPixels().width();
    } else {
        return 0;
    }
}

int AdMobCtl::bannerHeight() const
{
    if (m_initialized) {
        return m_Banner->GetSizeInPixels().height();
    } else {
        return 0;
    }
}

bool AdMobCtl::interstitialEnabled() const
{
    return m_interstitialEnabled;
}

void AdMobCtl::setInterstitialEnabled(bool interstitialEnabled)
{
    //qDebug() << "set m_interstitialEnabled" << interstitialEnabled;
    m_interstitialEnabled = interstitialEnabled;
}
