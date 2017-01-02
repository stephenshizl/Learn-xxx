package JH_Util;

import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.util.Log;

public class JHCamera {
    public Camera camera;
    
    public JHCamera() {
        this.camera = null;
        createCamera();
    }

    public Camera getCamera() {
        return camera;
    }
    
    public Camera createCamera() {
        if (null != camera) return camera;

        try {
            camera = Camera.open();
        } catch (Exception e) {
            Log.e("============", "摄像头被占用");
        }

        return camera;
    }
    
    public void initCamera(int width, int height) {
        if (null == camera) return ;
        
        try {
            //> 设定相片大小为1024*768， 格式为JPG
            Camera.Parameters parameters = camera.getParameters();
            parameters.setPreviewSize(width, height); 
            parameters.setPictureFormat(PixelFormat.JPEG);
            //> parameters.setPictureSize(1024, 768);
            camera.setParameters(parameters);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return ;
    }

    public void stopCamera() {
        if (null == camera) return ;

        try {
            camera.stopPreview();
            camera.release();
            camera = null;
        }catch (Exception e) {
            e.printStackTrace();
        }
        return ;
    }
}
