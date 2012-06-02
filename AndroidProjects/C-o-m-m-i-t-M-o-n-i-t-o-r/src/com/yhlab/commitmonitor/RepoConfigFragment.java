package com.yhlab.commitmonitor;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

public class RepoConfigFragment extends Fragment {

    static final String TAG = "RepoConfigFragment";

    @Override
    public void onCreate(Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreate");

        super.onCreate(savedInstanceState);
        this.setHasOptionsMenu(true);

        Log.v(TAG, "<<< onCreate");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreateView");

        View v = inflater.inflate(R.layout.repoconfig_fragment, container,
                false);
        // View v = super.onCreateView(inflater, container, savedInstanceState);

        Log.v(TAG, "<<< onCreateView");

        return v;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.repoconfig_optmenu, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.optitem_add_repo_sample:
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }
}
