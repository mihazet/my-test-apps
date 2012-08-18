package com.yhlab.guessnumberhelper.lite;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.view.Menu;

public class MainActivity extends FragmentActivity implements
        NumberFragment.IGuessedListener {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
    @Override
    public void onGuess(int number, int result) {
        FragmentManager fm = getSupportFragmentManager();
        HistoryFragment hf = (HistoryFragment) fm
                .findFragmentById(R.id.history_fragment);
        hf.addHistory(number, result);
    }
}
