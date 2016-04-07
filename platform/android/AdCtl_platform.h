#ifndef ADCTL_PLATFORM_H
#define ADCTL_PLATFORM_H

#include "../AdCtl_platform_interface.h"

class QAndroidJniObject;
class QObject;

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
    uint64_t  getLeaderBoardScoreBlocking(const QString &leaderboardId);
    void getLeaderBoardScore(const QString &leaderboardId);
    void submitScoreGPGS(const QString& leaderBoardId, int score);
    void unlockAchievementGPGS(const QString& achievementId);
    void showLeaderboard(const QString& leaderboardId);
    void showLeaderboardGPGS();
    void showAchievementsGPGS();

    //tools
    void shareImage(QString path);
private:
    QAndroidJniObject *m_Activity;
};

#endif
