package com.yhlab.fslistview;

import java.io.File;
import java.util.ArrayList;

import android.app.ListActivity;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class FSActivity extends ListActivity {

    private static final String TAG = "FileList";
    private static FSListAdapter mAdapter;
    private static ArrayList<FileMetaData> mFileMetaList;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        mFileMetaList = new ArrayList<FileMetaData>();
        mAdapter = new FSListAdapter(this, R.layout.row, mFileMetaList);
        setListAdapter(mAdapter);

        new DirScanAsyncTask().execute("/mnt/sdcard");
    }

    private class FSListAdapter extends ArrayAdapter<FileMetaData> {

        private ArrayList<FileMetaData> items;

        public FSListAdapter(Context context, int textViewResourceId,
                ArrayList<FileMetaData> items) {
            super(context, textViewResourceId, items);
            this.items = items;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            View v;
            if (convertView == null) {
                LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                v = li.inflate(R.layout.row, null);
            } else {
                v = convertView;
            }

            FileMetaData meta = items.get(position);
            if (meta != null) {
                TextView name = (TextView) v.findViewById(R.id.name);
                TextView size = (TextView) v.findViewById(R.id.size);
                TextView mdate = (TextView) v.findViewById(R.id.mdate);
                if (name != null) {
                    name.setText(meta.getName());
                }
                if (size != null) {
                    size.setText(Long.toString(meta.getSize()));
                }
                if (mdate != null) {
                    mdate.setText(Long.toString(meta.getLastModified()));
                }
            }

            return v;
        }

    }

    private static class DirScanAsyncTask extends
            AsyncTask<String, Long, ArrayList<FileMetaData>> {

        //private final String TAG = "DirScanAsyncTask";
        private File scanDir;

        enum ERROR {
            NO_ERROR, EMPTY_PARAMS, TOO_MORE_PARAMS, NOT_DIR,
        };

        private ERROR error;

        @Override
        protected ArrayList<FileMetaData> doInBackground(String... dirs) {

            ArrayList<FileMetaData> metaList = new ArrayList<FileMetaData>();

            /* parameters checking */
            if (dirs.length == 0) {
                error = ERROR.EMPTY_PARAMS;
                return metaList;
            } else if (dirs.length > 1) {
                error = ERROR.TOO_MORE_PARAMS;
                return metaList;
            } else {
                scanDir = new File(dirs[0]);
                if (!scanDir.isDirectory()) {
                    error = ERROR.NOT_DIR;
                    return metaList;
                }
            }

            /* scanning directory */
            File[] fileList = scanDir.listFiles();
            int fileCount = fileList.length;
            metaList.ensureCapacity(fileCount);

            for (File f : fileList) {
                metaList.add(new FileMetaData(f.getName(), f.length(), f
                        .lastModified(), f.isFile(), f.canRead(), f.canWrite(),
                        f.canExecute()));
            }

            return metaList;
        }

        @Override
        protected void onPreExecute() {
            Log.v(TAG, "onPreExecute");
            error = ERROR.NO_ERROR;
            mFileMetaList.clear();
            mAdapter.notifyDataSetChanged();
        }

        @Override
        protected void onPostExecute(ArrayList<FileMetaData> meta) {
            Log.v(TAG, "onPostExecute");

            switch (error) {
            case NO_ERROR:
//                DateFormat df = DateFormat.getInstance();
//                for (FileMetaData m : meta) {
//                    Log.d(TAG,
//                            m.getName() + " " + m.getSize() + " "
//                                    + df.format(new Date(m.getLastModified())));
//                }
//                Log.d(TAG, "Total files + dirs = " + meta.size());
                
                for (FileMetaData m : meta) {
                    mFileMetaList.add(m);
                }
                mAdapter.notifyDataSetChanged();
                break;
            case EMPTY_PARAMS:
                Log.e(TAG, "No directory indicated!");
                break;
            case TOO_MORE_PARAMS:
                Log.e(TAG, "Can't scan two more directory at the same time!");
                break;
            case NOT_DIR:
                Log.e(TAG, "Indicated path is not a directory!");
                break;
            }
        }

        @Override
        protected void onProgressUpdate(Long... progress) {
            Log.v(TAG, "onProgressUpdate");
        }
    }
}