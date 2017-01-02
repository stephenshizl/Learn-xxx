package com.example.remotecamera;

import java.io.File;
import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import JH_Util.JHCamera;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.hardware.Camera;

public class Activity_LocalCamera extends Activity {
	private SurfaceView surfaceView;
	private SurfaceHolder surfaceHolder;
	private Button btnKa;
	private JHCamera myCamera;
	private CameraAutoFocusCallBack autoFocusCallback;
	private CameraSurfaceHolderCallback surfaceHolderCallback;

	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_local_camera);

        if (!_checkHasCamera(this)) {
        	_doWhenNoCamera();
        	return ;
        }

        //> 隐藏状态栏
        // this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //> 隐藏标题栏
        // requestWindowFeature(Window.FEATURE_NO_TITLE);

        //> 设定屏幕显示为横向, 也可以在AndroidManifest.xml中设置 android:screenOrientation="landscape"
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        myCamera = new JHCamera();
        autoFocusCallback = new CameraAutoFocusCallBack();
        surfaceHolderCallback = new CameraSurfaceHolderCallback(myCamera);

        surfaceView = (SurfaceView)this.findViewById(R.id.camera_suface_view);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback((SurfaceHolder.Callback)surfaceHolderCallback);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS); 

        btnKa = (Button)this.findViewById(R.id.camera_takephoto_button);       btnKa.setOnClickListener(new Button.OnClickListener() {
        	public void onClick(View v) {
        	    myCamera.getCamera().autoFocus((android.hardware.Camera.AutoFocusCallback)autoFocusCallback);
        	    myCamera.getCamera().takePicture(
        	                         new Camera.ShutterCallback() {
        	                             public void onShutter() {
        	                                  //> 按下快门瞬间会调用这里的程序
        	                             }
        	                         } ,
        	                         null, //> Camera.PictureCallback(), 要处理raw data 
        	                         null, //> Camera.PictureCallback(), 要处理压缩图片 
        	                         jpegCallback); //> Camera.PictureCallback(), 要处理jpeg图片 
        	}
        });
    }
	
	@Override
    protected void onPause() {
        super.onPause();
        myCamera.getCamera().stopPreview();
    }

	@Override
    protected void onResume() {
        super.onResume();
        myCamera.getCamera().startPreview();
    }

	@Override
	protected void onStop() {
	    super.onResume();
        myCamera.getCamera().stopPreview();
	}
/****************************************************************************************/
	private boolean _checkHasCamera(Context con) {
		return JH_Util.camera_util.checkHasCamera(con);
	}
	
	private void _doWhenNoCamera() {
		Button btnKa = (Button)this.findViewById(R.id.camera_takephoto_button);
        btnKa.setVisibility(Button.GONE);
        new AlertDialog.Builder(this)
        		.setTitle((String)this.getResources().getText(R.string.local_camera_alert_title))
        		.setMessage(R.string.local_camera_alert_text)
        		.setPositiveButton(R.string.local_camera_alert_btn_ok, new android.content.DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
					    //> 结束当前activity
						finish();
					}
        		}).show();
        return ;
	}	
	
	//> 在takepicture中调用的回调方法之一，接收jpeg格式的图像
    Camera.PictureCallback jpegCallback = new Camera.PictureCallback() {
        public void onPictureTaken(byte[] data, Camera camera) {
            try {
                File storageDir =  Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
                if (!storageDir.exists()) {
                    if (!storageDir.mkdirs()) {
                        Log.e("*************", "failed to create directory");
                        return ;
                    }
                }

                String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date(System.currentTimeMillis()));
                File picFile = new File(storageDir.getPath()+File.separator+timeStamp+".jpg");
                if (picFile.exists()) {
                    Log.e("*************", picFile.getPath()+" has exist");
                }

                FileOutputStream outFileStream = new FileOutputStream(picFile);
                outFileStream.write(data);
                outFileStream.close();
            } catch (Exception e) {
                e.printStackTrace();
            }

            //> camera.stopPreview();
            //> 拍照后会自动停止预览
            camera.startPreview();
        }
    };
}
