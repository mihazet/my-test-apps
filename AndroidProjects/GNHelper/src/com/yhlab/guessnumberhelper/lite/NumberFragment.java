package com.yhlab.guessnumberhelper.lite;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
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
       
        Button add = (Button) (getView().findViewById(R.id.btn_result_add));
        add.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                GuessedNumber n = (GuessedNumber) (getView().findViewById(R.id.guessed_number));
                GuessedResult r = (GuessedResult) (getView().findViewById(R.id.guessed_result));
                mGuessedListener.onGuess(n.getNumber(), r.getResult());
            }

        });
    }

    public interface IGuessedListener {
        public void onGuess(int number, int result);
    }

    private IGuessedListener mGuessedListener;
}
