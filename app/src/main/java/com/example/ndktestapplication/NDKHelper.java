package com.example.ndktestapplication;

import android.app.Activity;
import android.os.Looper;

/**
 * 描述：
 *
 * @author Created by heiduo
 * @time Created on 2020/5/25
 */
public class NDKHelper {
    static {
        System.loadLibrary("test_lib");
    }

    public static native String getNDKPrint();

    public static native String sendIntArrayToJNI(int[] intArray);

    public static native String sendStringArrayToJNI(String[] stringArray);

    //处理实体类
    public static native String parseUser(User user);

    //创建JAVA实体类
    public static native void createBean();

}
