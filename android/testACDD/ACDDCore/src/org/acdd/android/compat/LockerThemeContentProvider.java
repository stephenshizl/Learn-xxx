package org.acdd.android.compat;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;

/**
 * Created by zhoukaifeng on 2016/1/14 19:56.
 * email: zhoukaifeng@conew.com.
 */
public class LockerThemeContentProvider extends ContentProvider{
    private ProviderProxy mProviderProxy;
    public LockerThemeContentProvider() {
        mProviderProxy = new ProviderProxy("ks.cm.antivirus.applock.lockertheme.LockerThemeContentProvider");
    }

    @Override
    public boolean onCreate() {
        return mProviderProxy.onCreate();
    }

    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        return mProviderProxy.query(uri, projection, selection, selectionArgs, sortOrder);
    }

    @Override
    public String getType(Uri uri) {
        return mProviderProxy.getType(uri);
    }

    @Override
    public Uri insert(Uri uri, ContentValues values) {
        return mProviderProxy.insert(uri, values);
    }

    @Override
    public int delete(Uri uri, String selection, String[] selectionArgs) {
        return mProviderProxy.delete(uri, selection, selectionArgs);
    }

    @Override
    public int update(Uri uri, ContentValues values, String selection, String[] selectionArgs) {
        return mProviderProxy.update(uri, values, selection, selectionArgs);
    }
}
