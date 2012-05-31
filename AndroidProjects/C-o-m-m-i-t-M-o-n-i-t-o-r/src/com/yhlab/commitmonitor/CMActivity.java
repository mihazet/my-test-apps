package com.yhlab.commitmonitor;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.widget.LinearLayout;

import com.google.ads.AdRequest;
import com.google.ads.AdSize;
import com.google.ads.AdView;

public class CMActivity extends FragmentActivity implements
        FirstUseFragment.INextButtonClickListener {

    static final String TAG = "CMActivity";
    static final String TAG_1ST_USE_FRAGMENT = "firstuse";
    static final String TAG_REPO_EDIT_FRAGMENT = "repoedit";
    static final String KEY_1STUSE = "1stuse";
    static final boolean KEY_1STUSE_DEF = true;

    /* activity-specific shared preferences */
    private SharedPreferences mPrefs;
    private Boolean m1stUse;

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

        if (savedInstanceState != null)
            Log.v(TAG, ">>> onCreate, not null");
        else
            Log.v(TAG, ">>> onCreate");

        super.onCreate(savedInstanceState);

        /* check shared preference for using difference layout */
        mPrefs = getPreferences(MODE_PRIVATE);
        m1stUse = mPrefs.getBoolean(KEY_1STUSE, KEY_1STUSE_DEF);

        if (m1stUse) {
            Log.d(TAG, "Use 1st use layout");
            setContentView(R.layout.firstuse);
            FragmentManager fm = getSupportFragmentManager();
            FragmentTransaction ft = fm.beginTransaction();

            Fragment f = fm.findFragmentByTag(TAG_1ST_USE_FRAGMENT);
            if (f == null && savedInstanceState == null) {
                Log.d(TAG, "fragment not found");
                f = new FirstUseFragment();
                ft.add(R.id.Layout1stUse, f, TAG_1ST_USE_FRAGMENT);
            }
            ft.commit();
        } else {
            Log.d(TAG, "Use regular layout");
            setContentView(R.layout.main);
        }

        mViewPager = (ViewPager) findViewById(R.id.viewpager_repo);
        if (mViewPager != null) { // viewpager is unavailable in phone.
            mPagerAdapter = new RepoFragmentPagerAdapter(
                    getSupportFragmentManager());
            mViewPager.setAdapter(mPagerAdapter);
        }

        /* Determinate ad size and then load ad */
        if (null != (adLayout = (LinearLayout) findViewById(R.id.adLayoutIABLeaderBoard))) {
            adSize = AdSize.IAB_LEADERBOARD;
            minHeight = 90;
            minWidth = 728;
        } else if (null != (adLayout = (LinearLayout) findViewById(R.id.adLayoutIABBanner))) {
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

        Log.v(TAG, "<<< onCreate");
    }

    @Override
    protected void onRestart() {

        Log.v(TAG, ">>> onRestart");

        super.onRestart();

        Log.v(TAG, "<<< onRestart");
    }

    @Override
    protected void onStart() {

        Log.v(TAG, ">>> onSart");

        super.onStart();

        Log.v(TAG, "<<< onStart");
    }

    @Override
    protected void onResume() {

        Log.v(TAG, ">>> onResume");

        super.onResume();

        Log.v(TAG, "<<< onResume");
    }

    @Override
    protected void onPause() {

        Log.v(TAG, ">>> onPause");

        super.onPause();

        Log.v(TAG, "<<< onPause");
    }

    @Override
    protected void onStop() {

        Log.v(TAG, ">>> onStop");

        super.onStop();

        Log.v(TAG, "<<< onStop");
    }

    @Override
    protected void onDestroy() {

        Log.v(TAG, ">>> onDestroy");

        /* remove ad from layout first or crash */
        if (adAvailable) {
            adLayout.removeView(adView);
            adView.destroy();
        }

        super.onDestroy();

        Log.v(TAG, "<<< onDestroy");
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {

        Log.v(TAG, ">>> onSaveInstanceState");

        super.onSaveInstanceState(outState);

        Log.v(TAG, "<<< onSaveInstanceState");
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {

        Log.v(TAG, ">>> onRestoreInstanceState");

        super.onRestoreInstanceState(savedInstanceState);

        Log.v(TAG, "<<< onRestoreInstanceState");
    }

    public void onNextButtonClick() {
        FragmentManager fm = getSupportFragmentManager();
        FragmentTransaction ft = fm.beginTransaction();

        Fragment f = fm.findFragmentByTag(TAG_REPO_EDIT_FRAGMENT);
        if (f == null) {
            f = new RepoConfigFragment();
        }
        ft.replace(R.id.Layout1stUse, f, TAG_REPO_EDIT_FRAGMENT);
        ft.addToBackStack(null);
        ft.commit();
    }
}
