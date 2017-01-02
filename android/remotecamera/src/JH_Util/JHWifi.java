package JH_Util;

import java.util.List;

import android.content.Context;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

public class JHWifi {
    private static final String TAG = "---JHWIFI---";
    private WifiManager wifiMgr;
    private List<ScanResult> wifiList;

    public static final int TYPE_NO_PWD = 0x11;  
    public static final int TYPE_WEP = 0x12;  
    public static final int TYPE_WPA = 0x13; 

    public JHWifi(Context context) {
        wifiMgr = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
    }

    public int checkState() {
        return wifiMgr.getWifiState();
    }
    
    public void openWifi() {
        if (!wifiMgr.isWifiEnabled()) {
            wifiMgr.setWifiEnabled(true);
        }
        return;
    }
    
    public void closeWifi() {
        if (wifiMgr.isWifiEnabled()) {
            wifiMgr.setWifiEnabled(false);
        }
        return;
    }
    
    public void startScan() {
        if (true == wifiMgr.startScan()) {
            wifiList = wifiMgr.getScanResults();
        }
        return;
    }

    public List<ScanResult> getWifiList() {
        return wifiList;
    }
    
    public int addWifi(String ssid, String password, int type, boolean connect) {
        if (ssid == null || password == null || ssid.equals("")) {
            return -1;
        }

        if (type != TYPE_NO_PWD && type != TYPE_WEP && type != TYPE_WPA) {
            return -1;
        }

        WifiConfiguration cfg = createWifiInfo(ssid, password, type);
        int nNetID = wifiMgr.addNetwork(cfg);
        if (connect && -1 != nNetID) wifiMgr.enableNetwork(nNetID, true);
        return nNetID;
    }

/***********************************************************************************/

    //> 只有在wifi开启完全后才能创建热点
    //> 创建指定的wifi配置信息
    private WifiConfiguration createWifiInfo(String ssid, String password, int type) {
        Log.v(TAG, "SSID = " + ssid + "## Password = " + password + "## Type = " + type);
        WifiConfiguration config = new WifiConfiguration();
        config.allowedAuthAlgorithms.clear();
        config.allowedGroupCiphers.clear();
        config.allowedKeyManagement.clear();
        config.allowedPairwiseCiphers.clear();
        config.allowedProtocols.clear();
        config.SSID = "\"" + ssid + "\"";

        {
            WifiConfiguration tmpConfig = isWifiExist(ssid);
            if (null != tmpConfig) {
                if (true != wifiMgr.removeNetwork(tmpConfig.networkId)) {
                    return null;
                }
            }
        }

        switch(type) {
            case TYPE_NO_PWD:
                config.wepKeys[0] = "";
                config.wepTxKeyIndex = 0;
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                break;
            case TYPE_WEP:
                config.hiddenSSID = true;
                config.wepKeys[0] = "\"" + password + "\"";
                config.wepTxKeyIndex = 0;
                config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);
                config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
                config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
                config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
                config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                break;
            case TYPE_WPA:
                config.hiddenSSID = true;  
                config.preSharedKey = "\"" + password + "\"";
                config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);  
                config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
                config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
                config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
                config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP); 
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
                config.status = WifiConfiguration.Status.ENABLED;  
                break;
            default:
        }

        return config;
    }
    
    private WifiConfiguration isWifiExist(String ssid) {
        List<WifiConfiguration> listConfigurations = wifiMgr.getConfiguredNetworks();
        for (WifiConfiguration cfg : listConfigurations) {
            if (cfg.SSID.equals("\"" + ssid + "\"")) {
                return cfg;
            }
        }
        return null;
    }
    
}
