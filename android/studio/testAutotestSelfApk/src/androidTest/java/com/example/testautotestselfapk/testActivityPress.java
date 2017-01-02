package com.example.testautotestselfapk;

import android.app.Instrumentation;
import android.test.ActivityInstrumentationTestCase2;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Log;
import android.widget.Button;

/**
 * Created by jiao on 2016/7/31.
 *
 * 单元测试案例可以单个运行，AndroidStudio 会自动安装被测试apk，以及把本测试案例生成独立apk并安装
 *
 */
public class testActivityPress extends ActivityInstrumentationTestCase2<MainActivity> {
    private static final String TAG = "testActivityPress";

    private Instrumentation inst;
    private MainActivity activity;
    private Button btn;

    public testActivityPress() {
        super(MainActivity.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        inst = getInstrumentation();
        activity = getActivity();
        btn = (Button) activity.findViewById(R.id.test_btn_counter);
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();
    }

    @SmallTest
    public void testBtnClick() {
        try {
            for (int i = 0; i < 100; i++) {
                Thread.sleep(500);
                inst.runOnMainSync(new Click(btn));
                Thread.sleep(500);
            }
        } catch (Exception e) {
            Log.v(TAG, e.toString());
        }
        Log.e(TAG, "btn text : " + btn.getText().toString());

        int n = Integer.parseInt(btn.getText().toString());
        assertTrue("testBtnClick", n == 100);
    }

    private class Click implements Runnable {
        Button button;

        Click(Button b) {
            button = b;
        }

        @Override
        public void run() {
            button.performClick();
        }
    }
}
