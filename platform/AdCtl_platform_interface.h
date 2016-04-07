#ifndef ADCTL_PLATFORM_INTERFACE_H
#define ADCTL_PLATFORM_INTERFACE_H
#include <stdint.h>
class QString;
class QObject;

class AdCtl_platform_interface{
public:
    AdCtl_platform_interface(){}
    virtual ~AdCtl_platform_interface()=0;

    virtual void init(){}
    virtual void initStartAd();
    virtual void setStartAdId(const QString& id);
    virtual void setStartAdBannerSize(const int width,const int height);
    virtual void setStartAdBannerPosition(const int x,const int y);

    virtual int startAdBannerHeight()const;
    virtual int startAdBannerWidth()const;
    virtual int startAdBannerX()const;
    virtual int startAdBannerY()const;

    virtual void showStartAd();
    virtual void hideStartAd();

    virtual float getDensity()const;

    virtual int adMobBannerX()const;
    virtual int adMobBannerY()const;

    virtual bool isGPGSSignedIn()const;
    virtual void signInGPGS();
    virtual void getLeaderBoardScore(const QString& leaderboardId);
    virtual void submitScoreGPGS(const QString& leaderBoardId, int score);
    virtual void unlockAchievementGPGS(const QString& achievementId);
    virtual void showLeaderboard(const QString& leaderboardId);
    virtual void showLeaderboardGPGS();
    virtual void showAchievementsGPGS();

    virtual void shareImage(QString path);

    void setAdCtlObject(QObject* obj);
protected:
    QObject *m_adctl_obj;
};

#endif
