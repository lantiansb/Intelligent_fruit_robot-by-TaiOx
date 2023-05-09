package com.example.logininterface.database;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import androidx.annotation.NonNull;

import com.example.logininterface.enity.CartInfo;
import com.example.logininterface.enity.GoodsInfo;
import com.example.logininterface.enity.Logininfo;

import java.util.ArrayList;
import java.util.List;

public class ShoppingDBHelper extends SQLiteOpenHelper {
    private static final String DB_NAME = "shopping.db";//数据库的名字
    //登录信息表
    private static final String TABLE_LOGIN_INFO = "login_info";
    //商品信息表
    private static final String TABLE_GOODS_INFO = "goods_info";
    //购物车信息表
    private static final String TABLE_CART_INFO = "cart_info";
    private static final int DB_VERSION = 1;//数据库的版本
    private static ShoppingDBHelper mHelper = null;
    private SQLiteDatabase mRDB = null;
    private SQLiteDatabase mWDB = null;

    //子类中调用父类的构造方法，像父类传参
    private ShoppingDBHelper(Context context){
        super(context,DB_NAME,null,DB_VERSION);
    }
    //利用单例模式获取数据库帮助器的唯一实例
    public static ShoppingDBHelper getInstance(Context context){
        if(mHelper == null){
            mHelper = new ShoppingDBHelper(context);
        }
        return mHelper;
    }
    //打开数据库的读连接
    public SQLiteDatabase openReadLink(){
        if(mRDB == null || !mRDB.isOpen()){
            mRDB = mHelper.getReadableDatabase();
        }
        return mRDB;
    }
    //打开数据库的写连接
    public SQLiteDatabase openWriteLink(){
        if(mWDB == null || !mWDB.isOpen()){
            mWDB = mHelper.getWritableDatabase();
        }
        return mWDB;
    }
    //关闭数据库连接
    public void closeLink(){
        if(mRDB != null && mRDB.isOpen()){
            mRDB.close();
            mRDB = null;
        }
        if(mWDB != null && mWDB.isOpen()){
            mWDB.close();
            mWDB = null;
        }
    }

    //创建数据库，执行建表语句
    @Override
    public void onCreate(@NonNull SQLiteDatabase db) {
        //创建用户信息表
        String sql = "CREATE TABLE IF NOT EXISTS " +TABLE_LOGIN_INFO+
                "(_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," +
                " phone VARCHAR NOT NULL," +
                " password VARCHAR NOT NULL," +
                " remember INTEGER NOT NULL);";
        db.execSQL(sql);
        //创建商品信息表，此处id前面的_为了SQL可移植性。
        sql = "CREATE TABLE IF NOT EXISTS " +TABLE_GOODS_INFO +
                "(_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," +
                " name VARCHAR NOT NULL," +
                " description VARCHAR NOT NULL," +
                " price FLOAT NOT NULL," +
                " pic_path VARCHAR NOT NULL);";
        db.execSQL(sql);
        //创建购物车信息表
        sql = "CREATE TABLE IF NOT EXISTS " +TABLE_CART_INFO+
                "(_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," +
                " goods_id INTEGER NOT NULL," +
                " count INTEGER NOT NULL);";
        db.execSQL(sql);
    }

