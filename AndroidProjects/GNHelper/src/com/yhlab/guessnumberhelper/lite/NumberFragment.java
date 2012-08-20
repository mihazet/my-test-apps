package com.yhlab.guessnumberhelper.lite;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TableRow;
import com.yhlab.component.guessnumber.GuessedNumber;
import com.yhlab.component.guessnumber.GuessedResult;

public class NumberFragment extends Fragment {

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        try {
            mGuessedListener = (IGuessedListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement IGuessedListener interface.");
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        View v = inflater.inflate(R.layout.number_fragment, container, false);

        return v;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        SharedPreferences sp = PreferenceManager
                .getDefaultSharedPreferences(getActivity());
        int digitCount = Integer.parseInt(sp.getString(
                SettingsActivity.KEY_DIGIT_COUNT, "4"));

        /* add dynamic widgets GuessedNumber */
        final GuessedNumber gn = new GuessedNumber(getActivity(), digitCount,
                "0123456789?");
        TableRow tr = (TableRow) getView().findViewById(R.id.row_guess);
        TableRow.LayoutParams trlp = new TableRow.LayoutParams();
        trlp.column = 1;
        trlp.span = 2;
        tr.addView(gn, trlp);

        /* add dynamic widgets GuessedResult */
        final GuessedResult gr = new GuessedResult(getActivity(), digitCount);
        tr = (TableRow) getView().findViewById(R.id.row_result);
        tr.addView(gr, 1);

        Button add = (Button) (getView().findViewById(R.id.btn_result_add));
        add.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                mGuessedListener.onGuess(gn.getNumber(), gr.getResult());
            }

        });
    }

    public interface IGuessedListener {
        public void onGuess(int number, int result);
    }

    private IGuessedListener mGuessedListener;
}
