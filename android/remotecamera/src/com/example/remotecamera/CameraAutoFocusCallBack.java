package com.example.remotecamera;

import android.hardware.Camera;

/* 自定义class AutoFocusCallback */
@SuppressWarnings("deprecation")
public class CameraAutoFocusCallBack implements android.hardware.Camera.AutoFocusCallback {

	@Override
	public void onAutoFocus(boolean success, Camera camera) {
		// TODO Auto-generated method stub
		if (success) {
            // TODO: 
        }
	}

}
