package com.example.logininterface.enity;

public class Logininfo {
    public int id;
    public String phone;
    public String password;
    public boolean remember = false;

    public Logininfo(){}

    public Logininfo(String phone, String password, boolean remember) {
        this.phone = phone;
        this.password = password;
        this.remember = remember;
    }

    @Override
    public String toString() {
        return "Logininfo{" +
                "id=" + id +
                ", phone='" + phone + '\'' +
                ", password='" + password + '\'' +
                ", remember=" + remember +
                '}';
    }
}
