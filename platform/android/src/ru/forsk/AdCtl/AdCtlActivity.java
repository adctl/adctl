package ru.forsk.AdCtl;

import org.dreamdev.QtAdMob.QtAdMobActivity;
import android.view.View;
import android.view.ViewGroup;
import android.util.Log;

import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.games.leaderboard.LeaderboardScore;
import com.google.android.gms.games.leaderboard.Leaderboards;

import com.startad.lib.SADView;
import com.google.android.gms.ads.AdView;
import android.widget.FrameLayout;
import com.google.example.games.basegameutils.BaseGameUtils;
import com.google.example.games.basegameutils.GameHelper;
import android.os.Bundle;
import android.content.Intent;
import android.app.Activity;
//import com.google.android.gms.common.api.GoogleApiClient;
//import android.support.v4.app.FragmentActivity;
import com.google.android.gms.games.Games;

public class AdCtlActivity extends QtAdMobActivity implements GameHelper.GameHelperListener
{
    protected String m_StartAdId;
    protected SADView m_StartAdBannerView;
    protected boolean m_StartAdBannerShowed = false;
    protected int m_StatusBarHeight = 0;
    protected ViewGroup m_ViewGroup;
    protected AdView m_AdBannerView;
    private GameHelper gameHelper;

    protected int m_startAdWidth = 0;
    protected int m_startAdHeight = 0;

    private boolean mIsSignedIn = false;

