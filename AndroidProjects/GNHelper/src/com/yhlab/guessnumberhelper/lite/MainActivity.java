package com.yhlab.guessnumberhelper.lite;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.FragmentManager;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.style.ForegroundColorSpan;
import android.widget.Toast;
import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuItem;
import com.yhlab.guessnumberhelper.guess.Game;

public class MainActivity extends SherlockFragmentActivity implements
        NumberFragment.IGuessedListener, OnSharedPreferenceChangeListener {

    @SuppressWarnings("unused")
    private static final String TAG = "MainActivity";
    private SharedPreferences mSharedPrefs;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        /*
         * register sharedPreferences changed event. If event happened at the
         * time between event unregister and register, the event handler will
         * not be fired. In our case, because the sharedPreferences is changed
         * within another activity which was invoked by this main activity,
         * don't register and unregister event in the onResume and onPause
         * callback method.
         */
        mSharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        mSharedPrefs.registerOnSharedPreferenceChangeListener(this);

        // getSupportActionBar().setDisplayShowTitleEnabled(false);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        /* unregister sharedPreferences changed event */
        mSharedPrefs.unregisterOnSharedPreferenceChangeListener(this);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getSupportMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {
        case R.id.menu_settings:
            startActivity(new Intent(this, SettingsActivity.class));
            return true;
        case R.id.menu_restart:

            GNApp app = (GNApp) getApplication();
            app.InitGame(-1, null);
            int firstGuess = app.game.restart();

            FragmentManager fm = getSupportFragmentManager();
            NumberFragment nf = (NumberFragment) fm
                    .findFragmentById(R.id.number_fragment);
            nf.enableAddResult(true);
            nf.setGuessNumber(firstGuess);
            nf.resetResult();

            HistoryFragment hf = (HistoryFragment) fm
                    .findFragmentById(R.id.history_fragment);
            hf.clearHistory();

            return true;

        case R.id.menu_help:

            startActivity(new Intent(this, HelpActivity.class));
            return true;

        case R.id.menu_about:

            startActivity(new Intent(this, AboutActivity.class));
            return true;

        default:
            return super.onOptionsItemSelected(item);
        }
    }

    @Override
    public void onGuess(int number, int result) {
        FragmentManager fm = getSupportFragmentManager();
        HistoryFragment hf = (HistoryFragment) fm
                .findFragmentById(R.id.history_fragment);
        hf.addHistory(number, result);

        GNApp app = (GNApp) getApplication();
        int candidate = app.game.setGuessLabel(number, result);

        switch (candidate) {
        case Game.CANDIDATE_ONE:
        case Game.CANDIDATE_MORE:
            int nextGuess = app.game.nextGuess();
            NumberFragment nf = (NumberFragment) fm
                    .findFragmentById(R.id.number_fragment);
            nf.setGuessNumber(nextGuess);

            if (candidate == Game.CANDIDATE_ONE) {
                CharSequence cs1 = getString(R.string.notify_congra);
                int cs1len = cs1.length();
                CharSequence cs2 = String.format(" %04X", nextGuess);
                SpannableString msg =
                        new SpannableString(TextUtils.concat(cs1, cs2));
                msg.setSpan(new ForegroundColorSpan(0xFFFF0000),
                        cs1len + 1, cs1len + 5,
                        Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                Toast.makeText(getApplicationContext(),
                        msg, Toast.LENGTH_LONG).show();
            }
            break;

        case Game.CANDIDATE_MORE_LUCKY_ONE:
            Toast.makeText(getApplicationContext(),
                    getString(R.string.notify_congra_lucky),
                    Toast.LENGTH_SHORT).show();            
            break;

        case Game.CANDIDATE_ZERO:
            Toast.makeText(getApplicationContext(),
                    getString(R.string.notify_nosuchnumber),
                    Toast.LENGTH_LONG).show();
            break;
        }

    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sp, String k) {

        if (k.equals(SettingsActivity.KEY_DIGIT_COUNT)) {

            FragmentManager fm = getSupportFragmentManager();
            NumberFragment nf = (NumberFragment) fm
                    .findFragmentById(R.id.number_fragment);
            nf.changeDigitCount(Integer.parseInt(sp.getString(k, null)));

        } else if (k.equals(SettingsActivity.KEY_GUESS_METHOD)) {

        }
    }
}
