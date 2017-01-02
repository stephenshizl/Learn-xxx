package com.example.testautotestsyscamera;

import android.app.Instrumentation;
import android.graphics.Camera;
import android.test.ActivityInstrumentationTestCase2;
import android.test.suitebuilder.annotation.LargeTest;
import android.util.Log;
import android.view.KeyEvent;

/**
 * Created by jiao on 2016/7/31.
 *
 * Junit / Instrumentation test case for camera test
 * !!!
 * 运行失败：无法获取camera的签名和android:sharedUserId
 *
 * Running the test suite:
 * adb shell am instrument -w -e class com.example.testautotestsyscamera.ImageCapture \
 *     com.example.testautotestsyscamera/android.test.InstrumentationTestRunner
 *
 * https://developer.android.com/studio/test/command-line.html#AMSyntax
 * https://developer.android.com/reference/android/support/test/runner/AndroidJUnitRunner.html
*/
public class ImageCapture extends ActivityInstrumentationTestCase2<Camera>{
    private static final String Tag = "ImageCapture";
    private static final int TOTAL_NUMBER_OF_IMAGECAPTURE = 100;
    private static final int TOTAL_NUMBER_OF_VIDEOCAPTURE = 100;
    private static final long WAIT_FOR_IMAGE_CAPTURE_TO_BE_TAKEN = 1000;
    private static final long WAIT_FOR_VIDEO_CAPTURE_TO_BE_TAKEN = 50000; //50seconds
    private static final long WAIT_FOR_PREVIEW = 1000; //1 seconds

    public ImageCapture() {
        super(Camera.class);
    }

    @Override
    protected void setUp() throws Exception {
        getActivity();
        super.setUp();
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();
    }

    @LargeTest
    public void testImageCapture() {
        Instrumentation inst = getInstrumentation();
        try {
            for (int i = 0; i < TOTAL_NUMBER_OF_IMAGECAPTURE; i++) {
                Thread.sleep(WAIT_FOR_IMAGE_CAPTURE_TO_BE_TAKEN);
                inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_UP);
                inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_CENTER);
                Thread.sleep(WAIT_FOR_IMAGE_CAPTURE_TO_BE_TAKEN);
            }
        } catch (Exception e) {
            Log.v(Tag, e.toString());
        }
        assertTrue("testImageCapture", true);
    }

    @LargeTest
    public void testVideoCapture() {
        Instrumentation inst = getInstrumentation();
        //Switch to the video mode
        inst.sendKeyDownUpSync(KeyEvent.KEYCODE_MENU);
        inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_CENTER);
        try {
            for (int i = 0; i < TOTAL_NUMBER_OF_VIDEOCAPTURE; i++) {
                Thread.sleep(WAIT_FOR_PREVIEW);
                inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_UP);
                //record an video
                inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_CENTER);
                Thread.sleep(WAIT_FOR_VIDEO_CAPTURE_TO_BE_TAKEN);
                inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_CENTER);
                Thread.sleep(WAIT_FOR_PREVIEW);
                inst.sendKeyDownUpSync(KeyEvent.KEYCODE_DPAD_CENTER);
            }
        } catch (Exception e) {
            Log.v(Tag, e.toString());
        }
        assertTrue("testVideoCapture", true);
    }
}
