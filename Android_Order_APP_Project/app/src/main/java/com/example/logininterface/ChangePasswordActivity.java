package com.example.logininterface;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.example.logininterface.database.ShoppingDBHelper;
import com.example.logininterface.enity.GoodsInfo;
import com.example.logininterface.enity.Logininfo;

import java.util.Random;

public class ChangePasswordActivity extends AppCompatActivity implements View.OnClickListener {

    private String mPhone;
    private String mVerifyCode;
    private EditText et_firstPassword;
    private EditText et_secondPassword;
    private EditText et_verifycode;
    //声明一个商品数据库帮助器对象
    private ShoppingDBHelper mHelper;
    private String password_first;
    private String password_second;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_change_password);

        et_firstPassword = findViewById(R.id.et_firstPassword);
        et_secondPassword = findViewById(R.id.et_secondPassword);
        et_verifycode = findViewById(R.id.et_verifycode);
        //设置返回主界面的监听事件
        findViewById(R.id.iv_back).setOnClickListener(this);
        //设置确认修改密码的监听事件
        findViewById(R.id.btn_confirm).setOnClickListener(this);
        //从上一个页面获取要修改密码的手机号码
        mPhone = getIntent().getStringExtra("phone");
        //显示密码的监听事件
        findViewById(R.id.iv_eye1).setOnClickListener(this);
        findViewById(R.id.iv_eye2).setOnClickListener(this);

        findViewById(R.id.btn_verifycode).setOnClickListener(this);
    }

    //启动Activity时启动数据库
    @Override
    protected void onStart() {
        mHelper = ShoppingDBHelper.getInstance(this);
        super.onStart();
        mHelper.openReadLink();
        mHelper.openWriteLink();
//        Logininfo info = new Logininfo();
//        info.password = password_first;
//        mHelper.savaLoginInfo(info);
    }

    //关闭Activity时关闭数据库
    @Override
    protected void onStop() {
        super.onStop();
//        mHelper.closeLink();
    }

    @Override
    public void onClick(View v) {
        if(v.getId() == R.id.btn_verifycode){
            //点击获取验证码按钮
            //生成六位随机数字验证码
            mVerifyCode = String.format("%06d",new Random().nextInt(999999));
            //弹出提醒对话框，提示用户记住验证码数字
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("请记住验证码");
            builder.setMessage("手机号："+ mPhone + "，本次验证码是" + mVerifyCode +"，请输入验证码");
            builder.setPositiveButton("好的",null);
            AlertDialog dialog = builder.create();
            dialog.show();
        }
        if(v.getId() == R.id.btn_confirm){
            //点击了“确定”按钮
            password_first = et_firstPassword.getText().toString();
            password_second = et_secondPassword.getText().toString();
            if(password_first.length()<6){
                Toast.makeText(this, "请输入正确的密码", Toast.LENGTH_SHORT).show();
                return;
            }
            if(!password_first.equals(password_second)){
                Toast.makeText(this, "两次输入的密码不一样喔", Toast.LENGTH_SHORT).show();
                return;
            }
            if(!mVerifyCode.equals(et_verifycode.getText().toString())){
                Toast.makeText(this, "验证码错误，请重试", Toast.LENGTH_SHORT).show();
                return;
            }
            Toast.makeText(this, "密码修改成功", Toast.LENGTH_SHORT).show();
            //把修改好的密码返回登录界面
            Intent intent = new Intent();
            intent.putExtra("new_password", password_first);
            setResult(Activity.RESULT_OK,intent);
            finish();
        }
        if(v.getId() == R.id.iv_eye1){
            //显示新密码
            if(et_firstPassword.getInputType() == (InputType.TYPE_NUMBER_VARIATION_PASSWORD|InputType.TYPE_CLASS_NUMBER)) {
                //将隐藏的密码显示
                et_firstPassword.setInputType(InputType.TYPE_CLASS_NUMBER);
            }else if(et_firstPassword.getInputType() == InputType.TYPE_CLASS_NUMBER){
                //将显示的密码隐藏起来
                et_firstPassword.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD|InputType.TYPE_CLASS_NUMBER);
            }
        }
        if(v.getId() == R.id.iv_eye2){
            //显示新密码
            if(et_secondPassword.getInputType() == (InputType.TYPE_NUMBER_VARIATION_PASSWORD|InputType.TYPE_CLASS_NUMBER)) {
                //将隐藏的密码显示
                et_secondPassword.setInputType(InputType.TYPE_CLASS_NUMBER);
            }else if(et_secondPassword.getInputType() == InputType.TYPE_CLASS_NUMBER){
                //将显示的密码隐藏起来
                et_secondPassword.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD|InputType.TYPE_CLASS_NUMBER);
            }
        }
        if(v.getId() == R.id.iv_back){
            //返回主界面
            Intent intent = new Intent(this,LoginActivity.class);
            //设置启动标志，避免多次返回同一页面
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
        }
    }
}