package com.example.logininterface;

import android.app.Application;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;

import androidx.annotation.NonNull;

import com.example.logininterface.database.ShoppingDBHelper;
import com.example.logininterface.enity.GoodsInfo;
import com.example.logininterface.util.Fileutil;
import com.example.logininterface.util.SharedUtil;

import java.io.File;
import java.util.HashMap;
import java.util.List;

public class MyApplication extends Application {

    private static MyApplication mApp;

    //声明一个公开的信息映射对象，可当作全局变量使用
    public HashMap<String,String> infoMap = new HashMap<>();

    public static MyApplication getInstance(){
        return mApp;
    }

    //在App启动时调用
    @Override
    public void onCreate() {
        super.onCreate();
        mApp = this;

        //初始化商品信息
        initgoodsInfo();
    }

    private void initgoodsInfo() {
        //获取共享参数保存的是否首次打开
        boolean isFirst =  SharedUtil.getInstance(this).readBoolean("first",true);
        //获取当前App的私有下载路径(separatorChar是分隔符)
        String directory = getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).toString() + File.separatorChar;
        //外部公共存储
//        String directory = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).toString() + File.separatorChar;
        if(isFirst){
            //模拟网络图片下载
            List<GoodsInfo> list = GoodsInfo.getDefaultList();
            for (GoodsInfo info : list) {
                Bitmap bitmap = BitmapFactory.decodeResource(getResources(),info.pic);
                String path = directory + info.id + ".png";
                //往存储卡保存商品图片
                Fileutil.saveImage(path,bitmap);
                //回收位图对象
                bitmap.recycle();
                info.picPath = path;
            }
            //打开数据库，把商品信息插入到表中
            ShoppingDBHelper dbHelper = ShoppingDBHelper.getInstance(this);
            //开启写连接
            dbHelper.openWriteLink();
            //写入数据
            dbHelper.insertGoodsInfos(list);
            //关闭连接
            dbHelper.closeLink();
            //把是否首次打开写入共享参数
            SharedUtil.getInstance(this).writeBoolean("first",false);
        }
    }

    //在App终止时调用
    @Override
    public void onTerminate() {
        super.onTerminate();
    }

    //在配置改变时调用，比如横屏变成竖屏
    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }
}
