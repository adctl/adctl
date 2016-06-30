#ifndef ADCTL_PLATFORM_H
#define ADCTL_PLATFORM_H

#include <QString>

#include "../AdCtl_platform_interface.h"

struct AdCtl_platform_private;

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
private:
    AdCtl_platform_private* d;
};

#endif