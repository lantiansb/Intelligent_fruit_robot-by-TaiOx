package com.example.logininterface;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.CheckBox;
import android.widget.EditText;

import androidx.activity.result.ActivityResultLauncher;
import androidx.appcompat.app.AppCompatActivity;

import com.example.logininterface.database.ShoppingDBHelper;
import com.example.logininterface.enity.Logininfo;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity {

    //声明一个商品数据库帮助器对象
    private ShoppingDBHelper mHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Timer timer = new Timer();
        TimerTask task = new TimerTask(){
            @Override
            public void run() {
                //返回登录界面
                Intent intent = new Intent(MainActivity.this, LoginActivity.class);
                //设置启动标志，避免多次返回同一页面
                intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                startActivity(intent);
                MainActivity.this.finish();
            }
        };
        timer.schedule(task, 1000);
    }
    //启动Activity时启动数据库
    @Override
    protected void onStart() {
        super.onStart();
        mHelper = ShoppingDBHelper.getInstance(this);
        mHelper.openReadLink();
        mHelper.openWriteLink();

    }

    //关闭Activity时关闭数据库
    @Override
    protected void onStop() {
        super.onStop();
//        mHelper.closeLink();
    }

}