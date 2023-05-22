package com.example.logininterface;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.example.logininterface.database.ShoppingDBHelper;
import com.example.logininterface.enity.GoodsInfo;
import com.example.logininterface.util.ClickFilter;

import java.util.List;

public class ShoppingGoodsActivity extends AppCompatActivity implements View.OnClickListener {

    //声明一个商品数据库帮助器对象
    private ShoppingDBHelper mHelper;
    private TextView tv_count;

    //购物车总数量
    private int count = 0;
    //苹果数量
    private int count_apple = 0;
    //橙橘数量
    private int count_orange = 0;
    //青橘数量
    private int count_qingju = 0;

    private ActivityResultLauncher<Intent> register;
    private TextView tv_price_apple;
    private TextView tv_price_orange;
    private TextView tv_price_qingju;
    private GoodsInfo info_apple;
    private GoodsInfo info_orange;
    private GoodsInfo info_qingju;
    private TextView tv_count_qingju;
    private TextView tv_count_orange;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shopping_goods);

        //设置标题
        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("水果挑选区");
        //获取购物车数量
        tv_count = findViewById(R.id.tv_count);
        //取得价格显示区
        tv_price_apple = findViewById(R.id.tv_price_apple);
        tv_price_orange = findViewById(R.id.tv_price_orange);
        tv_price_qingju = findViewById(R.id.tv_price_qingju);
        //获取水果单个数量
        tv_count_orange = findViewById(R.id.tv_count_orange);
        tv_count_qingju = findViewById(R.id.tv_count_qingju);
        //设置监听
        findViewById(R.id.iv_back).setOnClickListener(this);
        findViewById(R.id.iv_cart).setOnClickListener(this);
        findViewById(R.id.iv_cart_come).setOnClickListener(this);
        findViewById(R.id.iv_message).setOnClickListener(this);
        findViewById(R.id.iv_myself).setOnClickListener(this);

        findViewById(R.id.btn_add_apple).setOnClickListener(this);
        findViewById(R.id.btn_add_orange).setOnClickListener(this);
        findViewById(R.id.btn_add_qingju).setOnClickListener(this);
        findViewById(R.id.btn_delete_orange).setOnClickListener(this);
        findViewById(R.id.btn_delete_qingju).setOnClickListener(this);

        //获取返回信息
        register = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), new ActivityResultCallback<ActivityResult>() {
            @Override
            public void onActivityResult(ActivityResult result) {
//                Intent intent = result.getData();
//                if(intent != null && result.getResultCode() == Activity.RESULT_OK){
//                }
            }
        });

        //获取返回的购物车总数量
        tv_count.setText(getIntent().getStringExtra("count"));
        tv_count.setText("0");
    }

    //启动Activity时启动数据库
    @Override
    protected void onStart() {
        mHelper = ShoppingDBHelper.getInstance(this);
        super.onStart();
        mHelper.openReadLink();
        mHelper.openWriteLink();
        //获取商品数据库的价格
        String desc = "优惠后:￥";
        //获取苹果价格
        info_apple = mHelper.queryGoodsInfoById(1);
        tv_price_apple.setText(desc + String.valueOf(info_apple.price));
        //获取橙橘价格
        info_orange = mHelper.queryGoodsInfoById(2);
        tv_price_orange.setText(desc + String.valueOf(info_orange.price));
        //获取青橘价格
        info_qingju = mHelper.queryGoodsInfoById(3);
        tv_price_qingju.setText(desc + String.valueOf(info_qingju.price));
    }

    //关闭Activity时关闭数据库
    @Override
    protected void onStop() {
        super.onStop();
//        mHelper.closeLink();
    }

    @Override
    public void onClick(View v) {
        if(v.getId() == R.id.iv_back){
            //返回登录界面
            Intent intent = new Intent(this,LoginActivity.class);
            //设置启动标志，避免多次返回同一页面
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
        }
        if(v.getId() == R.id.iv_cart_come){
            //前往购物车界面
            Intent intent = new Intent(this, ShoppingCartActivity.class);
            //以下携带购物车数量跳转到购物车页面
            intent.putExtra("count",tv_count.getText().toString());
            intent.putExtra("count_orange",String.valueOf(count_orange));
            intent.putExtra("count_qingju",String.valueOf(count_qingju));
            register.launch(intent);
        }
        if(v.getId() == R.id.iv_cart){
            //前往购物车界面
            Intent intent = new Intent(this, ShoppingCartActivity.class);
            //以下携带购物车数量跳转到购物车页面
            intent.putExtra("count",tv_count.getText().toString());
//            intent.putExtra("count_apple",String.valueOf(count_apple));
            intent.putExtra("count_orange",String.valueOf(count_orange));
            intent.putExtra("count_qingju",String.valueOf(count_qingju));
            //传以下数据会闪退，原因不明
            //以下携带水果单价跳转到购物车页面
//            intent.putExtra("price_apple",info_apple.price);
//            intent.putExtra("price_orange",info_orange.price);
//            intent.putExtra("price_qingju",info_qingju.price);
            register.launch(intent);
//            //设置启动标志，避免多次返回同一页面
//            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
//            startActivity(intent);
        }
        if(v.getId() == R.id.btn_add_apple){
            //将苹果添加到购物车中
//            count_apple++;
//            count++;
//            tv_count.setText(String.valueOf(count));
            if (ClickFilter.filter()) {//防止重复点击
                Toast.makeText(this, "已售罄", Toast.LENGTH_SHORT).show();
            }
        }
        if(v.getId() == R.id.btn_add_orange){
            //橙橘添加到购物车
            count_orange++;
            count++;
            tv_count.setText(String.valueOf(count));
            tv_count_orange.setText(String.valueOf(count_orange));
        }
        if(v.getId() == R.id.btn_add_qingju){
            //青橘添加到购物车
            count_qingju++;
            count++;
            tv_count.setText(String.valueOf(count));
            tv_count_qingju.setText(String.valueOf(count_qingju));
        }
        if(v.getId() == R.id.btn_delete_orange){
            //减少橙橘数量
            if(count_orange > 0) {
                count_orange--;
                count--;
                tv_count.setText(String.valueOf(count));
                tv_count_orange.setText(String.valueOf(count_orange));
            }
        }
        if(v.getId() == R.id.btn_delete_qingju){
            //减少青橘数量
            if(count_qingju > 0) {
                count_qingju--;
                count--;
                tv_count.setText(String.valueOf(count));
                tv_count_qingju.setText(String.valueOf(count_qingju));
            }
        }
        if(v.getId() == R.id.iv_message){
            if (ClickFilter.filter()) {//防止重复点击
                //提示暂未开通此功能
                Toast.makeText(this, "暂未开通此功能", Toast.LENGTH_SHORT).show();
            }
        }
        if(v.getId() == R.id.iv_myself){
            if (ClickFilter.filter()) {//防止重复点击
                //提示暂未开通此功能
                Toast.makeText(this, "暂未开通此功能", Toast.LENGTH_SHORT).show();
            }
        }
    }
}