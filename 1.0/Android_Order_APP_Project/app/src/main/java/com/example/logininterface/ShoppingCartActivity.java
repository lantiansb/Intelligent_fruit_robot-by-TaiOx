package com.example.logininterface;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Handler;
import android.os.Message;

import com.example.logininterface.database.ShoppingDBHelper;
import com.example.logininterface.enity.GoodsInfo;
import com.example.logininterface.enity.Logininfo;
import com.example.logininterface.util.ClickFilter;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.List;

public class ShoppingCartActivity extends AppCompatActivity implements View.OnClickListener {

    //声明一个商品数据库帮助器对象
    private ShoppingDBHelper mHelper;

    private MqttClient client;
    private Handler handler;
    //连接信息
    private  String host = "tcp://183.230.40.39:6002";    // TCP协议
    private  String userName = "592489";             //用户名,产品id
    private  String passWord = "ccvV4Fx5OEnq1tS9IKf5iPR=zmM=";            //密码
    //    private String mqtt_id = "1068836997";          //客户端id
    private String mqtt_id = "1079136185";          //客户端id
    private String mqtt_sub_topic = "/deviceA/commands";       //订阅主题
    private MqttConnectOptions options;
    private String order = "hhhhhhhh";          //订单数据

    //声明信息显示框
    private TextView tv_cart;
    private TextView tv_connect;
    //用户手机号
    private String phone;
    //订单总数量
    private String mCount;
    //苹果数量
    private String mCountApple = "0";
    //橙橘数量
    private String mCountOrange;
    //青橘数量
    private String mCountQingju;

    //要记得同步数据库修改
    //苹果单价
    private String mPriceApple = "1";
    //橙橘单价
    private String mPriceOrange = "2";
    //青橘单价
    private String mPriceQingju = "3";
    private float total_prices;

    private ActivityResultLauncher<Intent> register;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shopping_cart);

        //初始化mqtt
        Mqtt_init();

        //获取订单信息显示框
        tv_cart = findViewById(R.id.tv_cart);
        //获取网络信息显示框
//        tv_connect = findViewById(R.id.tv_connect);
        //获取标题，并设置为购物车
        TextView tv_title = findViewById(R.id.tv_title);
        tv_title.setText("购物车");

        //设置监听事件
        findViewById(R.id.iv_back).setOnClickListener(this);
        findViewById(R.id.iv_niushi).setOnClickListener(this);
        findViewById(R.id.btn_order).setOnClickListener(this);
        findViewById(R.id.iv_home).setOnClickListener(this);
        findViewById(R.id.iv_message).setOnClickListener(this);
        findViewById(R.id.iv_myself).setOnClickListener(this);
        findViewById(R.id.iv_setting).setOnClickListener(this);

        //从上一个页面获取购物车总数量
        mCount = getIntent().getStringExtra("count");
        //获取水果的点单信息
//        mCountApple = getIntent().getStringExtra("count_apple");
        mCountOrange = getIntent().getStringExtra("count_orange");
        mCountQingju = getIntent().getStringExtra("count_qingju");
//        获取水果单价,通过数据库获取则无法打开此界面,于是注释掉
//        mPriceApple = getIntent().getStringExtra("price_apple");
//        mPriceOrange = getIntent().getStringExtra("price_orange");
//        mCountQingju = getIntent().getStringExtra("price_qingju");
//        //计算总金额
//        //苹果总金额
        float total_prices_apple = Integer.parseInt(mCountApple) * Float.parseFloat(mPriceApple);
//        //橙橘总金额
        float total_prices_orange = Integer.parseInt(mCountOrange) * Float.parseFloat(mPriceOrange);
//        //青橘总金额
        float total_prices_qingju = Integer.parseInt(mCountQingju) * Float.parseFloat(mPriceQingju);

