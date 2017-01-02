/*
 * ACDD Project
 * file ACDDApp.java  is  part of ACCD
 * The MIT License (MIT)  Copyright (c) 2015 Bunny Blue,achellies.
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 */
package org.acdd.android.compat;

import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteException;
import android.os.Build.VERSION;
import android.os.Process;
import android.text.TextUtils;
import android.util.Log;

import org.acdd.android.initializer.ACDDInitializer;
import org.acdd.android.initializer.BundleParser;
import org.acdd.runtime.Globals;
import org.acdd.runtime.ContextImplHook;
import org.acdd.util.ACDDUtils;

import java.lang.reflect.Field;

/****
 * ACDDApp, you can  extend  this class direct
 ****/
public abstract class ACDDApp extends Application {
//    private static final Handler mAppHandler;

//    private Context mBaseContext;
    ACDDInitializer mACDDInitializer;


    public ACDDApp() {
    }

//    public static void runOnUiThread(Runnable runnable) {
//        mAppHandler.post(runnable);
//    }

    /* (non-Javadoc)
     * @see android.content.ContextWrapper#attachBaseContext(android.content.Context)
     */
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
//        this.mBaseContext = base;
        BundleParser.parser(getBaseContext());

        try {
            Field declaredField = Globals.class
                    .getDeclaredField("sInstalledVersionName");
            declaredField.setAccessible(true);
            declaredField.set(null, base.getPackageManager()
                    .getPackageInfo(base.getPackageName(), 0).versionName);
        } catch (Exception e) {
            e.printStackTrace();
        }

        mACDDInitializer = new ACDDInitializer(this, getPackageName(), isPurgeUpdate());

        mACDDInitializer.init();
    }

    protected abstract boolean isPurgeUpdate();

    @Override
    public void onCreate() {
        super.onCreate();

        this.mACDDInitializer.startUp();

    }

    @Override
    public final boolean bindService(Intent intent,
                                     ServiceConnection serviceConnection, int flags) {
        return new ContextImplHook(getBaseContext(), null).
                bindService(intent, serviceConnection, flags);
    }

    @Override
    public final void startActivity(Intent intent) {
        new ContextImplHook(getBaseContext(), getClassLoader()).startActivity(intent);
    }


    @Override
    public final ComponentName startService(Intent intent) {
        try {
            return new ContextImplHook(getBaseContext(), null).startService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    @Override
    public final SQLiteDatabase openOrCreateDatabase(String name, int mode, CursorFactory cursorFactory) {
        String processName = ACDDUtils.getProcessNameByPID(Process.myPid());
        if (!TextUtils.isEmpty(processName)) {
            Log.i("SQLiteDatabase", processName);
            if (!processName.equals(getPackageName())) {
                String[] split = processName.split(":");
                if (split != null && split.length > 1) {
                    processName = split[1] + "_" + name;
                    Log.i("SQLiteDatabase", "openOrCreateDatabase:" + processName);
                    return hookDatabase(processName, mode, cursorFactory);
                }
            }
        }
        return hookDatabase(name, mode, cursorFactory);
    }

    private SQLiteDatabase hookDatabase(String name, int mode, CursorFactory cursorFactory) {
        if (VERSION.SDK_INT >= 11) {
            return super.openOrCreateDatabase(name, mode, cursorFactory);
        }
        try {
            return super.openOrCreateDatabase(name, mode, cursorFactory);
        } catch (SQLiteException e) {
            e.printStackTrace();
            if (Globals.getApplication().deleteDatabase(name)) {
                return super.openOrCreateDatabase(name, mode, cursorFactory);
            }
            return null;
        }
    }

//    static {
//        mAppHandler = new Handler();
//    }
}