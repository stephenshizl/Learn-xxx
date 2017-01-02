package JH_Util;

import android.content.Context;
import android.content.pm.PackageManager;

public class camera_util {
	public static boolean checkHasCamera(Context context) {
		if (context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA)) {
			return true;
		}
		return false;
		
	}
}
