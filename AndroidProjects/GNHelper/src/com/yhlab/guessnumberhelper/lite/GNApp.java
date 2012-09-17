package com.yhlab.guessnumberhelper.lite;

import android.app.Application;
import android.content.SharedPreferences;
import android.graphics.Rect;
import android.preference.PreferenceManager;
import com.yhlab.guessnumberhelper.guess.ChooserFactory;
import com.yhlab.guessnumberhelper.guess.Game;
import com.yhlab.guessnumberhelper.guess.IGuessChooser;
import com.yhlab.guessnumberhelper.guess.ScoredChooser;

public class GNApp extends Application {

    @SuppressWarnings("unused")
    private static final String TAG = "App";

    public ChooserFactory factory;
    public Game game;
    public int nexGuess;
    
    /* position of view used by the help activity */
    public Rect rectRestartGame = null;
    public Rect rectAddResult = null;
    public Rect rectAppArea = null;
    public Rect rectGuessNumber = null;
    public Rect rectGuessResult = null;

    @Override
    public void onCreate() {

        super.onCreate();

        /* setup preference default values */
        PreferenceManager.setDefaultValues(this, R.xml.preferences, false);

    }

    public void InitGame(int digitCount, String method) {

        SharedPreferences sp = PreferenceManager
                .getDefaultSharedPreferences(this);

        /* retrieve game parameters if not specific */
        if (digitCount == -1) {
            String d = sp.getString(SettingsActivity.KEY_DIGIT_COUNT, null);
            digitCount = Integer.parseInt(d);
        }

        if (method == null) {
            method = sp.getString(SettingsActivity.KEY_GUESS_METHOD, null);
        }

        factory = ChooserFactory.getInstance();
        IGuessChooser chooser = factory.createChooser(method, null);
        if (chooser instanceof ScoredChooser) {
            ((ScoredChooser) chooser).setTimeout(5000);
        }
        game = new Game(digitCount, 10, chooser);

    }
}
