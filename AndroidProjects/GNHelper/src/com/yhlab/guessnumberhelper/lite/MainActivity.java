package com.yhlab.guessnumberhelper.lite;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.graphics.Rect;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.FragmentManager;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.style.ForegroundColorSpan;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.Toast;
import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuItem;
import com.actionbarsherlock.view.Window;
import com.yhlab.guessnumberhelper.guess.Game;

public class MainActivity extends SherlockFragmentActivity implements
        NumberFragment.IGuessedListener, OnSharedPreferenceChangeListener {

    private static final String TAG = "MainActivity";
    private SharedPreferences mSharedPrefs;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // This has to be called before setContentView
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);

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
        setSupportProgressBarIndeterminateVisibility(false);
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

        View v;
        Intent intent;

        switch (item.getItemId()) {
        case R.id.menu_settings:
            v = findViewById(R.id.guessed_number);
            intent = new Intent(this, SettingsActivity.class);
            intent.putExtra(SettingsActivity.INTENT_KEY_WIDTH, v.getWidth());
            startActivity(intent);
            return true;

        case R.id.menu_restart:

            new RestartGameTask().execute();
            return true;

        case R.id.menu_intro:

            /*
             * before start help activity, record some view position for the
             * help activity.
             */
            int offset = getStatusbarHeight();
            GNApp app = (GNApp) getApplication();
            app.rectAppArea = getAppArea();

            v = findViewById(R.id.btn_result_add);
            app.rectAddResult = getRectOnScreen(v, offset);

            v = findViewById(R.id.menu_restart);
            app.rectRestartGame = getRectOnScreen(v, offset);

            v = findViewById(R.id.guessed_number);
            app.rectGuessNumber = getRectOnScreen(v, offset);
            fixRectForGuessNumber(app.rectGuessNumber);

            v = findViewById(R.id.guessed_result);
            app.rectGuessResult = getRectOnScreen(v, offset);
            fixRectForGuessResult(app.rectGuessResult);

            startActivity(new Intent(this, IntroActivity.class));

            return true;
            
        case R.id.menu_facebook:
            
            Uri uri = Uri.parse("http://www.facebook.com/GuessNumberHelper");
            intent = new Intent(Intent.ACTION_VIEW, uri);
            startActivity(intent);
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
        new SetLabelAndGenNextGuessTask(number, result).execute();
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

    private class SetLabelAndGenNextGuessTask extends
            AsyncTask<Void, Integer, Void> {

        /* progress index */
        private final int REASON = 0;

        /* reasons index */
        private final int REASON_SHOW_NEXTGUESS = 0;
        private final int REASON_SHOW_TOAST = 1;

        private int num_;
        private int res_;
        private int digitCnt_;

        private NumberFragment nf_;
        private HistoryFragment hf_;

        public SetLabelAndGenNextGuessTask(int num, int res) {
            super();
            num_ = num;
            res_ = res;
            digitCnt_ = Integer.parseInt(mSharedPrefs.getString(
                    SettingsActivity.KEY_DIGIT_COUNT, "4"));
            FragmentManager fm = getSupportFragmentManager();
            nf_ = (NumberFragment) fm.findFragmentById(R.id.number_fragment);
            hf_ = (HistoryFragment) fm.findFragmentById(R.id.history_fragment);
        }

        @Override
        protected void onPreExecute() {

            /* show the indeterminate progress bar */
            setSupportProgressBarIndeterminateVisibility(true);

            /* add current number and result to history */
            hf_.addHistory(num_, res_);

            /* disable button to avoid it is clicked again */
            nf_.enableAddResult(false);

            // TODO: disable number scrolling
        }

        @Override
        protected Void doInBackground(Void... params) {

            GNApp app = (GNApp) getApplication();
            int candidate = app.game.setGuessLabel(num_, res_);

            switch (candidate) {
            case Game.CANDIDATE_ONE:
            case Game.CANDIDATE_MORE:

                int nextGuess = app.game.nextGuess();
                publishProgress(REASON_SHOW_NEXTGUESS, nextGuess);
                publishProgress(REASON_SHOW_TOAST, candidate, nextGuess);
                break;

            case Game.CANDIDATE_MORE_LUCKY_ONE:
            case Game.CANDIDATE_ZERO:
                publishProgress(REASON_SHOW_TOAST, candidate);
                break;

            default:
                break;
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(Integer... values) {

            if (values[REASON] == REASON_SHOW_NEXTGUESS) {

                nf_.setGuessNumber(values[1]);

            } else if (values[REASON] == REASON_SHOW_TOAST) {

                switch (values[1]) {
                case Game.CANDIDATE_ONE:
                    CharSequence cs1 = getString(R.string.notify_congra);
                    int cs1len = cs1.length();
                    String fmt = " %0" + digitCnt_ + "X";
                    CharSequence cs2 = String.format(fmt, values[2]);
                    SpannableString msg =
                            new SpannableString(TextUtils.concat(cs1, cs2));
                    msg.setSpan(new ForegroundColorSpan(0xFFFF0000),
                            cs1len + 1, cs1len + digitCnt_ + 1,
                            Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    Toast.makeText(getApplicationContext(),
                            msg, Toast.LENGTH_LONG).show();
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

                case Game.CANDIDATE_MORE:
                    dbg_show_suggest_number(values[2]);
                    break;

                default:
                    break;
                }
            }
        }

        @Override
        protected void onPostExecute(Void result) {

            /* hide the indeterminate progress bar */
            setSupportProgressBarIndeterminateVisibility(false);

            /* re-enable button for click */
            nf_.enableAddResult(true);

            // TODO: enable number scrolling
        }
    }

    private class RestartGameTask extends AsyncTask<Void, Void, Integer> {

        private NumberFragment nf_;
        private HistoryFragment hf_;

        public RestartGameTask() {
            super();
            FragmentManager fm = getSupportFragmentManager();
            nf_ = (NumberFragment) fm.findFragmentById(R.id.number_fragment);
            hf_ = (HistoryFragment) fm.findFragmentById(R.id.history_fragment);
        }

        @Override
        protected void onPreExecute() {

            /* show the indeterminate progress bar */
            setSupportProgressBarIndeterminateVisibility(true);

            /* clear history */
            hf_.clearHistory();

            /* reset result */
            nf_.resetResult();
        }

        @Override
        protected Integer doInBackground(Void... arg0) {

            GNApp app = (GNApp) getApplication();
            app.InitGame(-1, null);
            int firstGuess = app.game.restart();

            return firstGuess;
        }

        @Override
        protected void onPostExecute(Integer result) {

            dbg_show_suggest_number(result);

            /* enable add result button */
            nf_.enableAddResult(true);

            /* setup the suggest number */
            nf_.setGuessNumber(result);

            /* hide the indeterminate progress bar */
            setSupportProgressBarIndeterminateVisibility(false);
        }
    }

    private void dbg_show_suggest_number(int num) {
        Log.v(TAG, "suggest number: " + String.format("%X", num));
    }

    /*
     * return the statusbar height, but can not be called in onCreate, it should
     * be called after layout completed.
     */
    private int getStatusbarHeight() {
        Rect rect = new Rect();
        getWindow().getDecorView().getWindowVisibleDisplayFrame(rect);
        Log.v(TAG, "sbar h:" + rect.top);
        return rect.top;
    }

    private Rect getAppArea() {

        /*
         * The width and height returned by the DisplayMetrics may or may not
         * exclude the decoration (ex: status bar) size. Someone said: From
         * android version 3.2, it excludes the decoration, and prior 3.2, it
         * includes the decoration. And I need the size without decoration here,
         * so use another method I tried below.
         */
        // Display display = getWindowManager().getDefaultDisplay();
        // DisplayMetrics metrics = new DisplayMetrics();
        // display.getMetrics(metrics);
        // Log.v(TAG, "W:" + metrics.widthPixels + " H:" +
        // metrics.heightPixels);

        View v = getWindow().findViewById(Window.ID_ANDROID_CONTENT);
        // int h =
        // getWindow().findViewById(Window.ID_ANDROID_CONTENT).getHeight();
        // int t = getWindow().findViewById(Window.ID_ANDROID_CONTENT).getTop();
        // int b =
        // getWindow().findViewById(Window.ID_ANDROID_CONTENT).getBottom();
        // int w =
        // getWindow().findViewById(Window.ID_ANDROID_CONTENT).getWidth();
        // Log.v(TAG, "content h:" + h + " t:" + t + " b:" + b + " w:" + w);
        return new Rect(0, 0, v.getWidth(), v.getBottom());
    }

    private Rect getRectOnScreen(View view, int statusbarHeight) {
        if (view == null)
            return null;

        int[] location = new int[2];
        view.getLocationOnScreen(location);

        return new Rect(location[0],
                location[1] - statusbarHeight,
                location[0] + view.getWidth(),
                location[1] + view.getHeight() - statusbarHeight);
    }

    private void fixRectForGuessNumber(Rect inOutRect) {
        if (inOutRect != null) {
            int digitCount = Integer.parseInt(mSharedPrefs.getString(
                    SettingsActivity.KEY_DIGIT_COUNT, "4"));
            float t = inOutRect.top;
            float b = inOutRect.bottom;
            inOutRect.top = Math.round(t + (b - t) * 0.3f);
            inOutRect.bottom = Math.round(t + (b - t) * 0.7f);
            DisplayMetrics metrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(metrics);
            // Log.v(TAG, "density:" + metrics.density);
            int wheelWidth;
            if (metrics.density - 0.75f < 0.001) {
                wheelWidth = 24; // need to verify...
            } else if (metrics.density - 1.0f < 0.001) {
                wheelWidth = 33;
            } else if (metrics.density - 1.5f < 0.001) {
                wheelWidth = 41;
            } else {
                wheelWidth = 48;
            }
            inOutRect.right = inOutRect.left + wheelWidth * digitCount;
        }
    }

    private void fixRectForGuessResult(Rect inOutRect) {
        if (inOutRect != null) {
            float t = inOutRect.top;
            float b = inOutRect.bottom;
            inOutRect.top = Math.round(t + (b - t) * 0.3f);
            inOutRect.bottom = Math.round(t + (b - t) * 0.7f);
            DisplayMetrics metrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(metrics);
            // Log.v(TAG, "density:" + metrics.density);
            int wheelWidth;
            if (metrics.density - 0.75f < 0.001) {
                wheelWidth = 24; // need to verify...
            } else if (metrics.density - 1.0f < 0.001) {
                wheelWidth = 33;
            } else if (metrics.density - 1.5f < 0.001) {
                wheelWidth = 41;
            } else {
                wheelWidth = 48;
            }
            inOutRect.right = inOutRect.left + wheelWidth * 4;
        }
    }
}
