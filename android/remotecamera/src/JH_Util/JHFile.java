package JH_Util;

import java.io.File;

import android.os.Environment;

public class JHFile {
    //> 获取共享的媒体目录：File storageDir =  Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
    
    public static Boolean IsExternalStorageExist() {
        if (Environment.getExternalStorageState() == Environment.MEDIA_MOUNTED) {
            return true;
        }
        return false;
    }
    
    public static String createFileOnExternalStorage(String strRelativePath) {
        String strAbsolutePathString = null;
        File skRoot = Environment.getExternalStorageDirectory();
        strAbsolutePathString = skRoot.getPath() + File.separator + strRelativePath;
        File file = new File(strAbsolutePathString);
        //> TODO:
        return strAbsolutePathString;
    }
    
    
}