//        //总价
        total_prices = total_prices_apple + total_prices_orange + total_prices_qingju;
    }

    //启动Activity时启动数据库
    @Override
    protected void onStart() {
        mHelper = ShoppingDBHelper.getInstance(this);
        super.onStart();
        mHelper.openReadLink();
        mHelper.openWriteLink();
        //获取数据库信息，写入tv_cart
        //获取用户手机号码
        List<Logininfo> list = mHelper.queryAllLoginInfo();
        for (Logininfo info : list) {
            phone = info.phone;
        }
//        Toast.makeText(this, phone, Toast.LENGTH_SHORT).show();
        //获取商品数据库的价格
//        GoodsInfo info_apple = mHelper.queryGoodsInfoById(1);
//        mPriceApple = String.valueOf(info_apple.price);
//        GoodsInfo info_orange = mHelper.queryGoodsInfoById(2);
//        mPriceOrange = String.valueOf(info_orange.price);
//        GoodsInfo info_qingju = mHelper.queryGoodsInfoById(3);
//        mPriceQingju = String.valueOf(info_qingju.price);
        //获取商品信息
        String ge = "斤";
        String desc = "用户" + phone + "购买了：\n" + mCountApple + ge + "樱桃,\n" + mCountOrange + ge + "橙橘,\n"+ mCountQingju + ge + "青橘,\n"
                + "一共消费了" + total_prices + "元";
        tv_cart.setText(desc);
//        Toast.makeText(this, "确认订单信息是否正确；如需下单，请先连接服务器，再进行下单。", Toast.LENGTH_SHORT).show();

        //接受回传的数据
        register = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), new ActivityResultCallback<ActivityResult>() {
            @Override
            public void onActivityResult(ActivityResult result) {
//                Intent intent = result.getData();
//                if(intent != null && result.getResultCode() == Activity.RESULT_OK){
//                }
            }
        });

    }

    //关闭Activity时关闭数据库
    @Override
    protected void onStop() {
        super.onStop();
//        mHelper.closeLink();
    }

    @Override
    public void onClick(View v) {
        if(v.getId() == R.id.btn_order) {
            if (ClickFilter.filter()) {//防止重复点击
                //下单功能
                //获取订单信息,处理为数据帧格式
                order = "ID:" + phone + ";type:1,num:" + mCountOrange + ";type:2,num:" + mCountQingju;  //订阅并发送数据
                //连接mqtt——onenet服务器
                try {
                    client.connect(options);
                    if (client.isConnected()) {
//                        if (ClickFilter.filter()) {//防止重复点击
                            //同步到显示区
    //                        Toast.makeText(this, "连接成功", Toast.LENGTH_SHORT).show();
//                        }
                    }
                } catch (MqttException e) {
                    e.printStackTrace();
                }
                //如果连接到网络，进行发送
                if (client.isConnected()) {
                    try {
    //                    if (ClickFilter.filter()) {//防止重复点击
                        Mqtt_publishMessage(mqtt_sub_topic, order);
                        //同步至网络显示区
//                        Toast.makeText(this, "下单成功", Toast.LENGTH_SHORT).show();
    //                    }
                        if (client.isConnected()) {
                            client.disconnect();
                            if(!client.isConnected()) {//已经断开连接
//                            Toast.makeText(this, "已断开连接", Toast.LENGTH_SHORT).show();
                                Toast.makeText(this, "下单成功", Toast.LENGTH_SHORT).show();
                            }
                        }
                    } catch (MqttException e) {
                        e.printStackTrace();
                    }
                } else {
    //                Toast.makeText(this, "未连接到服务器，下单失败", Toast.LENGTH_SHORT).show();
                    Toast.makeText(this, "未连接到服务器，正在尝试连接", Toast.LENGTH_SHORT).show();
                    try {
                        client.connect(options);
                        if (client.isConnected()) {
                            Mqtt_publishMessage(mqtt_sub_topic, order);
//                            Toast.makeText(this, "连接成功，下单成功", Toast.LENGTH_SHORT).show();
                        }
                        if (client.isConnected()) {
                            client.disconnect();
//                            Toast.makeText(this, "已断开连接", Toast.LENGTH_SHORT).show();
                            Toast.makeText(this, "重新连接成功，下单成功", Toast.LENGTH_SHORT).show();
                        }
                    } catch (MqttException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
        if(v.getId() == R.id.iv_back){
            //返回商品列表界面
            Intent intent = new Intent(this,ShoppingGoodsActivity.class);
            //设置启动标志，避免多次返回同一页面
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
        }
        if(v.getId() == R.id.iv_niushi){
            //返回商品列表界面
            Intent intent = new Intent(this,LoginActivity.class);
            //设置启动标志，避免多次返回同一页面
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
        }
        if(v.getId() == R.id.iv_home){
            //返回主页
            Intent intent = new Intent(this,ShoppingGoodsActivity.class);
            //设置启动标志，避免多次返回同一页面
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
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
        if(v.getId() == R.id.iv_setting){
            //弹出对话框
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("设置");
            builder.setMessage("是否清空购物车");
            builder.setPositiveButton("清空购物车", (dialog, which) -> {
                mCount = "0";
                mCountOrange = "0";
                mCountQingju = "0";
                //以下携带购物车数量跳转到商品挑选页面
                Intent intent = new Intent(this, ShoppingGoodsActivity.class);
                intent.putExtra("count",mCount);
                register.launch(intent);
            });
            builder.setNegativeButton("返回",null);
            AlertDialog dialog = builder.create();
            dialog.show();
        }
    }
    private void Mqtt_init()
    {
        try {
            //host为主机名，test为clientid即连接MQTT的客户端ID，一般以客户端唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(host,mqtt_id,new MemoryPersistence());
            //MQTT的连接设置
            options = new MqttConnectOptions();
            //设置mqtt版本号
            options.setMqttVersion(MqttConnectOptions.MQTT_VERSION_3_1_1);
            //设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，这里设置为true表示每次连接到服务器都以新的身份连接
            options.setCleanSession(false);
            //设置连接的用户名
            options.setUserName(userName);
            //设置连接的密[码
            options.setPassword(passWord.toCharArray());
            // 设置超时时间 单位为秒
            options.setConnectionTimeout(10);
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options.setKeepAliveInterval(20);
            //设置回调
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //连接丢失后，一般在这里面进行重连
                    System.out.println("connectionLost----------");
                    //startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish后会执行到这里
                    System.out.println("deliveryComplete---------"+ token.isComplete());
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message) throws Exception {
                    //subscribe后得到的消息会执行到这里面
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //收到消息标志位
//                    msg.obj = topicName + "---" +message.toString();
                    msg.obj = message.toString();
                    handler.sendMessage(msg);    // hander 回传
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    // 订阅函数    (下发任务/命令)
    private void Mqtt_publishMessage(String topic,String message2) throws MqttException {
        if (client == null || !client.isConnected()) {
            //订阅主题
            client.subscribe(topic,0);
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setPayload(message2.getBytes());
        try {
            client.publish(topic,message);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}