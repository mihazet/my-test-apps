package com.yhlab.guessnumberhelper.lite;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class IntroActivity extends Activity {

    private static final String TAG = "IntroActivity";

    /* action sequence */
    private final int SEQ_NOTHING = 0;
    private final int SEQ_DRAW_RESTART_TEXT = SEQ_NOTHING + 2;
    private final int SEQ_DRAW_RESTART_ACTION = SEQ_DRAW_RESTART_TEXT + 30;
    private final int SEQ_DRAW_NUMBER_1_TEXT = SEQ_DRAW_RESTART_ACTION + 40;
    private final int SEQ_DRAW_NUMBER_ACTION = SEQ_DRAW_NUMBER_1_TEXT + 10;
    private final int SEQ_DRAW_NUMBER_2_TEXT = SEQ_DRAW_NUMBER_1_TEXT + 80;
    private final int SEQ_DRAW_NUMBER_3_TEXT = SEQ_DRAW_NUMBER_2_TEXT + 80;
    private final int SEQ_DRAW_RESULT_1_TEXT = SEQ_DRAW_NUMBER_3_TEXT + 60;
    private final int SEQ_DRAW_RESULT_1_ACTION = SEQ_DRAW_RESULT_1_TEXT + 1;
    private final int SEQ_DRAW_RESULT_2_TEXT = SEQ_DRAW_RESULT_1_TEXT + 80;
    private final int SEQ_DRAW_RESULT_2_ACTION = SEQ_DRAW_RESULT_2_TEXT + 1;
    private final int SEQ_DRAW_RESULT_3_TEXT = SEQ_DRAW_RESULT_2_TEXT + 80;
    private final int SEQ_DRAW_STEP_REPEAT_TEXT = SEQ_DRAW_RESULT_3_TEXT + 80;
    private final int SEQ_DRAW_NOTICE_1_TEXT = SEQ_DRAW_STEP_REPEAT_TEXT + 80;
    private final int SEQ_MAX = SEQ_DRAW_NOTICE_1_TEXT + 100;
    private int sequence = SEQ_NOTHING;

    /* sequence update timer task */
    private AsyncTask<Void, Void, Void> timerTask;
    private View v;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        v = new HelpView(this);
        setContentView(v);
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume");
        super.onResume();
        timerTask = new TimerTask().execute();
    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause");
        super.onPause();
        timerTask.cancel(true);
    }

    private class HelpView extends View {

        Context mContext;
        Paint actionPaint;
        Paint rectPaint;
        GNApp app;
        Bitmap finger;
        String footprint, desc_restart, desc_number_1, desc_number_2,
                desc_number_3, desc_result_1, desc_result_2, desc_result_3,
                desc_step_repeat, desc_notice_1;

        public HelpView(Context context) {
            super(context);

            mContext = context;
            app = (GNApp) getApplication();
            finger = BitmapFactory.decodeResource(getResources(),
                    R.drawable.finger_rt);

            actionPaint = new Paint();
            actionPaint.setColor(0x801010F8);

            rectPaint = new Paint();
            rectPaint.setColor(Color.RED);
            rectPaint.setStyle(Paint.Style.STROKE);
            rectPaint.setStrokeWidth(5);

            footprint = getString(R.string.desc_interrupt_intro);
            desc_restart = getString(R.string.desc_restart);
            desc_number_1 = getString(R.string.desc_number_1);
            desc_number_2 = getString(R.string.desc_number_2);
            desc_number_3 = getString(R.string.desc_number_3);
            desc_result_1 = getString(R.string.desc_result_1);
            desc_result_2 = getString(R.string.desc_result_2);
            desc_result_3 = getString(R.string.desc_result_3);
            desc_step_repeat = getString(R.string.desc_step_repeat);
            desc_notice_1 = getString(R.string.desc_notice_1);
        }

        @Override
        protected void onDraw(Canvas canvas) {

            /* draw action */
            if (sequence >= SEQ_DRAW_RESULT_2_ACTION) {
                if (sequence - SEQ_DRAW_RESULT_2_ACTION > 5
                        && sequence - SEQ_DRAW_RESULT_2_ACTION < 12) {
                    canvas.drawCircle(
                            app.rectAddResult.centerX(),
                            app.rectAddResult.centerY(),
                            3 * Math.min(
                                    0.25f * (app.rectAddResult.bottom - app.rectAddResult.top),
                                    1.5f * (sequence - SEQ_DRAW_RESULT_2_ACTION)),
                            actionPaint);
                }
                if (sequence - SEQ_DRAW_RESULT_2_ACTION < 20) {
                    canvas.drawBitmap(
                            finger,
                            app.rectAddResult.centerX()
                                    - finger.getWidth()
                                    - 30
                                    + 6
                                    * Math.min(5, sequence
                                            - SEQ_DRAW_RESULT_2_ACTION),
                            app.rectAddResult.centerY()
                                    + 30
                                    - 6
                                    * Math.min(5, sequence
                                            - SEQ_DRAW_RESULT_2_ACTION),
                            null);
                }
            } else if (sequence >= SEQ_DRAW_RESULT_1_ACTION) {
                canvas.drawRect(app.rectGuessResult, rectPaint);
            } else if (sequence >= SEQ_DRAW_NUMBER_ACTION) {
                canvas.drawRect(app.rectGuessNumber, rectPaint);
            } else if (sequence >= SEQ_DRAW_RESTART_ACTION) {
                if (sequence - SEQ_DRAW_RESTART_ACTION > 5
                        && sequence - SEQ_DRAW_RESTART_ACTION < 12) {
                    canvas.drawCircle(
                            app.rectRestartGame.centerX(),
                            app.rectRestartGame.centerY(),
                            3 * Math.min(0.25f * (app.rectRestartGame.bottom
                                    - app.rectRestartGame.top),
                                    1.5f * (sequence - SEQ_DRAW_RESTART_ACTION)),
                            actionPaint);
                }
                if (sequence - SEQ_DRAW_RESTART_ACTION < 20) {
                    canvas.drawBitmap(
                            finger,
                            app.rectRestartGame.centerX()
                                    - finger.getWidth()
                                    - 30
                                    + 6
                                    * Math.min(5, sequence
                                            - SEQ_DRAW_RESTART_ACTION),
                            app.rectRestartGame.centerY()
                                    + 30
                                    - 6
                                    * Math.min(5, sequence
                                            - SEQ_DRAW_RESTART_ACTION),
                            null);
                }
            }

            /* draw text */
            if (sequence >= SEQ_DRAW_NOTICE_1_TEXT) {
                drawTextOnCanvas(canvas, desc_notice_1, 5,
                        app.rectAppArea.bottom * .2f);
            } else if (sequence >= SEQ_DRAW_STEP_REPEAT_TEXT) {
                drawTextOnCanvas(canvas, desc_step_repeat, 5,
                        app.rectAppArea.bottom * .5f);
            } else if (sequence >= SEQ_DRAW_RESULT_3_TEXT) {
                drawTextOnCanvas(canvas, desc_result_3, 5,
                        app.rectAppArea.bottom * .2f);
            } else if (sequence >= SEQ_DRAW_RESULT_2_TEXT) {
                drawTextOnCanvas(canvas, desc_result_2, 5,
                        app.rectAppArea.bottom * .2f);
            } else if (sequence >= SEQ_DRAW_RESULT_1_TEXT) {
                drawTextOnCanvas(canvas, desc_result_1, 5,
                        app.rectAppArea.bottom * .2f);
            } else if (sequence >= SEQ_DRAW_NUMBER_3_TEXT) {
                drawTextOnCanvas(canvas, desc_number_3, 5,
                        app.rectAppArea.bottom * .5f);
            } else if (sequence >= SEQ_DRAW_NUMBER_2_TEXT) {
                drawTextOnCanvas(canvas, desc_number_2, 5,
                        app.rectAppArea.bottom * .5f);
            } else if (sequence >= SEQ_DRAW_NUMBER_1_TEXT) {
                drawTextOnCanvas(canvas, desc_number_1, 5,
                        app.rectAppArea.bottom * .5f);
            } else if (sequence >= SEQ_DRAW_RESTART_TEXT) {
                drawTextOnCanvas(canvas, desc_restart, 5,
                        app.rectAppArea.bottom * .5f);
            }

            /* draw footprint text */
            if (((sequence / 5) % 2) == 0)
                drawFootText(canvas, footprint);
        }

        private void drawTextOnCanvas(Canvas canvas, String text,
                float left, float top) {

            TextView tv = new TextView(mContext);
            tv.setText(text);
            tv.setTextColor(Color.RED);
            tv.setBackgroundColor(Color.WHITE);
            tv.setTextSize(20);
            tv.setDrawingCacheEnabled(true);
            // Setup how big the view should be
            tv.measure(
                    MeasureSpec.makeMeasureSpec(canvas.getWidth() - (int) left
                            * 2,
                            MeasureSpec.AT_MOST),
                    MeasureSpec.makeMeasureSpec(canvas.getHeight() - (int) top,
                            MeasureSpec.AT_MOST));
            // assign the layout values to the textview
            tv.layout(0, 0, tv.getMeasuredWidth(), tv.getMeasuredHeight());
            // draw the bitmap from the drawingcache to canvas
            canvas.drawBitmap(tv.getDrawingCache(), left, top, null);

            // disable drawing cache
            tv.setDrawingCacheEnabled(false);
        }

        private void drawFootText(Canvas canvas, String text) {

            TextView tv = new TextView(mContext);
            tv.setText(text);
            tv.setTextColor(Color.GREEN);
            tv.setTextSize(25.0f);
            tv.setDrawingCacheEnabled(true);
            tv.measure(
                    MeasureSpec.makeMeasureSpec(canvas.getWidth() - 10,
                            MeasureSpec.AT_MOST),
                    MeasureSpec.makeMeasureSpec(canvas.getHeight(),
                            MeasureSpec.AT_MOST));
            tv.layout(0, 0, tv.getMeasuredWidth(), tv.getMeasuredHeight());
            canvas.drawBitmap(tv.getDrawingCache(), 5, app.rectAppArea.bottom
                    - tv.getHeight(), null);
            tv.setDrawingCacheEnabled(false);
        }

    }

    private class TimerTask extends AsyncTask<Void, Void, Void> {

        @Override
        protected Void doInBackground(Void... params) {

            Log.v(TAG, "TimerTask start running...");
            while (sequence++ < SEQ_MAX) {
                if (isCancelled()) {
                    Log.v(TAG, "TimerTask is cancelled, quit!");
                    break;
                }
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    // do nothing...
                }
                v.postInvalidate();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            finish();
        }
    }
}
