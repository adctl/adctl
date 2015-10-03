package ru.forsk.AdCtl;

import org.dreamdev.QtAdMob.QtAdMobActivity;
import android.view.View;
import android.view.ViewGroup;
import android.util.Log;
import com.startad.lib.SADView;
import com.google.android.gms.ads.AdView;
import android.widget.FrameLayout;
import com.google.example.games.basegameutils.BaseGameUtils;
import com.google.example.games.basegameutils.GameHelper;
import android.os.Bundle;
import android.content.Intent;
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
        return Math.round(m_AdBannerView.getX());
    }

    public int GetAdMobBannerY()
    {
        return Math.round(m_AdBannerView.getY());
    }

    public int GetStartAdBannerX()
    {
        return Math.round(m_StartAdBannerView.getX());
    }

    public int GetStartAdBannerY()
    {
        return Math.round(m_StartAdBannerView.getY());
    }

    @Override
    public void onDestroy() {
      if (m_StartAdBannerView != null) {
        m_StartAdBannerView.destroy();
      }
      super.onDestroy();
    }

    //Google Play Game services
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        gameHelper = new GameHelper(this, GameHelper.CLIENT_ALL);
        gameHelper.setConnectOnStart(false);
        gameHelper.enableDebugLog(true);
        //getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        gameHelper.setup(this);
    }

    protected void onStart() {
        super.onStart();
        gameHelper.onStart(this);
    }

    protected void onStop() {
        super.onStop();
        gameHelper.onStop();
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        // здесь gameHelper принимает решение о подключении, переподключении или
        // отключении от игровых сервисов, в зависимости от кода результата
        // Activity
        gameHelper.onActivityResult(requestCode, resultCode, data);
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
                    gameHelper.beginUserInitiatedSignIn();
                }
            });
        } catch (Exception e) {
                e.printStackTrace();
        }
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
}
