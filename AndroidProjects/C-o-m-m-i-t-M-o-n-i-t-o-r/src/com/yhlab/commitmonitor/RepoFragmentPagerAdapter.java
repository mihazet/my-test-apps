package com.yhlab.commitmonitor;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;

public class RepoFragmentPagerAdapter extends FragmentPagerAdapter {

    final static String TAG = "RepoFragmentPagerAdapter";

    public RepoFragmentPagerAdapter(FragmentManager fragMgr) {
        super(fragMgr);
    }

    @Override
    public Fragment getItem(int arg0) {
        Log.d(TAG, "getItem" + arg0);

        switch (arg0) {
        case 0:
            return new RepoConfigFragment();
        case 1:
            return new RepoDetailFragment();
        default:
            Log.e(TAG, "Don't have so many fragment.");
            return null;
        }
    }

    @Override
    public int getCount() {
        return 2;
    }

}
