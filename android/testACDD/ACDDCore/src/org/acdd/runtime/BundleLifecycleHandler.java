/*
 * ACDD Project
 * file BundleLifecycleHandler.java  is  part of ACCD
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
package org.acdd.runtime;

import android.annotation.SuppressLint;
import android.app.Application;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

import org.acdd.android.compat.ICrashReporter;
import org.acdd.framework.ACDD;
import org.acdd.framework.BundleImpl;
import org.acdd.framework.InternalConstant;
import org.acdd.hack.ACDDHacks;
import org.acdd.log.Logger;
import org.acdd.log.LoggerFactory;
import org.acdd.util.StringUtils;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleEvent;
import org.osgi.framework.ServicePermission;
import org.osgi.framework.SynchronousBundleListener;
/***This is ACDD framework  lifecycle  handle,mange  bundle  lifecycles**/
public class BundleLifecycleHandler implements SynchronousBundleListener {
    static final Logger log;

    static {
        log = LoggerFactory.getInstance("BundleLifecycleHandler");
    }

    @Override
    @SuppressLint({"NewApi"})
    public void bundleChanged(final BundleEvent bundleEvent) {
        switch (bundleEvent.getType()) {
            case BundleEvent.LOADED:
                loaded(bundleEvent.getBundle());
                break;
            case BundleEvent.INSTALLED:
                installed(bundleEvent.getBundle());
                break;
            case BundleEvent.STARTED:
                if (isLewaOS()) {
                    if (Looper.myLooper() == null) {
                        Looper.prepare();
                    }
                    started(bundleEvent.getBundle());
                } else /*if (Framework.isFrameworkStartupShutdown()) {
                    BundleLifecycleThread.getInstance().postTask(new Runnable() {
                        @Override
                        public void run() {
                            BundleLifecycleHandler.this.started(bundleEvent.getBundle());
                        }
                    });
                } else */{
                    started(bundleEvent.getBundle());
                }
                break;
            case BundleEvent.STOPPED:
                stopped(bundleEvent.getBundle());
                break;
            case BundleEvent.UPDATED:
                updated(bundleEvent.getBundle());
                break;
            case BundleEvent.UNINSTALLED:

            {
                uninstalled(bundleEvent.getBundle());
                break;
            }
            default:
        }
    }

    private void loaded(Bundle bundle) {
        long currentTimeMillis = System.currentTimeMillis();
        BundleImpl bundleImpl = (BundleImpl) bundle;
        try {
            DelegateResources.newDelegateResources(
                    RuntimeVariables.androidApplication,
                    RuntimeVariables.delegateResources, bundleImpl.getArchive().getArchiveFile().getAbsolutePath());
        } catch (Throwable e) {
            log.error("Could not load resource in bundle "
                    + bundleImpl.getLocation(), e);
        }
        if (DelegateComponent.getPackage(bundle.getLocation()) == null) {
            PackageLite parse = PackageLite.parse(bundleImpl.getArchive().getArchiveFile());
            log.info("Bundle installation info " + bundle.getLocation() + ":" + parse.components);
            DelegateComponent.putPackage(bundle.getLocation(), parse);
        }
        log.info("loaded() spend " + (System.currentTimeMillis() - currentTimeMillis) + " milliseconds");
    }

    private void installed(Bundle bundle) {
    }

    private void updated(Bundle bundle) {
    }

    private void uninstalled(Bundle bundle) {
        DelegateComponent.removePackage(bundle.getLocation());
    }

    private void started(Bundle bundle) {
        BundleImpl bundleImpl = (BundleImpl) bundle;
        long currentTimeMillis = System.currentTimeMillis();
        if (InternalConstant.CODE_ENABLE_COMPILE) {//no used OSGI.MF any more,disable compile this code
            String mBundleApplicationNames = bundleImpl.getHeaders().get("Bundle-Application");
            if (StringUtils.isNotEmpty(mBundleApplicationNames)) {
                String[] bundleApplications;
                String[] split = StringUtils.split(mBundleApplicationNames, ",");
                if (split == null || split.length == 0) {
                    bundleApplications = new String[]{mBundleApplicationNames};
                } else {
                    bundleApplications = split;
                }
                if (bundleApplications != null) {
                    for (String bundleApplication : bundleApplications) {
                        String trim = StringUtils.trim(bundleApplication);
                        if (StringUtils.isNotEmpty(trim)) {
                            try {
                                boolean needInit = true;
                                for (Application initedApplication : DelegateComponent.apkApplications.values()) {
                                    if (initedApplication.getClass().getName().equals(trim)) {
                                        needInit = false;
                                        break;
                                    }
                                }
                                if (needInit) {
                                    Application newApplication = newApplication(trim, bundleImpl.getClassLoader());
                                    newApplication.onCreate();
                                    DelegateComponent.apkApplications.put("system:" + trim, newApplication);
                                }
                            } catch (Throwable th) {
                                log.error("Error to start application", th);
                            }
                        }
                    }
                }
                return;
            }
        }
        {
            PackageLite packageLite = DelegateComponent.getPackage(bundleImpl.getLocation());
            if (packageLite != null) {
                String applicationClassName = packageLite.applicationClassName;
                if (StringUtils.isNotEmpty(applicationClassName)) {
                    Application application = null;
                    try {
                        application = newApplication(applicationClassName, bundleImpl.getClassLoader());
                    } catch (Exception e) {
                        log.error("Error to start application >>>", e);
                    }
                    if (application != null) {
                        try {
                            application.onCreate();
                        } catch (Exception ex) {
                            ACDD.getInstance().reportCrash(ICrashReporter.ACDD_CREATE_PLUGIN_APPLICATION_ERROR, ex);
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        log.info("started() spend " + (System.currentTimeMillis() - currentTimeMillis) + " milliseconds");
    }

    protected static Application newApplication(String applicationClassName, ClassLoader classLoader) throws Exception {
        Class loadClass = classLoader.loadClass(applicationClassName);
        if (loadClass == null) {
            throw new ClassNotFoundException(applicationClassName);
        }
        Application application = (Application) loadClass.newInstance();
        ACDDHacks.Application_attach.invoke(application, RuntimeVariables.androidApplication);
        return application;
    }

    private void stopped(Bundle bundle) {
        Application application = DelegateComponent.apkApplications.get(bundle.getLocation());
        if (application != null) {
            application.onTerminate();
            DelegateComponent.apkApplications.remove(bundle.getLocation());
        }
    }

    public void handleLowMemory() {
    }

    private boolean isLewaOS() {
        try {
            return StringUtils.isNotEmpty((String) Class
                    .forName("android.os.SystemProperties")
                    .getDeclaredMethod(ServicePermission.GET,
                            new Class[]{String.class})
                    .invoke(null, "ro.lewa.version"));
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    static class BundleLifecycleThread extends HandlerThread {
        private Handler mWorkerHandler;
        private static BundleLifecycleThread sInstance;

        public static synchronized BundleLifecycleThread getInstance() {
            if (sInstance == null) {
                sInstance = new BundleLifecycleThread();
                sInstance.start();
            }
            return sInstance;
        }

        private BundleLifecycleThread() {
            super("BundleLifecycleThread");
        }

        public void postTask(Runnable task) {
            if (mWorkerHandler == null) {
                mWorkerHandler = new Handler(getLooper());
            }
            mWorkerHandler.post(task);
        }
    }
}
