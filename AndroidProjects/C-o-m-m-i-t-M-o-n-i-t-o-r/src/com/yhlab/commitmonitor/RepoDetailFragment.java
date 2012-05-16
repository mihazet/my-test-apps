package com.yhlab.commitmonitor;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class RepoDetailFragment extends Fragment {

    static final String TAG = "RepoDetailFragment";

    @Override
    public void onAttach(Activity activity) {

        Log.v(TAG, ">>> onAttach");

        super.onAttach(activity);

        Log.v(TAG, "<<< onAttach");
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreate");

        super.onCreate(savedInstanceState);

        Log.v(TAG, "<<< onCreate");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreateView");

        View v = inflater.inflate(R.layout.repodetail_fragment, container,
                false);
        // View v = super.onCreateView(inflater, container, savedInstanceState);

        Log.v(TAG, "<<< onCreateView");

        return v;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {

        Log.v(TAG, ">>> onActivityCreated");

        super.onActivityCreated(savedInstanceState);

        Log.v(TAG, "<<< onActivityCreated");
    }

    @Override
    public void onStart() {

        Log.v(TAG, ">>> onStart");

        super.onStart();

        Log.v(TAG, "<<< onStart");
    }

    @Override
    public void onResume() {

        Log.v(TAG, ">>> onResume");

        super.onResume();

        Log.v(TAG, "<<< onResume");
    }

    @Override
    public void onPause() {

        Log.v(TAG, ">>> onPause");

        super.onPause();

        Log.v(TAG, "<<< onPause");
    }

    @Override
    public void onStop() {

        Log.v(TAG, ">>> onStop");

        super.onStop();

        Log.v(TAG, "<<< onStop");
    }

    @Override
    public void onDestroyView() {

        Log.v(TAG, ">>> onDestroyView");

        super.onDestroyView();

        Log.v(TAG, "<<< onDestroyView");
    }

    @Override
    public void onDestroy() {

        Log.v(TAG, ">>> onDestroy");

        super.onDestroy();

        Log.v(TAG, "<<< onDestroy");
    }

    @Override
    public void onDetach() {

        Log.v(TAG, ">>> onDetach");

        super.onDetach();

        Log.v(TAG, "<<< onDetach");
    }
}
