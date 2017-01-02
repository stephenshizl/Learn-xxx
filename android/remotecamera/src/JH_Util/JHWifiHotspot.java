package JH_Util;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.content.Context;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;

public class JHWifiHotspot {
    private WifiManager wifiMgr;

    public JHWifiHotspot(Context context) {
        wifiMgr = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
    }
    
    public boolean startWifiHotspot(String ssid, String password) {
        if (wifiMgr.isWifiEnabled()) {
            wifiMgr.setWifiEnabled(false);
        }
        
        _startWifiHotspot(ssid, password);
        return true;
    }

    public boolean isWifiHotspotEnabled() {
        return _isWifiHotspotEnabled(wifiMgr);
    }
    
    public boolean closeWifiHotspot() {
        return _closeWifiHotspot(wifiMgr);
    }
    
    public static boolean closeWifiHotspot(Context con) {
        WifiManager wifiMgr = (WifiManager)con.getSystemService(Context.WIFI_SERVICE);
        return _closeWifiHotspot(wifiMgr);
    }



    private static boolean _isWifiHotspotEnabled(WifiManager wifiMgr) {
        try {
            Method method = wifiMgr.getClass().getMethod("isWifiApEnabled");
            method.setAccessible(true);
            return (Boolean) method.invoke(wifiMgr);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
    
    private static boolean _closeWifiHotspot(WifiManager wifiMgr) {
        boolean bRet = false;

        if (!_isWifiHotspotEnabled(wifiMgr)) {
            return true;
        }
        
        try {
            Method method = wifiMgr.getClass().getMethod("getWifiApConfiguration");
            method.setAccessible(true);

            WifiConfiguration config = (WifiConfiguration)method.invoke(wifiMgr);
            Method method2 = wifiMgr.getClass().getMethod("setWifiApEnabled", WifiConfiguration.class, boolean.class);
            method2.invoke(wifiMgr, config, false);
            bRet = true;
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        
        return bRet;
    }

    private boolean _startWifiHotspot(String ssid, String password) {
        Method method = null;
        boolean bRet = false;
        
        try {
            method = wifiMgr.getClass().getMethod("setWifiApEnabled", WifiConfiguration.class, boolean.class);
            WifiConfiguration config = new WifiConfiguration();
            config.SSID = ssid;
            config.preSharedKey = password;
            
            config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
            config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
            config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
            config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
            config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            
            method.invoke(wifiMgr, config, true);
            bRet = true;
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodError e) {
            e.printStackTrace();
        }
        
        return bRet;
    }
}
