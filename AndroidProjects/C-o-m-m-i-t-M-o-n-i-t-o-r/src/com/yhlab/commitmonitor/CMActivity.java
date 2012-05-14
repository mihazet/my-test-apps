package com.yhlab.commitmonitor;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.widget.LinearLayout;

import com.google.ads.AdRequest;
import com.google.ads.AdSize;
import com.google.ads.AdView;

public class CMActivity extends FragmentActivity {

    static final String TAG = "CMActivity";

    /* for viewpager management used in tablet */
    private RepoFragmentPagerAdapter mPagerAdapter;
    private ViewPager mViewPager;

    /* ad */
    private AdSize adSize;
    private AdView adView;
    private LinearLayout adLayout;
    private boolean adAvailable = true;
    private int minHeight;
    private int minWidth;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {

        Log.d(TAG, ">>> onCreate");

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mViewPager = (ViewPager) findViewById(R.id.viewpager_repo);
        if (mViewPager != null) { // viewpager is unavailable in phone.
            mPagerAdapter = new RepoFragmentPagerAdapter(
                    getSupportFragmentManager());
            mViewPager.setAdapter(mPagerAdapter);
        }

        /* Determinate ad size and then load ad */
        if (null != (adLayout = (LinearLayout) findViewById(R.id.adLayoutIABLeaderBoard))) {
            Log.d(TAG,
                    "land - " + adLayout.getWidth() + ", "
                            + adLayout.getHeight());
            adSize = AdSize.IAB_LEADERBOARD;
            minHeight = 90;
            minWidth = 728;
        } else if (null != (adLayout = (LinearLayout) findViewById(R.id.adLayoutIABBanner))) {
            Log.d(TAG,
                    "port - " + adLayout.getWidth() + ", "
                            + adLayout.getHeight());
            adSize = AdSize.IAB_BANNER;
            minHeight = 60;
            minWidth = 468;
        } else if (null != (adLayout = (LinearLayout) findViewById(R.id.adLayoutBanner))) {
            adSize = AdSize.BANNER;
            minHeight = 50;
            minWidth = 320;
        } else {
            adAvailable = false;
        }

        if (adAvailable) {
            adView = new AdView(this, adSize, "a14fa492f7183ee");
            adView.setMinimumHeight(minHeight);
            adView.setMinimumWidth(minWidth);
            adLayout.addView(adView, 0);
            AdRequest adRequest = new AdRequest();
            // adRequest addTestDevice();
            adRequest.addTestDevice("310E735B8E260B98F50AD7FBAEB83D74");
            adRequest.addTestDevice("7370AF93550EB18283A7EA7D78D020B7");
            adRequest.addTestDevice("45EF6179ECEA9435C3D8731E94E8A16F");
            adView.loadAd(adRequest);
        }

        Log.d(TAG, "<<< onCreate");
    }

    @Override
    protected void onRestart() {

        Log.d(TAG, ">>> onRestart");

        super.onRestart();

        Log.d(TAG, "<<< onRestart");
    }

    @Override
    protected void onStart() {

        Log.d(TAG, ">>> onSart");

        super.onStart();

        Log.d(TAG, "<<< onStart");
    }

    @Override
    protected void onResume() {

        Log.d(TAG, ">>> onResume");

        super.onResume();

        Log.d(TAG, "<<< onResume");
    }

    @Override
    protected void onPause() {

        Log.d(TAG, ">>> onPause");

        super.onPause();

        Log.d(TAG, "<<< onPause");
    }

    @Override
    protected void onStop() {

        Log.d(TAG, ">>> onStop");

        super.onStop();

        Log.d(TAG, "<<< onStop");
    }

    @Override
    protected void onDestroy() {

        Log.d(TAG, ">>> onDestroy");

        /* remove ad from layout first or crash */
        if (adAvailable) {
            adLayout.removeView(adView);
            adView.destroy();
        }

        super.onDestroy();

        Log.d(TAG, "<<< onDestroy");
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {

        Log.d(TAG, ">>> onSaveInstanceState");

        super.onSaveInstanceState(outState);

        Log.d(TAG, "<<< onSaveInstanceState");
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {

        Log.d(TAG, ">>> onRestoreInstanceState");

        super.onRestoreInstanceState(savedInstanceState);

        Log.d(TAG, "<<< onRestoreInstanceState");
    }
}
