package com.yhlab.guessnumberhelper.lite;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class HistoryFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        View v = inflater.inflate(R.layout.history_fragment, container, false);

        return v;
    }

    public void addHistory(int number, int result) {
        TextView tv = (TextView) getView()
                .findViewById(R.id.tv_guessed_history);
        tv.append(String.format("%X %XA%XB\n", number, result >> 4, result & 0xF));
    }

    public void clearHistory() {
        TextView tv = (TextView) getView()
                .findViewById(R.id.tv_guessed_history);
        tv.setText(null);
    }
}
