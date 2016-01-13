#ifndef ADCTL_PLATFORM_H
#define ADCTL_PLATFORM_H

#include "../AdCtl_platform_interface.h"

class QAndroidJniObject;

class AdCtl_platform:public AdCtl_platform_interface{
public:
    AdCtl_platform();
    ~AdCtl_platform();

    void initStartAd();
    void setStartAdId(const QString& id);
    void setStartAdBannerSize(const int width,const int height);
    void setStartAdBannerPosition(const int x,const int y);

    int startAdBannerHeight()const;
    int startAdBannerWidth()const;
    int startAdBannerX()const;
    int startAdBannerY()const;

    void showStartAd();
    void hideStartAd();

    float getDensity()const;

    int adMobBannerX()const;
    int adMobBannerY()const;

    bool isGPGSSignedIn()const;
    void signInGPGS();
    void submitScoreGPGS(const QString& leaderBoardId, int score);
    void unlockAchievementGPGS(const QString& achievementId);
    void showLeaderboardGPGS();
    void showAchievementsGPGS();
private:
    QAndroidJniObject *m_Activity;
};

#endif
