package com.yhlab.commitmonitor;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;

public class FirstUseFragment extends Fragment implements OnClickListener {
    private static final String TAG = "FirstUseFragment";

    INextButtonClickListener mListener;

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        try {
            mListener = (INextButtonClickListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement INextButtonClickListener interface.");
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreateView");

        View v = inflater.inflate(R.layout.firstuse_fragment, container, false);

        Log.v(TAG, "<<< onCreateView");
        return v;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {

        Log.v(TAG, ">>> onViewCreated");

        super.onViewCreated(view, savedInstanceState);

        View btn = view.findViewById(R.id.btn_next_add_repo);
        btn.setOnClickListener(this);

        Log.v(TAG, "<<< onViewCreated");
    }

    /**
     * The interface(s) that the container activity must implement
     */
    public interface INextButtonClickListener {
        public void onNextButtonClick();
    }

    public void onClick(View v) {
        int id = v.getId();
        switch (id) {
        case R.id.btn_next_add_repo:
            mListener.onNextButtonClick();
            break;
        default:
            Log.w(TAG, "Unhandled click event: id = " + id);
            break;
        }
    }
}
