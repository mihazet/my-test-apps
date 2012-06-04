package com.yhlab.commitmonitor;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.CheckBox;

public class RepoConfigFragment extends Fragment {

    static final String TAG = "RepoConfigFragment";

    private final OnClickListener _clickHandler = new OnClickListener() {
        public void onClick(View v) {
            int id = v.getId();
            switch (id) {
            case R.id.btn_repo_config_save:
                Log.d(TAG, "save button clicked");
                break;
            case R.id.btn_repo_config_cancel:
                Log.d(TAG, "cancel button clicked");
                break;
            case R.id.btn_repo_config_delete:
                Log.d(TAG, "delete button clicked");
                break;
            case R.id.chkbox_repo_anonymous:
                getView().findViewById(R.id.edit_repo_username).setEnabled(
                        !((CheckBox) v).isChecked());
                getView().findViewById(R.id.edit_repo_password).setEnabled(
                        !((CheckBox) v).isChecked());
                break;
            default:
                Log.d(TAG, "some button clicked");
                break;
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreate");

        super.onCreate(savedInstanceState);
        this.setHasOptionsMenu(true);

        Log.v(TAG, "<<< onCreate");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        Log.v(TAG, ">>> onCreateView");

        View v = inflater.inflate(R.layout.repoconfig_fragment, container,
                false);
        // View v = super.onCreateView(inflater, container, savedInstanceState);

        Log.v(TAG, "<<< onCreateView");

        return v;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {

        Log.v(TAG, ">>> onViewCreated");
        View v, vRoot = getView();

        super.onViewCreated(view, savedInstanceState);

        /* process view stuff */
        v = vRoot.findViewById(R.id.btn_repo_config_save);
        if (v != null)
            v.setOnClickListener(_clickHandler);
        v = vRoot.findViewById(R.id.btn_repo_config_cancel);
        if (v != null)
            v.setOnClickListener(_clickHandler);
        v = vRoot.findViewById(R.id.btn_repo_config_delete);
        if (v != null)
            v.setOnClickListener(_clickHandler);
        v = vRoot.findViewById(R.id.chkbox_repo_anonymous);
        if (v != null)
            v.setOnClickListener(_clickHandler);

        /* process view stuff container-specific */
        int container_id = ((ViewGroup) (getActivity()
                .findViewById(android.R.id.content))).getChildAt(0).getId();
        switch (container_id) {
        case R.id.Layout1stUse:
            /* No need delete button in 1st use layout, hide it. */
            v = vRoot.findViewById(R.id.btn_repo_config_delete);
            if (v != null) {
                v.setVisibility(View.INVISIBLE);
            }
            break;
        default:
            break;
        }

        Log.v(TAG, "<<< onViewCreated");
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.repoconfig_optmenu, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.optitem_add_repo_sample:
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }
}
