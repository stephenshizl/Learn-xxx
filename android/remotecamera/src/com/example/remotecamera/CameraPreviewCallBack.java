package com.example.remotecamera;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Environment;
import android.util.Log;

//> 每次camera采集到新图像时调用的回调方法，前提是必须开启预览
public class CameraPreviewCallBack implements Camera.PreviewCallback {
    static private Camera.Size size;
    private int nCnt = 0;
    
    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        size = camera.getParameters().getPreviewSize();
        if (nCnt == 1)
        {
            storePicOnExternalStorage(data, camera);
        }

        nCnt++;
        if (nCnt > 1000) nCnt %= 1000;
    }

    public void storePicOnExternalStorage(byte[] data, Camera camera) {
        if (ImageFormat.NV21 != camera.getParameters().getPreviewFormat()) {
            return ;
        }

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
            YuvImage image = new YuvImage(data, ImageFormat.NV21, size.width, size.height, null);
            if (image != null) {
                image.compressToJpeg(new Rect(0, 0, size.width, size.height), 80, outFileStream);
                outFileStream.write(data);
                outFileStream.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } 
        
        return ;
    }
}
