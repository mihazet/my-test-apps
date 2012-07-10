package com.yhlab.commitmonitor;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;

public class ProgressDialogFragment extends DialogFragment {

    private int titleId;
    private int messageId;

    public static ProgressDialogFragment newInstance(int titleId, int msgId) {
        ProgressDialogFragment pdf = new ProgressDialogFragment();
        Bundle bundle = new Bundle();

        bundle.putInt("titleId", titleId);
        bundle.putInt("messageId", msgId);

        pdf.setArguments(bundle);

        return pdf;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Bundle bundle = getArguments();
        titleId = bundle.getInt("titleId");
        messageId = bundle.getInt("messageId");
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {

        ProgressDialog dlg = new ProgressDialog(getActivity());

        if (titleId != 0) {
            dlg.setTitle(titleId);
        } else {
            setStyle(STYLE_NO_TITLE, getTheme());
        }
        dlg.setMessage(getString(messageId));
        dlg.setCanceledOnTouchOutside(false);
        dlg.setIndeterminate(true);

        return dlg;
    }
}
