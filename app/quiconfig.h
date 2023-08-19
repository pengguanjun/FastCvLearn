﻿#ifndef QUICONFIG_H
#define QUICONFIG_H

#include <QObject>

class QUIConfig
{
public:
    //全局图标
    static int IconMain;            //标题栏左上角图标
    static int IconMenu;            //下拉菜单图标
    static int IconMin;             //最小化图标
    static int IconMax;             //最大化图标
    static int IconNormal;          //还原图标
    static int IconClose;           //关闭图标

    //全局字体
    static QString FontName;        //全局字体名称
    static int FontSize;            //全局字体大小

    //样式表颜色值
    static QString TextColor;       //文字颜色
    static QString PanelColor;      //面板颜色
    static QString BorderColor;     //边框颜色
    static QString NormalColorStart;//正常状态开始颜色
    static QString NormalColorEnd;  //正常状态结束颜色
    static QString DarkColorStart;  //加深状态开始颜色
    static QString DarkColorEnd;    //加深状态结束颜色
    static QString HighColor;       //高亮颜色
};

#endif // QUICONFIG_H
