package com.yhlab.guessnumberhelper.lite;

import android.os.Bundle;
import android.preference.ListPreference;
import android.util.DisplayMetrics;
import com.actionbarsherlock.app.SherlockPreferenceActivity;

public class SettingsActivity extends SherlockPreferenceActivity {

    /* preference keys */
    public static final String KEY_DIGIT_COUNT = "prefs_digitCount";
    public static final String KEY_GUESS_METHOD = "prefs_guessMethod";

    /* intent bundle key */
    public static final String INTENT_KEY_WIDTH = "width";

    @SuppressWarnings("deprecation")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);

        /* determinate the max digit count number */
        int wheelWidth;
        DisplayMetrics out = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(out);
        switch (out.densityDpi) {
        case DisplayMetrics.DENSITY_LOW:
            wheelWidth = 24;
            break;
        default:
        case DisplayMetrics.DENSITY_MEDIUM:
            wheelWidth = 34;
            break;
        case DisplayMetrics.DENSITY_HIGH:
            wheelWidth = 41;
            break;
        case DisplayMetrics.DENSITY_XHIGH:
            wheelWidth = 48;
            break;
        }
        int labelWidth = getIntent().getExtras().getInt(INTENT_KEY_WIDTH);
        int digitMax = labelWidth / wheelWidth;
        digitMax = Math.min(digitMax, 8);
        String[] entries = new String[digitMax - 2];
        String[] entryValues = new String[digitMax - 2];
        for (int i = 3; i <= digitMax; i++) {
            entries[i - 3] = "" + i;
            entryValues[i - 3] = "" + i;
        }
        ListPreference pref = (ListPreference) findPreference(KEY_DIGIT_COUNT);
        pref.setEntries(entries);
        pref.setEntryValues(entryValues);
    }
}
