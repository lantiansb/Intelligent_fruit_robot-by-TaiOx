package com.example.logininterface.util;

import android.content.Context;
import android.content.SharedPreferences;

public class SharedUtil {
    private static SharedUtil mUtil;
    private SharedPreferences preferences;

    //单例方法
    public static SharedUtil getInstance(Context ctx){
        if(mUtil == null){
            mUtil = new SharedUtil();
            //初始化preferences
            mUtil.preferences = ctx.getSharedPreferences("shopping",Context.MODE_PRIVATE);
        }
        return mUtil;
    }

    //快捷方法---写
    public void writeBoolean(String key,boolean value){
        SharedPreferences.Editor editor = preferences.edit();
        editor.putBoolean(key,value);
        editor.commit();
    }

    //快捷方法---读
    public boolean readBoolean(String key,boolean defaultValue){
        return preferences.getBoolean(key,defaultValue);
    }
}
