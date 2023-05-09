package com.example.logininterface.enity;

import com.example.logininterface.R;

import java.util.ArrayList;

public class GoodsInfo {
    public  int id;
    //名称
    public String name;
    //描述
    public String description;
    //价格
    public float price;
    //大图保存的路径
    public String picPath;
    //大图的资源编号
    public int pic;
    //声明一个水果商品的名称数组
    private static String[] mNameArray = {
            "苹果","橙橘","青橘"
    };
    //声明一个水果商品的描述数组
    private static String [] mDescArray = {
            "一种红色的水果","一种橙色的水果","一种青色的水果"
    };
    //声明一个水果商品的价格数组
    private static float[] mPriceArray = {1,2,3};
    //声明一个手机商品的大图数组
    private static int[] mPicArray = {
            R.drawable.apple,
            R.drawable.orange,
            R.drawable.qingju
    };
    //商品信息列表
    public static ArrayList<GoodsInfo> getDefaultList(){
        ArrayList<GoodsInfo> goodslist = new ArrayList<>();
        for (int i = 0; i < mNameArray.length; i++) {
            GoodsInfo info = new GoodsInfo();
            info.id = i;
            info.name = mNameArray[i];
            info.description = mDescArray[i];
            info.price = mPriceArray[i];
            info.pic = mPicArray[i];
            goodslist.add(info);
        }
        return goodslist;
    }

    @Override
    public String toString() {
        return "GoodsInfo{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", description='" + description + '\'' +
                ", price=" + price +
                ", picPath='" + picPath + '\'' +
                ", pic=" + pic +
                '}';
    }
}
