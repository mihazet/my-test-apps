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

        Fragment f;

        Log.d(TAG, ">>> getItem " + arg0);

        switch (arg0) {
        case 0:
            f = new RepoConfigFragment();
            break;
        case 1:
            f = new RepoDetailFragment();
            break;
        default:
            f = null;
        }

        Log.d(TAG, "<<< getItem");

        return f;
    }

    @Override
    public int getCount() {

        Log.d(TAG, ">>> getCount");

        Log.d(TAG, "<<< getCount");

        return 2;
    }

}
