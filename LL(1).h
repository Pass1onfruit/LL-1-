#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <map>
#include <cstdlib>
#include <algorithm>

using namespace std;

void Input();  //LL(1)文法输入函数以及VN和VT之间的分离
void Select();
void Predict();
void Analyze(string str);
string CharStr(char ch);

string p[100];       //表示产生式
int n_chan;             //产生式的个数
int n_VN;
int step = 0;  //预测分析过程步骤序数#pragma once

vector<string> VN;      //终结符集,和set_VN存储相同，但向量存储便于随机访问
vector<string> VT;      //非终结符集
set<string> set_VN;     //非终结符集合,和VN存储相同，但集合存储便于查找元素和去重
set<string> set_VT;     //终结符集合
string all_left[100];      //产生式所有左部VN
string all_right[100];      //产生式所有右部VN+VN
string S;  //开始VN
map<string, int> Vn2Num;    //VN映射到FIRST集合下标
map<string, int> Vt2Num;    //VN映射到FIRST集合下标
set<string> SELECT[100];
vector<string> predict[100];  //预测分析表
