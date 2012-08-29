package com.yhlab.guessnumberhelper.lite;

import android.os.Bundle;
import com.actionbarsherlock.app.SherlockPreferenceActivity;

public class SettingsActivity extends SherlockPreferenceActivity {
    
    public static final String KEY_DIGIT_COUNT = "prefs_digitCount";
    public static final String KEY_GUESS_METHOD = "prefs_guessMethod";

    @SuppressWarnings("deprecation")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);
    }    
}
