package com.yhlab.guessnumberhelper.lite;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TableRow;
import android.widget.Toast;
import com.actionbarsherlock.app.SherlockFragment;
import com.yhlab.component.guessnumber.GuessedNumber;
import com.yhlab.component.guessnumber.GuessedResult;

public class NumberFragment extends SherlockFragment {

    private GuessedNumber gn;
    private GuessedResult gr;
    private int digitCount;

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

        View v = inflater.inflate(R.layout.fragment_number, container, false);

        return v;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        SharedPreferences sp = PreferenceManager
                .getDefaultSharedPreferences(getActivity());
        digitCount = Integer.parseInt(sp.getString(
                SettingsActivity.KEY_DIGIT_COUNT, "4"));

        /* add dynamic widgets GuessedNumber */
        gn = new GuessedNumber(getActivity(), digitCount, "0123456789?");
        TableRow tr = (TableRow) getView().findViewById(R.id.row_guess);
        TableRow.LayoutParams trlp = new TableRow.LayoutParams();
        trlp.column = 1;
        trlp.span = 2;
        tr.addView(gn, trlp);

        /* add dynamic widgets GuessedResult */
        gr = new GuessedResult(getActivity(), digitCount);
        tr = (TableRow) getView().findViewById(R.id.row_result);
        tr.addView(gr, 1);

        Button add = (Button) (getView().findViewById(R.id.btn_result_add));
        add.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                int gnn = gn.getNumber();
                int grr = gr.getResult();
                int grrA = (grr >> 4) & 0xF;
                int grrB = grr & 0xF;
                boolean invalidNumber = false;
                boolean invalidResult = false;

                for (int digit = 0; digit < digitCount; digit++) {
                    if (((gnn >> (digit * 4)) & 0xF) >= 10) {
                        invalidNumber = true;
                        break;
                    }
                }
                if (grrA > digitCount || grrB > digitCount
                        || (grrA + grrB) > digitCount) {
                    invalidResult = true;
                }
                
                if (invalidNumber)
                    Toast.makeText(getActivity().getApplicationContext(),
                            getString(R.string.notify_badformat_guess),
                            Toast.LENGTH_SHORT).show();
                else if (invalidResult)
                    Toast.makeText(getActivity().getApplicationContext(),
                            getString(R.string.notify_badformat_result),
                            Toast.LENGTH_SHORT).show();
                else
                    mGuessedListener.onGuess(gnn, grr);
            }

        });
    }

    public interface IGuessedListener {
        public void onGuess(int number, int result);
    }

    private IGuessedListener mGuessedListener;

    public void changeDigitCount(int newDigitCount) {
        gn.changeDigitCount(newDigitCount);
        gr.changeDigitUsed(newDigitCount);
    }

    public void setGuessNumber(int guess) {
        gn.setNumber(guess, true);
    }

    public int getGuessNumber() {
        return gn.getNumber();
    }

    public void resetResult() {
        gr.resetToUnknown();
    }

    public void enableAddResult(boolean enabled) {
        Button btn = (Button) getView().findViewById(R.id.btn_result_add);
        if (btn != null) {
            btn.setEnabled(enabled);
        }
    }
}
