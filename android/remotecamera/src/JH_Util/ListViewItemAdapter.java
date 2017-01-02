package JH_Util;

import java.util.ArrayList;
import java.util.HashMap;

import com.example.remotecamera.R;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;

//> 
//> 自定义的 adapter，可以实现捕获 ListView 的 item 中的 button 等控件的事件监听
//> 
public class ListViewItemAdapter extends BaseAdapter {
    private static final String ITEM_LISTVIEW__ON = "on";
    private static final String ITEM_LISTVIEW__OFF = "off";
    private static final String KEY_ITEM_TEXT = "item_text";
    private static final String KEY_BUTTON_TEXT = "button_text";
    
    private LayoutInflater mInflater;//得到一个LayoutInfalter对象用来导入布局
    private ArrayList<HashMap<String, Object>> listData;
    private WidgetClickedCallback clickedCallback;

    public class CallbackData {
        public int nPosition;
    }

    public abstract interface WidgetClickedCallback {
        abstract public void onClicked(CallbackData data);
    }

    private final class ViewHolder{
        public TextView text;
        public Button   bt;
    }

    public ListViewItemAdapter(Context context, ArrayList<HashMap<String, Object>> list) {
        this.mInflater = LayoutInflater.from(context);
        this.listData = list;
    }
    
    public void setClickedCallback(WidgetClickedCallback callback) {
        clickedCallback = callback;
    }

    @Override
    public int getCount() {
        return listData.size();
    }

    @Override
    public Object getItem(int position) {
        return listData.get(position);
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(final int position, View convertView, final ViewGroup parent) {
        ViewHolder holder;

        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.item_listview_wifi, null); //> 缓存 item 的 View，避免重复绘制
            holder = new ViewHolder();
            holder.bt = (Button)convertView.findViewById(R.id.item_button_listview_wifi);
            holder.text = (TextView)convertView.findViewById(R.id.item_textview_listview_wifi);
            convertView.setTag(holder); //> 是为了减少控件的 findViewById 的查询动作
        } else{
            holder = (ViewHolder)convertView.getTag();
        }

        holder.text.setText(listData.get(position).get(KEY_ITEM_TEXT).toString());
        holder.bt.setText(listData.get(position).get(KEY_BUTTON_TEXT).toString());

        holder.bt.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                Log.v("---------", "id : " + position);
                Button button = (Button)v;
                String value = new String();
                HashMap<String, Object> map = listData.get(position);

                value = ITEM_LISTVIEW__OFF;
                if (map.get(KEY_BUTTON_TEXT).toString().equals(ITEM_LISTVIEW__OFF)) {
                    value = ITEM_LISTVIEW__ON;
                    _resetOther(position, parent);
                }

                map.put(KEY_BUTTON_TEXT, value);
                button.setText(value);

                if (null != clickedCallback) {
                    CallbackData data = new CallbackData();
                    data.nPosition = position;
                    clickedCallback.onClicked(data);
                }
            }
        });

        return convertView;
    }
    
    private void _resetOther(int position, ViewGroup parent) {
        for (int i = 0; i < listData.size(); i++) {
            HashMap<String, Object> map = listData.get(i);
            if (i != position && !map.get(KEY_BUTTON_TEXT).equals(ITEM_LISTVIEW__OFF)) {
                map.put(KEY_BUTTON_TEXT, ITEM_LISTVIEW__OFF);
            }
        }

        for (int i = 0; i < parent.getChildCount(); i++) {
            ViewHolder holder;
            View view = parent.getChildAt(i);
            holder = (ViewHolder)view.getTag();
            if (null != holder && !holder.bt.getText().toString().equals(ITEM_LISTVIEW__OFF)) {
                holder.bt.setText(ITEM_LISTVIEW__OFF);
            }
        }
    }
}

