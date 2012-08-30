package com.yhlab.guessnumberhelper.lite;

import android.app.Application;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;
import com.yhlab.guessnumberhelper.guess.ChooserFactory;
import com.yhlab.guessnumberhelper.guess.Game;
import com.yhlab.guessnumberhelper.guess.IGuessChooser;
import com.yhlab.guessnumberhelper.guess.ScoredChooser;

public class GNApp extends Application {

    private static final String TAG = "App";

    public ChooserFactory factory;
    public Game game;
    public int nexGuess;

    @Override
    public void onCreate() {

        Log.v(TAG, "Creating...");

        super.onCreate();

        /* setup preference default values */
        PreferenceManager.setDefaultValues(this, R.xml.preferences, false);

        /* retrieve game parameters */
        SharedPreferences sp = PreferenceManager
                .getDefaultSharedPreferences(this);
        String d = sp.getString(SettingsActivity.KEY_DIGIT_COUNT, null);
        int digitCount = Integer.parseInt(d);
        String method = sp.getString(SettingsActivity.KEY_GUESS_METHOD, null);

        /* Game: chooser factory */
        factory = ChooserFactory.getInstance();
        IGuessChooser chooser = factory.createChooser(method, null);
        if (chooser instanceof ScoredChooser) {
            ((ScoredChooser) chooser).setTimeout(5000);
        }
        game = new Game(digitCount, 10, chooser);

        Log.v(TAG, "Created!");
    }
}
