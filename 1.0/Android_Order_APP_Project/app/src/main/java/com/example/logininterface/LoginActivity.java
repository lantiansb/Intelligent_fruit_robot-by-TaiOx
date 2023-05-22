package com.example.logininterface;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.view.View;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import com.example.logininterface.database.ShoppingDBHelper;
import com.example.logininterface.enity.Logininfo;
import com.example.logininterface.util.ClickFilter;

public class LoginActivity extends AppCompatActivity implements View.OnClickListener, View.OnFocusChangeListener {

    private EditText et_phone;
    private EditText et_password;
    private String mPasswork = "111111";//设置初始密码
    //声明一个商品数据库帮助器对象
    private ShoppingDBHelper mHelper;
    private CheckBox ck_remember;
    private SharedPreferences preferences;
    private ActivityResultLauncher<Intent> register;
    private Logininfo info;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        //获取编辑框参数
        et_phone = findViewById(R.id.et_phone);
        et_password = findViewById(R.id.et_password);
        ck_remember = findViewById(R.id.ck_remember);
        //设置监听事件
        findViewById(R.id.iv_tip).setOnClickListener(this);
        findViewById(R.id.btn_sign).setOnClickListener(this);
        findViewById(R.id.tv_register).setOnClickListener(this);
        findViewById(R.id.iv_eyes).setOnClickListener(this);
        //给et_phone添加文本变更监听器
//        et_phone.addTextChangedListener(new  HideTextWatcher(et_phone, 11));
//        et_password.addTextChangedListener(new  HideTextWatcher(et_password, 6));

        //使用共享参数的记住密码
//        preferences = getSharedPreferences("config", Context.MODE_PRIVATE);

        //取到修改好的密码
        register = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), new ActivityResultCallback<ActivityResult>() {
            @Override
            public void onActivityResult(ActivityResult result) {
                Intent intent = result.getData();
                if(intent != null && result.getResultCode() == Activity.RESULT_OK){
                    mPasswork = intent.getStringExtra("new_password");
//                    Toast.makeText(MainActivity.this, mPasswork, Toast.LENGTH_SHORT).show();
                }
            }
        });

        //对密码输入框进行监听
        et_password.setOnFocusChangeListener(this);

    }

    //重新登录,记住密码
    private void reload() {
        Logininfo info = mHelper.queryTopLoginInfo();
        if(info != null && info.remember){
            et_phone.setText(info.phone);
            et_password.setText(info.password);
            ck_remember.setChecked(true);
        }
        //共享参数用法
//        boolean isRemember = preferences.getBoolean("isRemember", false);
//        if(isRemember){
//            String phone = preferences.getString("phone", "");
//            et_phone.setText(phone);
//            String password = preferences.getString("password", "");
//            et_password.setText(password);
//            ck_remember.setChecked(true);
//        }
    }

    //启动Activity时启动数据库
    @Override
    protected void onStart() {
        super.onStart();
        mHelper = ShoppingDBHelper.getInstance(this);
        mHelper.openReadLink();
        mHelper.openWriteLink();

        reload();
    }

    //关闭Activity时关闭数据库
    @Override
    protected void onStop() {
        super.onStop();
//        mHelper.closeLink();
    }

    @Override
    public void onClick(View v) {
        if(v.getId() == R.id.btn_sign){
            //点击了登录按钮
            //判断手机号码长度是否为11位
            if(et_phone.getText().length() < 11){
                Toast.makeText(this, "请输入正确的手机号", Toast.LENGTH_SHORT).show();
                return;
            }
            //判断密码是否正确
            if(!mPasswork.equals(et_password.getText().toString())){
                Toast.makeText(this, "密码错误", Toast.LENGTH_SHORT).show();
                return;
            }
            //提醒用户登陆成功
            loginSuccess();
        }
        if(v.getId() == R.id.iv_tip){
            //点击了提示窗口
            String desc = String.format("初始密码为111111，如需设置密码，直接对话框进行输入");
            //弹出提醒对话框
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("登录成功");
            builder.setMessage(desc);
            builder.setPositiveButton("修改密码", (dialog, which) -> {
                //跳转到修改密码界面
                Intent intent = new Intent(this, ChangePasswordActivity.class);
                //以下携带手机号码跳转到找回密码页面
                intent.putExtra("phone",et_phone.getText().toString());
                register.launch(intent);
            });
            builder.setNegativeButton("返回",null);
            AlertDialog dialog = builder.create();
            dialog.show();
        }
        if(v.getId() == R.id.tv_register){
            if (ClickFilter.filter()) {//防止重复点击
                //提示暂未开通此功能
                Toast.makeText(this, "暂未开通此功能", Toast.LENGTH_SHORT).show();
            }
        }
        if(v.getId() == R.id.iv_eyes){
            if(et_password.getInputType() == (InputType.TYPE_NUMBER_VARIATION_PASSWORD|InputType.TYPE_CLASS_NUMBER)) {
                //将隐藏的密码显示
//                Toast.makeText(this, String.valueOf(et_password.getInputType()), Toast.LENGTH_SHORT).show();
                et_password.setInputType(InputType.TYPE_CLASS_NUMBER);
            }else if(et_password.getInputType() == InputType.TYPE_CLASS_NUMBER){
                //将显示的密码隐藏起来
                et_password.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD|InputType.TYPE_CLASS_NUMBER);
            }
        }
    }

    //登录成功的操作
    private void loginSuccess() {
        String desc = String.format("手机号为%s的用户，欢迎体验本次服务！",et_phone.getText().toString());
        //弹出提醒对话框
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("登录成功");
        builder.setMessage(desc);
        builder.setPositiveButton("确定", (dialog, which) -> {
            //结束当前页面
//            finish();
            //跳转到商品界面
            Intent intent = new Intent(this, ShoppingGoodsActivity.class);
            //设置启动标志，避免多次返回同一页面
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
        });
        builder.setNegativeButton("取消",null);
        AlertDialog dialog = builder.create();
        dialog.show();

        //将登录信息同步至数据库
        info = new Logininfo();
        info.phone = et_phone.getText().toString();
        info.password = et_password.getText().toString();
//        info.password = mPasswork;
        info.remember = ck_remember.isChecked();
        mHelper.savaLoginInfo(info);
    }

    //当密码输入框获取焦点之后，根据输入的电话号码，查询出对应的密码，自动填入
    @Override
    public void onFocusChange(View v, boolean hasFocus) {
        if(v.getId() == R.id.et_password && hasFocus){
            Logininfo info = mHelper.queryByPhone(et_phone.getText().toString());
            if(info != null){
                //如果根据电话号码，查询出了密码
                et_password.setText(info.password);
                ck_remember.setChecked(info.remember);
            }else {
                //没有查到，清空密码
                et_password.setText("");
                ck_remember.setChecked(false);
            }
        }
    }

    //定义一个编辑框监听器，在输入文本达到指定长度时自动隐藏输入法
    private class HideTextWatcher implements TextWatcher {
        private EditText mView;
        private int mMaxLength;
        public HideTextWatcher(EditText v, int maxLength) {
            this.mView = v;
            this.mMaxLength = maxLength;
        }

        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

        }

        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) {

        }

        @Override
        public void afterTextChanged(Editable s) {
//            if(s.toString().length() == mMaxLength){
//                //隐藏输入法软键盘
//                ViewUtil.hideOneInputMethod(LoginMainActivity.this,mView);
//            }
        }
    }
}