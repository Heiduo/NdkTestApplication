package com.example.ndktestapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private final String TAG = getClass().getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.tv);
        tv.setText(NDKHelper.getNDKPrint());

        int [] tmpArray = {100,200,300,400};
        tv.setText(NDKHelper.sendIntArrayToJNI(tmpArray));

        for (int i : tmpArray) {
            Log.e(TAG,i+"");
        }
        
        String[] stringArray = {"张三","李四","王五"};
        tv.setText(NDKHelper.sendStringArrayToJNI(stringArray));

        User user = new User();
        user.setName("heiduo");
        Log.e(TAG,NDKHelper.parseUser(user) + ":" + user.getName());

        NDKHelper.createBean();
        downloadThread(this);
    }

    //在JNI中创建JNI线程，并实现线程中调用JAVA方法
    public static native void downloadThread(MainActivity activity);

    public void updateUI(){
        if (Looper.myLooper() == Looper.getMainLooper()){
            System.out.println("更新UI0");
        }else{
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    System.out.println("更新UI1");
                }
            });
        }
    }
}
