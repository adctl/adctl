#include "AdCtl_platform_interface.h"
#include <QString>
#include <QGuiApplication>
#include <QScreen>

AdCtl_platform_interface::~AdCtl_platform_interface(){

}

void AdCtl_platform_interface::initStartAd(){

}

void AdCtl_platform_interface::setStartAdId(const QString& id){
    Q_UNUSED(id)
}

void AdCtl_platform_interface::setStartAdBannerSize(const int width, const int height){
    Q_UNUSED(width)
    Q_UNUSED(height)
}

void AdCtl_platform_interface::setStartAdBannerPosition(const int x, const int y){
    Q_UNUSED(x)
    Q_UNUSED(y)
}

int AdCtl_platform_interface::startAdBannerHeight() const{
    return 0;
}

int AdCtl_platform_interface::startAdBannerWidth() const{
    return 0;
}

int AdCtl_platform_interface::startAdBannerX() const{
    return 0;
}

int AdCtl_platform_interface::startAdBannerY() const{
    return 0;
}

void AdCtl_platform_interface::showStartAd(){

}

void AdCtl_platform_interface::hideStartAd(){

}

float AdCtl_platform_interface::getDensity() const{
    QScreen *screen = qApp->primaryScreen();
    return screen->physicalDotsPerInch();
}

int AdCtl_platform_interface::adMobBannerX() const{
    return 0;
}

int AdCtl_platform_interface::adMobBannerY() const{
    return 0;
}

bool AdCtl_platform_interface::isGPGSSignedIn() const{
    return false;
}

void AdCtl_platform_interface::signInGPGS(){

}

void AdCtl_platform_interface::getLeaderBoardScore(const QString &leaderboardId){
    Q_UNUSED(leaderboardId)
}
void AdCtl_platform_interface::submitScoreGPGS(const QString& leaderBoardId, int score){
    Q_UNUSED(leaderBoardId)
    Q_UNUSED(score)
}

void AdCtl_platform_interface::unlockAchievementGPGS(const QString& achievementId){
    Q_UNUSED(achievementId)
}

void AdCtl_platform_interface::showLeaderboard(const QString &leaderboardId)
{

}

void AdCtl_platform_interface::showLeaderboardGPGS(){

}

void AdCtl_platform_interface::showAchievementsGPGS(){

}

void AdCtl_platform_interface::shareImage(QString path)
{

}

void AdCtl_platform_interface::setAdCtlObject(QObject *obj)
{
    m_adctl_obj = obj;
}

