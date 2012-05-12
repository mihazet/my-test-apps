package com.yhlab.commitmonitor;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class RepoConfigFragment extends Fragment {

    static final String TAG = "RepoConfigFragment";

    @Override
    public void onAttach(Activity activity) {

        Log.d(TAG, ">>> onAttach");

        super.onAttach(activity);

        Log.d(TAG, "<<< onAttach");
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        Log.d(TAG, ">>> onCreate");

        super.onCreate(savedInstanceState);

        Log.d(TAG, "<<< onCreate");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        Log.d(TAG, ">>> onCreateView");

        View v = inflater.inflate(R.layout.repoconfig_fragment, container,
                false);
        // View v = super.onCreateView(inflater, container, savedInstanceState);

        Log.d(TAG, "<<< onCreateView");

        return v;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {

        Log.d(TAG, ">>> onActivityCreated");

        super.onActivityCreated(savedInstanceState);

        Log.d(TAG, "<<< onActivityCreated");
    }

    @Override
    public void onStart() {

        Log.d(TAG, ">>> onStart");

        super.onStart();

        Log.d(TAG, "<<< onStart");
    }

    @Override
    public void onResume() {

        Log.d(TAG, ">>> onResume");

        super.onResume();

        Log.d(TAG, "<<< onResume");
    }

    @Override
    public void onPause() {

        Log.d(TAG, ">>> onPause");

        super.onPause();

        Log.d(TAG, "<<< onPause");
    }

    @Override
    public void onStop() {

        Log.d(TAG, ">>> onStop");

        super.onStop();

        Log.d(TAG, "<<< onStop");
    }

    @Override
    public void onDestroyView() {

        Log.d(TAG, ">>> onDestroyView");

        super.onDestroyView();

        Log.d(TAG, "<<< onDestroyView");
    }

    @Override
    public void onDestroy() {

        Log.d(TAG, ">>> onDestroy");

        super.onDestroy();

        Log.d(TAG, "<<< onDestroy");
    }

    @Override
    public void onDetach() {

        Log.d(TAG, ">>> onDetach");

        super.onDetach();

        Log.d(TAG, "<<< onDetach");
    }
}
