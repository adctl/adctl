#include "AdCtl_platform.h"

#include <UIKit/UIKit.h>
#include "SADWebView.h"

#include <QGuiApplication>
#include <QtGui/qpa/qplatformnativeinterface.h>

@interface AdctlViewController : UIViewController

@end

@interface AdctlViewController () <SADWebViewDelegate>
{
    SADWebView* webView;

    int m_height;
    int m_width;
    int m_x;
    int m_y;
}
-(void)initStartAd:(NSString*)id;
-(void)setSize:(int)width :(int)height;
-(void)setPos:(int)x :(int)y;

-(int)getHeight;
-(int)getWidth;
-(int)getX;
-(int)getY;

-(void)setViewVisible:(bool)visible;
@end

@implementation AdctlViewController

- (IBAction)reloadSADView:(id)sender
{
    Q_UNUSED(sender)
    qDebug("reload view (ios)");
    //[self loadData];
}

-(void)onReceivedAd
{
    [self setPos:m_x:m_y];
    [self setSize:m_width:m_height];
    qDebug("start ad recieved (ios)");
}
-(void)onShowedAd
{
    qDebug("start ad showed (ios)");
}
-(void)onError:(SADVIEW_ERROR)error
{
    qDebug("start ad error %d (ios)",error);
}
-(void)onAdClicked
{
    qDebug("start ad clicked (ios)");
}
-(void)noAdFound
{
    qDebug("start ad not found (ios)");
}
-(void)initStartAd:(NSString*)id
{
    if(!webView) {
        webView = [[SADWebView alloc]initWithId:id];
        webView.sadDelegate = self;
        UIView *mainView = (__bridge UIView *)reinterpret_cast<void *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", qApp->topLevelWindows().at(0)));
        [mainView addSubview:webView];
    }
    [webView loadAd:LANGUAGE_EN];
}
-(void)setSize:(int)width :(int)height
{
    m_width=width;
    m_height=height;
    CGRect frame = webView.frame;
    frame.size.width=width;
    frame.size.height=webView.height;
    webView.frame = frame;
}
-(void)setPos:(int)x :(int)y
{
    m_x=x;
    m_y=y;
    CGRect frame = webView.frame;
    frame.origin.x=x;
    frame.origin.y=y;
    webView.frame = frame;
}
-(int)getHeight
{
    return webView.frame.size.height;
}
-(int)getWidth
{
    return webView.frame.size.width;
}
-(int)getX
{
    return webView.frame.origin.x;
}
-(int)getY
{
    return webView.frame.origin.y;
}
-(void)setViewVisible:(bool)visible
{
    webView.hidden=!visible;
}
@end

struct AdCtl_platform_private{
    AdCtl_platform_private()
        :m_controller([[AdctlViewController alloc]init]){

    }
    ~AdCtl_platform_private(){
        //[m_controller release];
    }
    AdctlViewController* m_controller;
    QString m_startAdId;
};

AdCtl_platform::AdCtl_platform()
{
    d=new AdCtl_platform_private;
}

AdCtl_platform::~AdCtl_platform(){
    delete d;
}

void AdCtl_platform::initStartAd(){
    NSString* str = [NSString stringWithUTF8String:d->m_startAdId.toUtf8().constData()];
    [d->m_controller initStartAd:str];
}

void AdCtl_platform::setStartAdId(const QString& id){
    d->m_startAdId=id;
}

void AdCtl_platform::setStartAdBannerSize(const int width, const int height){
    [d->m_controller setSize:width :height];
}

void AdCtl_platform::setStartAdBannerPosition(const int x, const int y){
    [d->m_controller setPos:x :y];
}

int AdCtl_platform::startAdBannerHeight() const{
    return [d->m_controller getHeight];
}

int AdCtl_platform::startAdBannerWidth() const{
    return [d->m_controller getWidth];
}

int AdCtl_platform::startAdBannerX() const{
    return [d->m_controller getX];
}

int AdCtl_platform::startAdBannerY() const{
    return [d->m_controller getY];
}

void AdCtl_platform::showStartAd(){
    [d->m_controller setViewVisible:true];
}

void AdCtl_platform::hideStartAd(){
    [d->m_controller setViewVisible:false];
}

