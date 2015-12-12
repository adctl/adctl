#include "AdCtl_platform.h"
#include <QString>

#include "SADWebView.h"

@interface AdctlViewController () <SADWebViewDelegate>
{
    SADWebView* webView;
}
//-(void)loadData;
@end

@implementation ContentViewController

- (IBAction)reloadSADView:(id)sender
{
    qDebug("reload view (ios)");
//    [self loadData];
}

-(void)onReceivedAd
{
    qDebug("start ad recieved (ios)");
}
-(void)onShowedAd
{
    qDebug("start ad showed (ios)");
}
-(void)onError:(SADVIEW_ERROR)error
{
    qDebug("start ad error (ios)");
}
-(void)onAdClicked
{
    qDebug("start ad clicked (ios)");
}
-(void)noAdFound
{
    qDebug("start ad not found (ios)");
}

@end


AdCtl_platform::AdCtl_platform():m_controller([[AdctlViewController alloc]init]){

}

AdCtl_platform::~AdCtl_platform(){
    [m_controller release];
}

void AdCtl_platform::initStartAd(){
    if (!m_controller.webView) {
        m_controller.webView = [[SADWebView alloc]initWithId:////];
        m_controller.webView.sadDelegate = m_controller;
    }
    [m_controller.webView loadAd:m_controller.webView LANGUAGE_RU];
}

void AdCtl_platform::setStartAdId(const QString& id){
    m_startAdId=id;
}

void AdCtl_platform::setStartAdBannerSize(const int width, const int height){
    Q_UNUSED(width)
    Q_UNUSED(height)
}

void AdCtl_platform::setStartAdBannerPosition(const int x, const int y){
    Q_UNUSED(x)
    Q_UNUSED(y)
}

int AdCtl_platform::startAdBannerHeight() const{
    return 0;
}

int AdCtl_platform::startAdBannerWidth() const{
    return 0;
}

int AdCtl_platform::startAdBannerX() const{
    return 0;
}

int AdCtl_platform::startAdBannerY() const{
    return 0;
}

void AdCtl_platform::showStartAd(){

}

void AdCtl_platform::hideStartAd(){

}

