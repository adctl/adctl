package ru.forsk.AdCtl;

import org.dreamdev.QtAdMob.QtAdMobActivity;
import android.view.View;
import android.view.ViewGroup;
import android.util.Log;
import com.startad.lib.SADView;
import com.google.android.gms.ads.AdView;
import android.widget.FrameLayout;

public class AdCtlActivity extends QtAdMobActivity
{
    protected String m_StartAdId;
    protected SADView m_StartAdBannerView;
    protected boolean m_StartAdBannerShowed = false;
    protected int m_StatusBarHeight = 0;
    protected ViewGroup m_ViewGroup;
    protected AdView m_AdBannerView;

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

    public void SetStartAdBannerPosition(final int x, final int y)
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
               FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                    FrameLayout.LayoutParams.WRAP_CONTENT);

               m_StartAdBannerView.setLayoutParams(layoutParams);
               m_StartAdBannerView.setX(x);
               m_StartAdBannerView.setY(y + m_StatusBarHeight);
            }
        });
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

                    FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                         FrameLayout.LayoutParams.WRAP_CONTENT);

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
}