    //记住密码
    public void savaLoginInfo(Logininfo info){
        //如果存在则先删除，再添加
        try {
            mWDB.beginTransaction();
            deleteLoginInfo(info);
            insertLoginInfos(info);
            mWDB.setTransactionSuccessful();
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            mWDB.endTransaction();
        }
    }
    public long deleteLoginInfo(Logininfo info){
        return mWDB.delete(TABLE_LOGIN_INFO,"phone=?",new String[]{info.phone});
    }
    //添加登录信息
    public long insertLoginInfos(Logininfo info){
        ContentValues values = new ContentValues();
        values.put("phone",info.phone);
        values.put("password",info.password);
        values.put("remember",info.remember);
        return mWDB.insert(TABLE_LOGIN_INFO,null,values);
    }
    //版本更新
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
    }

    //读取最后一个用户登录的信息
    public Logininfo queryTopLoginInfo(){
        Logininfo info = null;
        String sql = "select * from "+TABLE_LOGIN_INFO + " where remember = 1 ORDER BY _id DESC limit 1";
        //执行记录查询动作，该语句返回结果集游标
        Cursor cursor = mRDB.rawQuery(sql,null);
        if(cursor.moveToNext()){
            info = new Logininfo();
            info.id = cursor.getInt(0);
            info.phone = cursor.getString(1);
            info.password = cursor.getString(2);
            info.remember = (cursor.getInt(3) == 0) ? false : true;
        }
        return info;
    }

    public Logininfo queryByPhone(String phone){
        Logininfo info = null;
        String sql = "select * from "+TABLE_LOGIN_INFO;
        //执行记录查询动作，该语句返回结果集游标
        Cursor cursor = mRDB.query(TABLE_LOGIN_INFO,null,"phone=? and remember=1",new String[]{phone},null,null,null);
        if(cursor.moveToNext()){
            info = new Logininfo();
            info.id = cursor.getInt(0);
            info.phone = cursor.getString(1);
            info.password = cursor.getString(2);
            info.remember = (cursor.getInt(3) == 0) ? false : true;
        }
        return info;
    }

    //添加多条商品信息
    public void insertGoodsInfos(List<GoodsInfo> list){
        //插入多条记录，要么全部成功，要么全部失败，使用事务管理
        try {
            mWDB.beginTransaction();
            for (GoodsInfo info : list) {
                ContentValues values = new ContentValues();
                values.put("name",info.name);
                values.put("description",info.description);
                values.put("price",info.price);
                values.put("pic_path",info.picPath);
                mWDB.insert(TABLE_GOODS_INFO,null,values);
            }
            mWDB.setTransactionSuccessful();
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            mWDB.endTransaction();
        }
    }
    //添加密码到用户信息表
    public Logininfo insertLoginInfo(String phone) {
        //如果不存在，添加一条信息
        Logininfo logininfo = queryLoginInfoById(queryTopLoginInfo().id);
        ContentValues values = new ContentValues();
        values.put("phone",phone);
        if(logininfo == null){
            values.put("password",111111);
            mWDB.insert(TABLE_LOGIN_INFO,null,values);
        }else {
            //如果已存在，更新信息
            values.put("_id",logininfo.id);
            values.put("password",logininfo.password);
            mWDB.update(TABLE_LOGIN_INFO,values,"phone=?",new String[]{String.valueOf(logininfo.phone)});
        }
        return logininfo;
    }

    //查询所有的商品信息
    public List<GoodsInfo> queryAllGoodsInfo(){
        //执行命令
        String sql = "select * from "+TABLE_GOODS_INFO;
        List<GoodsInfo> list = new ArrayList<>();
        Cursor cursor = mRDB.rawQuery(sql,null);
        while (cursor.moveToNext()){
            GoodsInfo info = new GoodsInfo();
            info.id = cursor.getInt(0);
            info.name = cursor.getString(1);
            info.description = cursor.getString(2);
            info.price = cursor.getFloat(3);
            info.picPath = cursor.getString(4);
            //添加到list
            list.add(info);
        }
        //关闭游标
        cursor.close();
        return list;
    }

    //添加商品到购物车
    public void insertCartInfos(int goodsid) {
        //如果购物车不存在该商品，添加一条信息
        CartInfo cartInfo = queryCartInfoByGoodsId(goodsid);
        ContentValues values = new ContentValues();
        values.put("goods_id",goodsid);
        if(cartInfo == null){
            values.put("count",1);
            mWDB.insert(TABLE_CART_INFO,null,values);
        }else {
            //如果购物车中已存在该商品信息，更新商品信息
            values.put("_id",cartInfo.id);
            values.put("count",++cartInfo.count);
            mWDB.update(TABLE_CART_INFO,values,"_id=?",new String[]{String.valueOf(cartInfo.id)});
        }
    }
    //根据电话ID查询信息
    public Logininfo queryLoginInfoById(int id) {
        //获取一个游标
        Cursor cursor = mRDB.query(TABLE_LOGIN_INFO, null, "_id=?", new String[]{String.valueOf(id)}, null, null, null);
        Logininfo info = null;
        if(cursor.moveToNext()){
            info = new Logininfo();
            info.id = cursor.getInt(0);//根据列索引
            info.phone = cursor.getString(1);
            info.password = cursor.getString(2);
            info.remember = (cursor.getInt(3) == 0)? false : true;
        }
        return info;
    }

    public List<Logininfo> queryAllLoginInfo(){
        //执行命令
        String sql = "select * from "+TABLE_LOGIN_INFO;
        List<Logininfo> list = new ArrayList<>();
        Cursor cursor = mRDB.rawQuery(sql,null);
        while (cursor.moveToNext()){
            Logininfo info = new Logininfo();
            info.id = cursor.getInt(0);//根据列索引
            info.phone = cursor.getString(1);
            info.password = cursor.getString(2);
            info.remember = (cursor.getInt(3) == 0)? false : true;
            //添加到list
            list.add(info);
        }
        //关闭游标
        cursor.close();
        return list;
    }

    //根据商品信息ID查询购物车信息
    private CartInfo queryCartInfoByGoodsId(int goodsid) {
        //获取一个游标
        Cursor cursor = mRDB.query(TABLE_CART_INFO, null, "goods_id=?", new String[]{String.valueOf(goodsid)}, null, null, null);
        CartInfo info = null;
        if(cursor.moveToNext()){
            info = new CartInfo();
            info.id = cursor.getInt(0);//根据列索引
            info.goodsid = cursor.getInt(1);
            info.count = cursor.getInt(2);
        }
        return info;
    }

    //统计购物车商品的总数量
    public int countCartInfo() {
        int count = 0;
        String sql = "select sum(count) from "+TABLE_CART_INFO;
        Cursor cursor = mRDB.rawQuery(sql, null);
        if(cursor.moveToNext()){
            count = cursor.getInt(0);
        }
        return count;
    }

    //查询购物车中所有的信息列表
    public List<CartInfo> queryAllCartInfo() {
        List<CartInfo> list = new ArrayList<>();
        Cursor cursor = mRDB.query(TABLE_CART_INFO, null, null, null, null, null, null);
        while (cursor.moveToNext()){
            CartInfo info = new CartInfo();
            info.id = cursor.getInt(0);//根据列索引
            info.goodsid = cursor.getInt(1);
            info.count = cursor.getInt(2);
            list.add(info);
        }
        return list;
    }

    //根据商品ID查询商品信息
    public GoodsInfo queryGoodsInfoById(int goodsid) {
        GoodsInfo info = null;
        Cursor cursor = mRDB.query(TABLE_GOODS_INFO, null, "_id=?", new String[]{String.valueOf(goodsid)}, null, null, null);
        if(cursor.moveToNext()){
            info = new GoodsInfo();
            info.id = cursor.getInt(0);
            info.name = cursor.getString(1);
            info.description = cursor.getString(2);
            info.price = cursor.getFloat(3);
            info.picPath = cursor.getString(4);
        }
        return info;
    }
}
