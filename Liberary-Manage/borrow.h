#pragma once
#ifndef __BORROW_H__   //防止重复引用
#define __BORROW_H__
extern int Maxstubw;   //设为外部访问全局变量
extern int Maxteabw;
extern float Delaytopay;
extern int Applyforday;

void borrowed(int level, int symbol, int id, int no);
void display();
int udRecord(int m,int n,int signal);
void showDelay(int level,int no);

#endif