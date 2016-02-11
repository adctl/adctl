#include "AdCtl_platform.h"

/*#include "SADWebView.h"

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

@end*/


AdCtl_platform::AdCtl_platform()
    //:m_controller([[AdctlViewController alloc]init])
{

}

AdCtl_platform::~AdCtl_platform(){
    //[m_controller release];
}

void AdCtl_platform::initStartAd(){
    //if (!m_controller.webView) {
    //    m_controller.webView = [[SADWebView alloc]initWithId:NSString////];
    //    m_controller.webView.sadDelegate = m_controller;
    //}
    //[m_controller.webView loadAd:m_controller.webView LANGUAGE_RU];
}

void AdCtl_platform::setStartAdId(const QString& id){
    //m_startAdId=id;
}

void AdCtl_platform::setStartAdBannerSize(const int width, const int height){
    //m_controller.webView.frame.width=width;
    //m_controller.webView.frame.height=height;
}

void AdCtl_platform::setStartAdBannerPosition(const int x, const int y){
    //m_controller.webView.frame.x=x;
    //m_controller.webView.frame.y=y;
}

int AdCtl_platform::startAdBannerHeight() const{
    //return m_controller.webView.frame.height;
}

int AdCtl_platform::startAdBannerWidth() const{
    //return m_controller.webView.frame.width;
}

int AdCtl_platform::startAdBannerX() const{
    //return m_controller.webView.frame.x;
}

int AdCtl_platform::startAdBannerY() const{
    //return m_controller.webView.frame.y;
}

void AdCtl_platform::showStartAd(){
    //m_controller.webView.visible=true;
}

void AdCtl_platform::hideStartAd(){
    //m_controller.webView.visible=false;
}

