package com.example.remotecamera;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;


public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_0);
        _setMainButtonsListener();
    } 

/****************************************************************************************/
    
    private boolean _setMainButtonsListener() {
    	Button btnLocalCamera = (Button)this.findViewById(R.id.button_local_camera);
    	Button btnRemoteCamera = (Button)this.findViewById(R.id.button_remote_camera);
    	Button btnCreateHotspot = (Button)this.findViewById(R.id.button_create_hotspot);
    	Button btnConnectHotspot = (Button)this.findViewById(R.id.button_connect_hotspot);

    	btnLocalCamera.setOnClickListener(new Button.OnClickListener() {
    		public void onClick(View v)
        	{
    			Intent intent = new Intent(MainActivity.this, Activity_LocalCamera.class);
    			startActivity(intent);
        	}
    	});
    	
    	btnRemoteCamera.setOnClickListener(new Button.OnClickListener() {
    		public void onClick(View v)
        	{
    			Toast toast = Toast.makeText(MainActivity.this, "hi, not impliment RemoteCamera", android.widget.Toast.LENGTH_SHORT);
    			toast.show();
        	}
    	});
    	
    	btnCreateHotspot.setOnClickListener(new Button.OnClickListener() {
    		public void onClick(View v)
        	{
    		    Intent intent = new Intent(MainActivity.this, Activity_WifiControl.class);
                startActivity(intent);
        	}
    	});
    	
    	// btnConnectHotspot.setOnClickListener((OnClickListener) this);
    	return true;
    }
}
