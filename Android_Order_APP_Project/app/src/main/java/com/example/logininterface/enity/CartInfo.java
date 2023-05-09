package com.example.logininterface.enity;

//购物车信息
public class CartInfo {
    public int id;
    //商品编号
    public  int goodsid;
    //商品数量
    public int count;

    public CartInfo(){}

    public CartInfo(int id, int goodsid, int count){
        this.id = id;
        this.goodsid = goodsid;
        this.count = count;
    }
}
