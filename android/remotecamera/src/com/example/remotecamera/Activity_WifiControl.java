package com.example.remotecamera;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import JH_Util.JHWifi;
import JH_Util.ListViewItemAdapter;
import JH_Util.ListViewItemAdapter.CallbackData;
import android.app.Activity;
import android.net.wifi.ScanResult;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class Activity_WifiControl extends Activity {
    ListView listView;
    List<String> wifiSSIDList;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wifi_hotspot);

        // _showWifiList();
        // _showWifiListWithSimpleAdapter();
        _showWifiListWithBaseAdapter();
    }

    public void getWifiSSID(List<String> SSIDList) {
        SSIDList.clear();
        JHWifi wifi;
        wifi = new JHWifi(this);
        wifi.startScan();
        List<ScanResult> wifiList = wifi.getWifiList();
        String wifiString = "";
        for (ScanResult result : wifiList) {
            wifiString = /* wifiString + */ result.SSID /*+  "\n" */;
            SSIDList.add(wifiString);
        }
    }
    
    

/*************************************************************************************/

    private void _showWifiListWithBaseAdapter() {
        wifiSSIDList = new ArrayList<String>();
        getWifiSSID(wifiSSIDList);

        listView = (ListView)this.findViewById(R.id.wifi_hotspot_listview);

        ArrayList<HashMap<String, Object>> list = new ArrayList<HashMap<String,Object>>();
        for (String ssid: wifiSSIDList) {
            HashMap<String,Object> map = new HashMap<String, Object>();
            map.put("item_text", ssid);
            map.put("button_text", "off");
            list.add(map);
        }
        
        ListViewItemAdapter adapter = new ListViewItemAdapter(this, list);
        adapter.setClickedCallback(new ListViewClickedCallback());
        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new ListView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String text = "position:" + position + "id:" + id;
                Toast tmpToast = Toast.makeText(Activity_WifiControl.this, text, android.widget.Toast.LENGTH_SHORT);
                tmpToast.show();
            }
        });
    }
    
    private void _showWifiListWithSimpleAdapter() {
        wifiSSIDList = new ArrayList<String>();
        getWifiSSID(wifiSSIDList);

        listView = (ListView)this.findViewById(R.id.wifi_hotspot_listview);

        ArrayList<HashMap<String, Object>> list = new ArrayList<HashMap<String,Object>>();
        for (String ssid: wifiSSIDList) {
            HashMap<String,Object> map = new HashMap<String, Object>();
            map.put("item_text", ssid);
            map.put("button_text", "off-on");
            list.add(map);
        }

        //> ListView 中如果使用了 SimpleAdapter 自定义布局的话
        //> 如果自定义布局中保护 Button 等控件时，Button会抢占 ListView 的焦点
        //> 所以，在 Button 等控件的xml中，增加 android:focusable="false" 来解决这个问题
        SimpleAdapter adapter = new SimpleAdapter(
                                                  Activity_WifiControl.this, 
                                                  list, 
                                                  R.layout.item_listview_wifi, 
                                                  new String[] {"item_text","button_text"},
                                                  new int[] {R.id.item_textview_listview_wifi, R.id.item_button_listview_wifi});
        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new ListView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent,
                                    View view,
                                    int position,
                                    long id) {
                String text = "position:" + position + "id:" + id;
                Toast tmpToast = Toast.makeText(Activity_WifiControl.this, text, android.widget.Toast.LENGTH_SHORT);
                tmpToast.show();
            }
        });
    }

    private void _showWifiList() {
        wifiSSIDList = new ArrayList<String>();
        getWifiSSID(wifiSSIDList);

        listView = (ListView)this.findViewById(R.id.wifi_hotspot_listview);
        listView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_single_choice, wifiSSIDList));
        listView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
        listView.setOnItemClickListener(new ListView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String text = "position:" + position + "id:" + id;
                Toast tmpToast = Toast.makeText(Activity_WifiControl.this, text, android.widget.Toast.LENGTH_SHORT);
                tmpToast.show();
            }
        });
    }
    
/*************************************************************************************/
    public class ListViewClickedCallback implements ListViewItemAdapter.WidgetClickedCallback {
        @Override
        public void onClicked(CallbackData data) {
            String text = "--- position:" + data.nPosition;
            Toast tmpToast = Toast.makeText(Activity_WifiControl.this, text, android.widget.Toast.LENGTH_SHORT);
            tmpToast.show();
        }
        
    }
}