    //StartAd and AdMob
    protected int GetStatusBarHeight()
    {
        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    public void SetStartAdId(final String StartAdId)
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                m_StartAdId = StartAdId;
            }
        });
    }

    protected FrameLayout.LayoutParams getLayoutParams()
    {
        Log.d("getLayoutParams", ""+m_startAdWidth);
        FrameLayout.LayoutParams layoutParams;
        if (m_startAdWidth == 0) {
            layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                        FrameLayout.LayoutParams.WRAP_CONTENT);
        } else {
            layoutParams = new FrameLayout.LayoutParams(m_startAdWidth,
                                                        FrameLayout.LayoutParams.WRAP_CONTENT);
        }
        return layoutParams;
    }

    public void SetStartAdBannerPosition(final int x, final int y)
    {
        Log.d("SetStartAdBannerPosition", ""+m_startAdWidth);
        runOnUiThread(new Runnable()
        {
            public void run()
            {
               FrameLayout.LayoutParams layoutParams = getLayoutParams();

               m_StartAdBannerView.setLayoutParams(layoutParams);
               m_StartAdBannerView.setX(x);
               m_StartAdBannerView.setY(y);
            }
        });
    }

    public void SetStartAdBannerSize(final int width, final int height)
    {
        m_startAdWidth = width;
        m_startAdHeight = height;
        Log.d("SetStartAdBannerSize", ""+m_startAdWidth);
//        runOnUiThread(new Runnable()
//        {
//            public void run()
//            {
//                m_startAdWidth = width;
//                m_startAdHeight = height;
//                FrameLayout.LayoutParams layoutParams = getLayoutParams();

//                m_StartAdBannerView.setLayoutParams(layoutParams);
//            }
//        });
    }

    public boolean IsStartAdBannerShowed()
    {
        return m_StartAdBannerShowed;
    }

    public int GetStartAdBannerWidth()
    {
        return m_StartAdBannerView.getWidth();
    }

    public int GetStartAdBannerHeight()
    {
        return m_StartAdBannerView.getHeight();
    }

    public void InitializeStartAdBanner()
    {
        final QtAdMobActivity self = this;
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                if (m_StartAdBannerView != null)
                {
                    return;
                }

                m_StatusBarHeight = GetStatusBarHeight();

                m_StartAdBannerView = new SADView(self, m_StartAdId);

                View view = getWindow().getDecorView().getRootView();
                if (view instanceof ViewGroup)
                {
                    m_ViewGroup = (ViewGroup) view;

                    FrameLayout.LayoutParams layoutParams = getLayoutParams();

                    m_StartAdBannerView.setLayoutParams(layoutParams);
                    m_StartAdBannerView.setX(0);
                    m_StartAdBannerView.setY(-300);
                    m_ViewGroup.addView(m_StartAdBannerView);

                    m_StartAdBannerView.setAdListener(new SADView.SADListener() {
                        @Override
                        public void onReceiveAd() {
                            Log.d("SADView", "SADListener onReceiveId");
                        }

                        @Override
                        public void onShowedAd() {
                            Log.d("SADView", "SADListener onShowedAd");
                            m_StartAdBannerShowed = true;
                        }

                        @Override
                        public void onError(SADView.ErrorCode error) {
                            Log.d("SADView", "SADListener onError " + error);
                        }

                        @Override
                        public void onAdClicked() {
                            Log.d("SADView", "SADListener onAdClicked");
                        }

                        @Override
                        public void noAdFound() {
                            Log.d("SADView", "SADListener noAdFound");
                        }
                    });

                    //Load ad for currently active language in app
                    m_StartAdBannerView.loadAd(SADView.LANGUAGE_RU); //or this.sadView.loadAd(SADView.LANGUAGE_RU);
                }
            }
        });
    }

    public void ShowStartAdBanner()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (IsStartAdBannerShowed())
                {
                    return;
                }

                m_StartAdBannerView.setVisibility(View.VISIBLE);
                m_StartAdBannerShowed = true;
            }
        });
    }

    public void HideStartAdBanner()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (!IsStartAdBannerShowed())
                {
                    return;
                }

                m_StartAdBannerView.setVisibility(View.GONE);
                m_StartAdBannerShowed = false;
            }
        });
    }

    public int GetAdMobBannerX()
    {
        if(m_AdBannerView != null){
            return Math.round(m_AdBannerView.getX());
        }else{
            return 0;
        }
    }

    public int GetAdMobBannerY()
    {
        if(m_AdBannerView != null){
            return Math.round(m_AdBannerView.getY());
        }else{
            return 0;
        }
    }

    public int GetStartAdBannerX()
    {
        if(m_StartAdBannerView!=null){
            return Math.round(m_StartAdBannerView.getX());
        }else{
            return 0;
        }
    }

    public int GetStartAdBannerY()
    {
        if(m_StartAdBannerView!=null){
            return Math.round(m_StartAdBannerView.getY());
        }else{
            return 0;
        }
    }


    //Google Play Game services
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        gameHelper = new GameHelper(this, GameHelper.CLIENT_ALL);
//        gameHelper.setConnectOnStart(false);
//        gameHelper.enableDebugLog(true);
//        //getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//        gameHelper.setup(this);
//        Log.i("ADCDADSAD","onCreate");

         // Set a callback function for the C++ code to invoke after an
         // authorization event ends.
         // This should be the String name of a public, void, non-static method
         // in this class that
         // takes a single boolean as an argument.
         setAuthResultCallback("onAuthActionFinished");

         // Initialize the Google Play Games C++ SDK. This will set up the proper
         // Android platform
         // configuration and make an initial attempt to sign in without user
         // interaction.
         nativeOnActivityCreated(this, savedInstanceState);
    }

    @Override
    public void onStart() {
        super.onStart();
//        gameHelper.onStart(this);
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onStop() {
        super.onStop();
//        gameHelper.onStop();
    }

    @Override
    public void onDestroy() {
      if(m_StartAdBannerView != null) {
         m_StartAdBannerView.destroy();
      }
      super.onDestroy();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.i("GPGS DEBUGG","onActivityResult");
        super.onActivityResult(requestCode, resultCode, data);
        // здесь gameHelper принимает решение о подключении, переподключении или
        // отключении от игровых сервисов, в зависимости от кода результата
        // Activity
        // Yeah i get all this Russian, VODKA MY FRIEND
        try{
//            gameHelper.onActivityResult(requestCode, resultCode, data);
        }catch(Exception e){
            e.printStackTrace();
        }
        nativeOnActivityResult(this, requestCode, resultCode, data);
    }

    public boolean getSignedInGPGS() {
        // статус подключения
        return gameHelper.isSignedIn();
    }

    public void loginGPGS() {
        try {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
//                    gameHelper.beginUserInitiatedSignIn();
                }
            });
        } catch (Exception e) {
                e.printStackTrace();
        }
    }
    public int getScoreGPGS(final String leaderboardId){
           PendingResult<Leaderboards.LoadPlayerScoreResult> l_score =
                   Games.Leaderboards.loadCurrentPlayerLeaderboardScore(gameHelper.getApiClient(),leaderboardId,0,0);
           int scorei = 0;
           l_score.setResultCallback(new ResultCallback<Leaderboards.LoadPlayerScoreResult>() {
               @Override
               public void onResult(Leaderboards.LoadPlayerScoreResult loadPlayerScoreResult) {
                   Status status = loadPlayerScoreResult.getStatus();
                   LeaderboardScore score = loadPlayerScoreResult.getScore();
                   Log.d("","sd"+status.getStatusMessage());
               }
           });
           return scorei;
       }

    public void submitScoreGPGS(final String leaderBoardId, int score) {
        Games.Leaderboards.submitScore(gameHelper.getApiClient(),
            leaderBoardId, score);
    }

    public void unlockAchievementGPGS(String achievementId) {
        Games.Achievements.unlock(gameHelper.getApiClient(), achievementId);
    }

    public void getLeaderboardGPGS() {
        startActivityForResult(
            Games.Leaderboards.getAllLeaderboardsIntent(gameHelper.getApiClient()), 100);
    }

    public void getAchievementsGPGS() {
        startActivityForResult(
            Games.Achievements.getAchievementsIntent(gameHelper.getApiClient()), 101);
    }

    @Override
    public void onSignInSucceeded() {
    }

    @Override
    public void onSignInFailed() {
    }

    public void onAuthActionFinished(boolean success) {
            Log.i("TAG", "onAuthActionFinished: " + String.valueOf(success));
            mIsSignedIn = success;

            // Need to make sure to run this on the UI thread since the C++ SDK may
            // invoke this
            // callback from a separate thread.
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    //setButtonState();
                }
            });
        }

    public native void nativeOnActivityCreated(Activity activity,Bundle savedInstanceState);

    public native void setAuthResultCallback(String method);

    public native void nativeOnActivityResult(Activity activity,int request_code, int result_code, Intent result);
}
