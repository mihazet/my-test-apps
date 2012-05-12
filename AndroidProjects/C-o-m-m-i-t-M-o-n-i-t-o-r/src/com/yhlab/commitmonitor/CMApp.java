package com.yhlab.commitmonitor;

import android.app.Application;
import android.util.Log;

public class CMApp extends Application {

    private final static String TAG = "CMApp";

    @Override
    public void onCreate() {

        Log.d(TAG, ">>> onCreate");

        super.onCreate();

        Log.d(TAG, "<<< onCreate");
    }

    @Override
    public void onTerminate() {

        Log.d(TAG, ">>> onTerminate");

        super.onTerminate();

        Log.d(TAG, "<<< onTerminate");
    }
}
