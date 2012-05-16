package com.yhlab.commitmonitor;

import android.app.Application;
import android.util.Log;

public class CMApp extends Application {

    private final static String TAG = "CMApp";

    @Override
    public void onCreate() {

        Log.v(TAG, ">>> onCreate");

        super.onCreate();

        Log.v(TAG, "<<< onCreate");
    }

    @Override
    public void onTerminate() {

        Log.v(TAG, ">>> onTerminate");

        super.onTerminate();

        Log.v(TAG, "<<< onTerminate");
    }
}
