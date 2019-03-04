#include "AdCtl_platform.h"
#include <QString>
#include <QVariant>
#include <QDebug>
#include "StateManager.h"
#include "utils.h"
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include "SADWebView.h"
#import <GoogleSignIn/GoogleSignIn.h>
#import <gpg/GooglePlayGames.h>
#import <GooglePlus/GooglePlus.h>
#import <QuartzCore/QuartzCore.h>

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

/**
 * Workaround to register the delegate methods needed for google authorization.
 */
@interface QIOSApplicationDelegate : UIResponder <UIApplicationDelegate>
    - (void)applicationDidFinishLaunching:(UIApplication *)application;
    -(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;
@end

@interface QAppDelegate

@end

@implementation QIOSApplicationDelegate (QAppDelegate)

-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation{
    qDebug("FINALLLYYYY");
//    [GIDSignIn sharedInstance].uiDelegate = self;
    return [GPPURLHandler handleURL:url sourceApplication:sourceApplication annotation:annotation];
}
-(void)applicationDidFinishLaunching:(UIApplication *)application{
    qDebug("FINALLLYYYY");
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    return YES;
}
@end

/**
 * Controller for everything specially GoogleGameStuff
 */
@interface AdctlController : UIViewController

@end

@interface QIOSViewController : UIViewController<GIDSignInUIDelegate>
- (void)viewDidLoad;
//- (void)signIn:(GIDSignIn *)signIn presentViewController:(UIViewController *)viewController;

// If implemented, this method will be invoked when sign in needs to dismiss a view controller.
// Typically, this should be implemented by calling |dismissViewController| on the passed
// view controller.
//- (void)signIn:(GIDSignIn *)signIn dismissViewController:(UIViewController *)viewController;
@end


//static NSString * const kClientID = @"37589320792";
static NSString * const kClientID = @"37589320792-s3j8h5juha4rl90q8v977c067em88s14.apps.googleusercontent.com";
@implementation QIOSViewController (AdctlController)

-(void) viewDidLoad
{
    [super viewDidLoad];
    
    // Configure platform configuration for iOS
    gpg::IosPlatformConfiguration config;
    config.SetClientID(std::string([kClientID UTF8String]));
    config.SetOptionalViewControllerForPopups(self);
    bool valid = config.Valid();
    
    StateManager::InitServices(config,
                               [self](gpg::AuthOperation op) {
                                   [self onAuthActionStarted:op];
                               },
                               [self](gpg::AuthOperation op, gpg::AuthStatus status) {
                                   [self onAuthActionFinished:op withStatus:status];
                               });
    [GIDSignIn sharedInstance].uiDelegate = self;

    NSLog(@"Looking good QIOSViewController");
    NSLog(@"ClientID %@",kClientID);
}
//- (void)signIn:(GIDSignIn *)signIn presentViewController:(UIViewController *)viewController{}

// If implemented, this method will be invoked when sign in needs to dismiss a view controller.
// Typically, this should be implemented by calling |dismissViewController| on the passed
// view controller.
//- (void)signIn:(GIDSignIn *)signIn dismissViewController:(UIViewController *)viewController{}


-(void) onAuthActionStarted:(gpg::AuthOperation) op{
    NSLog(@"Auth Action Started");
}
-(void) onAuthActionFinished:(gpg::AuthOperation)op withStatus:(gpg::AuthStatus) status{
    NSLog(@"Auth Action finished");
    dispatch_async(dispatch_get_main_queue(), ^{
        //Something here
    });
    if(gpg::IsSuccess(status)){
        NSLog(@"Status: Success %i",status);
    }else{
        NSLog(@"Status: failure %i",status);
    }
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

AdCtl_platform::AdCtl_platform(){
    d=new AdCtl_platform_private;
}

AdCtl_platform::~AdCtl_platform(){
    delete d;
}

void AdCtl_platform::init()
{

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

bool AdCtl_platform::isGPGSSignedIn() const {
    return StateManager::GetGameServices()->IsAuthorized();
}

void AdCtl_platform::signInGPGS() {
    StateManager::BeginUserInitiatedSignIn();
}

void AdCtl_platform::getLeaderBoardScore(const QString& leaderboardId)
{
    gpg::LeaderboardManager::FetchScoreSummaryCallback
            callback = [&](const gpg::LeaderboardManager::FetchScoreSummaryResponse& response)
    {
        uint64_t score;
        QString lID;
        int status = static_cast<int>(response.status);
        if(response.status == gpg::ResponseStatus::VALID ||
                response.status == gpg::ResponseStatus::VALID_BUT_STALE){
            score = response.data.CurrentPlayerScore().Value();
            lID = QString(response.data.LeaderboardId().c_str());
        }
        if(m_adctl_obj) {

            if(Util::javaScriptFuncExists(m_adctl_obj,"scoreGPG")){
                QMetaObject::invokeMethod(m_adctl_obj, "scoreGPG", Qt::DirectConnection,
                           Q_ARG(QVariant, QVariant(lID)),
                           Q_ARG(QVariant, QVariant(score)),
                           Q_ARG(QVariant, QVariant(status)));
            }

        }
    };
    StateManager::GetGameServices()->Leaderboards().FetchScoreSummary(leaderboardId.toStdString(),gpg::LeaderboardTimeSpan::ALL_TIME,
                                                                      gpg::LeaderboardCollection::PUBLIC,callback);
}

void AdCtl_platform::submitScoreGPGS(const QString& leaderBoardId, int score){
    StateManager::SubmitHighScore(leaderBoardId.toStdString().c_str(),score);
}

void AdCtl_platform::unlockAchievementGPGS(const QString& achievementId){
    StateManager::UnlockAchievement(achievementId.toStdString().c_str());
}

void AdCtl_platform::showLeaderboard(const QString& leaderboardId){
    StateManager::GetGameServices()->Leaderboards().ShowUI(leaderboardId.toStdString(),[](gpg::UIStatus const &){});
}

void AdCtl_platform::showLeaderboardGPGS(){
    StateManager::ShowLeaderboards();
}
void AdCtl_platform::showAchievementsGPGS(){
    StateManager::GetGameServices()->Achievements().ShowAllUI([](const gpg::UIStatus& status){

    });
}

void AdCtl_platform::shareImage(QString path){
//    NSString *textToShare = @"Enter your text to be shared";
//    UIImage * image = [UIImage imageNamed:@"imagename"];

//    NSArray *objectsToShare = @[textToShare, image];

//    UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:objectsToShare applicationActivities:nil];


//    [self presentViewController:controller animated:YES completion:nil];
}

