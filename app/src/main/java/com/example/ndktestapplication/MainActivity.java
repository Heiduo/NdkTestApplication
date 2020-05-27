package com.example.ndktestapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;

import static android.util.Half.less;

public class MainActivity extends AppCompatActivity {
    private final String TAG = getClass().getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.tv);

        /*tv.setText(NDKHelper.getNDKPrint());

        int [] tmpArray = {100,200,300,400};
        tv.setText(NDKHelper.sendIntArrayToJNI(tmpArray));

        for (int i : tmpArray) {
            Log.e(TAG,i+"");
        }
        
        String[] stringArray = {"张三","李四","王五"};
        tv.setText(NDKHelper.sendStringArrayToJNI(stringArray));

        User user = new User();
        user.setName("heiduo");
        Log.e(TAG,NDKHelper.parseUser(user) + ":" + user.getName());*/

//        NDKHelper.createBean();
//        downloadThread(this);

        ArrayList<String> arrayList = new ArrayList<>();
        AssetManager manager = getAssets();
        try {
            InputStream io = manager.open("row_data");
            InputStreamReader ior = new InputStreamReader(io);
            BufferedReader buf = new BufferedReader(ior);
            String str;
            while ((str = buf.readLine()) != null) {
                arrayList.add(str);
            }
            buf.close();
            ior.close();
            io.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        String [] splitData = arrayList.get(0).split(", ");
        double[] data = new double[splitData.length];
        for (int i = 0; i < splitData.length; i++) {
            data[i] = Double.parseDouble(splitData[i]);
        }
        Log.e(TAG,"data len:" + splitData.length);

        double [][] fm;
        if (NDKHelper.initAlgorithm(manager)){
            System.out.println("init true");
            fm = NDKHelper.applyAlgorithm(data);
            Log.e(TAG,"fm:" + fm.length);
            Log.e(TAG,"fm:" + Arrays.deepToString(fm));
        }else {
            System.out.println("init false");
        }


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
