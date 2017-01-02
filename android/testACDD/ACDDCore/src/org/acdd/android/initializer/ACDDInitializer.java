/*
 * ACDD Project
 * file ACDDInitializer.java  is  part of ACCD
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
package org.acdd.android.initializer;

import android.annotation.SuppressLint;
import android.app.Application;
import android.content.Context;
import android.os.Build;
import android.os.Build.VERSION;
import android.text.TextUtils;
import android.util.Log;

import org.acdd.android.task.Coordinator;
import org.acdd.android.task.Coordinator.TaggedRunnable;
import org.acdd.framework.ACDD;
import org.acdd.framework.InternalConstant;
import org.acdd.runtime.Globals;
import org.acdd.bundleInfo.BundleInfoList;
import org.acdd.log.Logger;
import org.acdd.log.LoggerFactory;
import org.acdd.util.ApkUtils;

import java.util.Properties;

public class ACDDInitializer {
    Logger log = LoggerFactory.getInstance("ACDDInitializer");
    private static long initStartTime = 0;
    private static boolean inTargetApp;
    private Application mApplication;
    private String mPackageName;
    private BundleDebug mDebug;
    private boolean tryInstall;

    private Properties mProperties = new Properties();
    private boolean isUpdate = false;


    public ACDDInitializer(Application application, String packagename, boolean isUpdate) {
        this.mApplication = application;
        this.mPackageName = packagename;

        this.isUpdate = isUpdate;
        if (application.getPackageName().equals(packagename)) {
            inTargetApp = true;
        }
    }

    public void init() {

        initStartTime = System.currentTimeMillis();

        try {
            ACDD.getInstance().init(this.mApplication);
            log.debug("ACDD framework inited end " + this.mPackageName + " " + (System.currentTimeMillis() - initStartTime) + " ms");
        } catch (Throwable e) {
            Log.e("ACDDInitializer", "Could not init atlas framework !!!", e);
            throw new RuntimeException("atlas initialization fail" + e.getMessage());
        }
    }

    public void startUp() {
        this.mProperties.put(InternalConstant.BOOT_ACTIVITY, InternalConstant.BOOT_ACTIVITY);
        this.mProperties.put(InternalConstant.COM_ACDD_DEBUG_BUNDLES, "true");
        this.mProperties.put(InternalConstant.ACDD_APP_DIRECTORY, getAppDirectory(this.mApplication));

        try {
            Globals.init(this.mApplication, ACDD.getInstance().getDelegateClassLoader());
            this.mDebug = new BundleDebug();

            if (TextUtils.equals(mApplication.getPackageName(), mPackageName)) {
                if (!(verifyRuntime() || !ApkUtils.isRootSystem())) {
                    this.mProperties.put(InternalConstant.ACDD_PUBLIC_KEY, SecurityBundleListner.PUBLIC_KEY);
                    ACDD.getInstance().addBundleListener(new SecurityBundleListner());
                }
                if (this.isUpdate || this.mDebug.isDebugable()) {
                    this.mProperties.put("osgi.init", "true");
                }
            }

            BundlesInstaller mBundlesInstaller = BundlesInstaller.getInstance();
            OptDexProcess mOptDexProcess = OptDexProcess.getInstance();
            if (this.mApplication.getPackageName().equals(this.mPackageName) && (this.isUpdate || this.mDebug.isDebugable())) {
                mBundlesInstaller.init(this.mApplication, this.mDebug, inTargetApp);
                mOptDexProcess.init(this.mApplication);
            }

            log.debug("ACDD framework prepare starting in process %s %d ms", this.mPackageName, System.currentTimeMillis() - initStartTime);
            ACDD.getInstance().setClassNotFoundInterceptorCallback(new ClassNotFoundInterceptor());
            if (InstallPolicy.install_when_findclass && BundleInfoList.getInstance().getBundles() == null) {
                InstallPolicy.install_when_oncreate = true;
                this.tryInstall = true;
            }

            ACDD.getInstance().startup(mProperties);
            installBundles(mBundlesInstaller, mOptDexProcess);
            log.debug("ACDD framework end startUp in process %s %d ms", mPackageName, System.currentTimeMillis() - initStartTime);
        } catch (Throwable e) {
            throw new RuntimeException("Could not set Globals !!!", e);
        }
    }

    private String getAppDirectory(Context context) {
        try {
            return context.getFilesDir().getParent();
        } catch (Throwable e) {
            return "/data/data/"+ context.getPackageName() +"/";
        }
    }

    private void installBundles(final BundlesInstaller mBundlesInstaller, final OptDexProcess mOptDexProcess) {
        if (this.mDebug.isDebugable()) {
            InstallPolicy.install_when_oncreate = true;
        }

        if (this.mApplication.getPackageName().equals(this.mPackageName)) {
//            if (InstallPolicy.install_when_oncreate) {
//            }
            if (this.isUpdate || this.mDebug.isDebugable()) {
                if (InstallPolicy.install_when_oncreate) {
                    Coordinator.postTask(new TaggedRunnable("AtlasStartup") {
                        @Override
                        public void run() {
                            mBundlesInstaller.process(true, false);
                            mOptDexProcess.processPackages(true, false);

                        }
                    });

                    return;
                }
                Utils.notifyBundleInstalled(mApplication);
                Utils.updatePackageVersion(this.mApplication);
                Utils.saveAtlasInfoBySharedPreferences(this.mApplication);
            } else if (!this.isUpdate) {
                if (this.tryInstall) {
                    Coordinator.postTask(new TaggedRunnable("AtlasStartup") {
                        @Override
                        public void run() {
                            mBundlesInstaller.process(false, false);
                            mOptDexProcess.processPackages(false, false);
                        }
                    });
                } else {
                    Utils.notifyBundleInstalled(mApplication);
                }
            }
        }
    }

    @SuppressLint({"DefaultLocale"})
    private boolean verifyRuntime() {
        if ((Build.BRAND == null || !Build.BRAND.toLowerCase().contains("xiaomi") || Build.HARDWARE == null || !Build.HARDWARE.toLowerCase().contains("mt65")) && VERSION.SDK_INT >= 14) {
            return false;
        }
        return true;
    }
}
