package com.example.ndktestapplication;

/**
 * 描述：
 *
 * @author Created by heiduo
 * @time Created on 2020/5/25
 */
public class NDKHelper {
    static {
        System.loadLibrary("main");
    }
    public native String getNDKPrint();
}
