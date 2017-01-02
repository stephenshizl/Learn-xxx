package com.example.remotecamera;

import java.io.IOException;

import JH_Util.JHCamera;
import android.graphics.drawable.GradientDrawable.Orientation;
import android.view.SurfaceHolder;

public class CameraSurfaceHolderCallback implements SurfaceHolder.Callback {
	private JHCamera myCamera;
	
	public CameraSurfaceHolderCallback(JHCamera c) {
	    this.myCamera = c;
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
	    //> 在创建面板的时候调用的方法
	    if (null == myCamera.getCamera()) return ;
	    
	    try {
	        //> 获取当前预览帧的帧率
            int a = myCamera.getCamera().getParameters().getPreviewFrameRate();
             //> int CurFpsRange[] = new int[2];
            //> 获取当前的帧率
            //> myCamera.getCamera().getParameters().getPreviewFpsRange(CurFpsRange);
            //> 设置帧率(两种方法均可)
            //> myCamera.getCamera().getParameters().setPreviewFrameRate(15);
            //> myCamera.getCamera().getParameters().setPreviewFpsRange(CurFpsRange[0], CurFpsRange[1]);
            //> 获取支持的帧率
            //> myCamera.getCamera().getParameters().getSupportedPreviewFpsRange();

            //> 设置显示面板控制器
	        myCamera.getCamera().setPreviewDisplay(holder);
	        //> 只是对预览图像进行了旋转，而不会影响生成的照片
	        myCamera.getCamera().setDisplayOrientation(90);
            //> 设置预览回调对象
            CameraPreviewCallBack pre = new CameraPreviewCallBack();
            myCamera.getCamera().setPreviewCallback(pre);
        } catch (IOException exception) {
            myCamera.stopCamera();
        }
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	    //> 在面板改变的时候调用的方法
	    myCamera.initCamera(width, height);
        myCamera.getCamera().startPreview();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
	    myCamera.stopCamera();
	}

/********************************************************************************/
	
}

