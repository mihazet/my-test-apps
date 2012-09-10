package com.yhlab.guessnumberhelper.lite;

import java.util.ArrayList;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import com.actionbarsherlock.app.SherlockFragment;

public class HistoryFragment extends SherlockFragment {

    @SuppressWarnings("unused")
    private static final String TAG = "HistoryFragment";
    private TextView tv1;
    private TextView tv2;
    private ArrayList<String> savedHistory = new ArrayList<String>();
    private final String KEY_HISTORY = "History";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        View v = inflater.inflate(R.layout.fragment_history, container, false);

        return v;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        tv1 = (TextView) getView().findViewById(R.id.tv_guessed_history);
        tv2 = (TextView) getView().findViewById(R.id.tv_guessed_history_2);

        /* check if we need to restore previous instance state */
        if (savedInstanceState != null) {
            savedHistory = savedInstanceState.getStringArrayList(KEY_HISTORY);
            if (!savedHistory.isEmpty()) {
                TextView tv;
                int historyCount = savedHistory.size();
                for (int history = 0; history < historyCount; history++) {
                    if ((history % 2) == 1 && tv2 != null)
                        tv = tv2;
                    else
                        tv = tv1;
                    tv.append(savedHistory.get(history));
                }
            }
        }
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        outState.putStringArrayList(KEY_HISTORY, savedHistory);
    }

    public void addHistory(int number, int result) {

        TextView tv;

        if ((savedHistory.size() % 2) == 1 && tv2 != null)
            tv = tv2;
        else
            tv = tv1;

        int digitCount = Integer.parseInt(PreferenceManager
                .getDefaultSharedPreferences(getActivity()).getString(
                        SettingsActivity.KEY_DIGIT_COUNT, "4"));
        String fmt = "[%d] %0" + digitCount + "X %XA%XB\n";
        String historyRecord = String.format(fmt, savedHistory.size() + 1,
                number, result >> 4, result & 0xF);
        savedHistory.add(historyRecord);
        tv.append(historyRecord);
    }

    public void clearHistory() {

        tv1.setText(null);
        if (tv2 != null)
            tv2.setText(null);

        savedHistory.clear();
    }
}
