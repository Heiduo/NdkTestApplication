package com.example.ndktestapplication;

/**
 * 描述：
 *
 * @author Created by heiduo
 * @time Created on 2020/5/26
 */
public class User {
    public String name;

    @Override
    public String toString() {
        return "User{" +
                "name='" + name + '\'' +
                '}';
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public static void printInfo(){
        System.out.println("这是USER实体类");
    }
}
