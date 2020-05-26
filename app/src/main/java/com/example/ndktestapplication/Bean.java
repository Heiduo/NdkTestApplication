package com.example.ndktestapplication;

/**
 * 描述：
 *
 * @author Created by heiduo
 * @time Created on 2020/5/26
 */
public class Bean {
    public String info;

    public Bean(String info){
        this.info = info;
    }

    public String getInfo() {
        return info;
    }

    public void setInfo(String info) {
        this.info = info;
    }

    public void jniCall(){
        System.out.println(info);
    }
}
