package com.yhlab.component.guessnumber;

import java.util.NoSuchElementException;
import kankan.wheel.widget.OnWheelScrollListener;
import kankan.wheel.widget.WheelView;
import kankan.wheel.widget.adapters.SymbolWheelAdapter;
import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.Log;
import android.view.animation.AnticipateOvershootInterpolator;
import android.widget.LinearLayout;
import com.yhlab.component.R;

public class GuessedNumber extends LinearLayout {

    public static final int DIGIT_MAX = 8; /*
                                            * How to express sizeof(int) / 4 in
                                            * Java?
                                            */
    public static final int SYMBOL_LEN_MAX = 0xF;
    public static final int DEFAULT_DIGIT_COUNT = 4;
    public static final String DEFAULT_SYMBOLS = "0123456789?";

    private WheelView[] wheel = new WheelView[DIGIT_MAX];
    private int digitCount = DEFAULT_DIGIT_COUNT;
    private String symbols = DEFAULT_SYMBOLS;

    /**
     * This constructor is used for adding this widget by xml declaration.
     * 
     * @param context
     * @param attrs
     * @param defStyle
     */
    public GuessedNumber(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        getParamsFromXml(context, attrs);
        initWidgets(context);
    }

    /**
     * This constructor is used for adding this widget by xml declaration.
     * 
     * @param context
     * @param attrs
     */
    public GuessedNumber(Context context, AttributeSet attrs) {
        super(context, attrs);
        getParamsFromXml(context, attrs);
        initWidgets(context);
    }

    /**
     * This constructor is used for adding view programmatically with default
     * parameters.
     * 
     * @param context
     */
    public GuessedNumber(Context context) {
        super(context);
        initWidgets(context);
    }

    /**
     * This constructor is used for adding this view programmatically.
     * 
     * @param context
     * @param digitCount
     *            the number of digit in this widget.
     * @param symbols
     *            the symbols in one digit.
     */
    public GuessedNumber(Context context, int digitCount, String symbols) {
        super(context);

        this.digitCount = digitCount;
        this.symbols = symbols;

        initWidgets(context);
    }

    private void getParamsFromXml(Context context, AttributeSet attrs) {
        TypedArray a = context.obtainStyledAttributes(attrs,
                R.styleable.GuessedNumber);
        digitCount = a.getInt(R.styleable.GuessedNumber_digitCount,
                DEFAULT_DIGIT_COUNT);
        symbols = a.getString(R.styleable.GuessedNumber_symbols);
        if (symbols == null)
            symbols = DEFAULT_SYMBOLS;
    }

    private void initWidgets(Context context) {

        if (digitCount > DIGIT_MAX || symbols.length() > SYMBOL_LEN_MAX)
            throw new IllegalArgumentException();

        setOrientation(HORIZONTAL);
        for (int i = digitCount - 1; i >= 0; i--) {
            wheel[i] = new WheelView(context);
            wheel[i].setVisibleItems(3);
            wheel[i].setViewAdapter(new SymbolWheelAdapter(context, symbols));
            wheel[i].setCurrentItem((int) (Math.random() * symbols.length()));
            wheel[i].setCyclic(true);
            wheel[i].addScrollingListener(scrolledListener);
            //wheel[i].setInterpolator(new AnticipateOvershootInterpolator());
            addView(wheel[i]);
        }
    }

    /**
     * Set the index number of each digit displayed in the wheel view. Note
     * that, we use 4-bits to represent one digit in the wheel view. <br>
     * For example, number = 0x1234 means: index value of first digit = 1, index
     * value of second digit = 2, index value of third digit = 3, index value of
     * fourth digit = 4.
     * 
     * @param number
     *            the index value of each digit in the wheel view.
     */
    public void setNumber(int number) {
        setNumber(number, false);
    }

    public void setNumber(int number, boolean scrool) {

        int limit = symbols.length(), current;

        for (int id = 0; id < digitCount && ((number & 0xF) < limit); id++, number >>= 4) {
            if (scrool) {
                current = wheel[id].getCurrentItem();
                wheel[id].scroll((number & 0xF) - current, 3000);
            } else {
                wheel[id].setCurrentItem(number & 0xF);
            }
        }
    }

    /**
     * Get the index number of each digit displayed in the wheel view.
     * 
     * @return
     */
    public int getNumber() {

        int result = 0, current;

        for (int id = digitCount - 1; id >= 0; id--) {
            current = wheel[id].getCurrentItem();
            result += current;
            if (id != 0)
                result <<= 4;
        }

        return result;
    }

    private OnWheelScrollListener scrolledListener = new OnWheelScrollListener() {
        public void onScrollingStarted(WheelView w) {
        }

        public void onScrollingFinished(WheelView w) {

            int source = getEventSourceWheel(w);
            int question_mark_index = symbols.indexOf('?');
            int current = w.getCurrentItem();

            if (question_mark_index >= symbols.length())
                return;

            for (int id = 0; id < digitCount; id++) {
                if (id != source) {
                    if (wheel[id].getCurrentItem() == current) {
                        wheel[id].setCurrentItem(question_mark_index);
                    }
                }
            }
        }
    };

    private int getEventSourceWheel(WheelView wheel) {
        int id;

        for (id = 0; id < digitCount; id++) {
            if (this.wheel[id] == wheel)
                break;
        }

        if (id == digitCount)
            throw new NoSuchElementException();

        return id;
    }
}
