package com.yhlab.commitmonitor;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class FirstUseFragment extends Fragment {
    private static final String TAG = "FirstUseFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreateView");

        View v = inflater.inflate(R.layout.firstuse_fragment, container, false);

        Log.v(TAG, "<<< onCreateView");
        return v;
    }
}
