package com.yhlab.component.guessnumber;

import kankan.wheel.widget.WheelView;
import kankan.wheel.widget.adapters.SymbolWheelAdapter;
import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import com.yhlab.component.R;

public class GuessedResult extends LinearLayout {

    public static final int DIGIT_USED_MAX = 8;
    public static final int DEFAULT_DIGIT_USED = 4;

    private WheelView[] wheel = new WheelView[2];
    private int digitUsed = DEFAULT_DIGIT_USED;
    private static final String RESULT = "012345678";
    private String symbols;

    public GuessedResult(Context context, AttributeSet attrs) {
        super(context, attrs);
        getParamsFromXml(context, attrs);
        initWidgets(context);
    }

    public GuessedResult(Context context) {
        super(context);
        initWidgets(context);
    }

    public GuessedResult(Context context, int digitUsed) {
        super(context);

        this.digitUsed = digitUsed;

        initWidgets(context);
    }

    private void getParamsFromXml(Context context, AttributeSet attrs) {
        TypedArray a = context.obtainStyledAttributes(attrs,
                R.styleable.GuessedResult);
        digitUsed = a.getInt(R.styleable.GuessedResult_digitUsed,
                DEFAULT_DIGIT_USED);
    }

    private void initWidgets(Context context) {
        if (digitUsed > DIGIT_USED_MAX)
            throw new IllegalArgumentException();

        setOrientation(HORIZONTAL);

        LayoutInflater.from(context).inflate(R.layout.guessed_result, this,
                true);

        symbols = RESULT.substring(0, digitUsed + 1) + "?";
        for (int i = 0; i < 2; i++) {
            if (i == 1)
                wheel[i] = (WheelView) findViewById(R.id.aCount);
            else if (i == 0)
                wheel[i] = (WheelView) findViewById(R.id.bCount);
            wheel[i].setVisibleItems(3);
            wheel[i].setViewAdapter(new SymbolWheelAdapter(context, symbols));
            wheel[i].setCurrentItem(symbols.length() - 1);
            wheel[i].setCyclic(true);
        }
    }

    public int getResult() {

        int result = 0, current;

        current = wheel[1].getCurrentItem();
        result += current << 4;
        current = wheel[0].getCurrentItem();
        result += current;

        return result;
    }
}
