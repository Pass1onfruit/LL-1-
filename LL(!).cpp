#include "LL(1).h"

int main()
{
	Input();
	Select();
	Predict();
	cout << "请输入对应以上文法要分析的字符串(输入'-'结束)(如i*i):";
	string str;
	cin >> str;
	while (str[str.length() - 1] != '-')
	{
		Analyze(str);
		cout << endl << "请继续输入要分析的字符串(输入'-'结束)(如i*i):";
		cin >> str;
	}
	return 0;
}

///字符转换成字符串函数
string CharStr(char ch)
{
	char t[] = { ch,'\0' };
	return t;
}

///输入合法文法&&分离VT和VN
void Input()
{
	string tmp;   //用于拼接字符串
	string tmp1;
	n_chan = 8;
	p[0] = "E->TE'";
	p[1] = "E'->+TE'";
	p[2] = "E'->e";
	p[3] = "T->FT'";
	p[4] = "T'->*FT'";
	p[5] = "T'->e";
	p[6] = "F->(E)";
	p[7] = "F->i";
	///提取非终结符VN的循环
	for (int i = 0; i < n_chan; i++)
	{
		tmp = "";
		tmp1 = "";
		//scanf("%[^\n]",p[i]);
		//getchar();
		int j = 0;
		while (p[i][j] != '-')
		{
			tmp += p[i][j];  //因为非终结符可能是形如E'之类的两个字符，所以需要拼接，遇到箭头就结束
			j++;
		}
		j += 2;
		for (; j < (int)p[i].length(); j++)
			tmp1 += p[i][j];
		cout << p[i] << endl;
		all_left[i] = tmp;      //存储每个产生式的左部
		all_right[i] = tmp1;    //存储每个产生式的右部
		set_VN.insert(tmp);  //将每行产生式拼接好的非终结符插入到集合中
	}
	n_VN = set_VN.size();   //读取VN个数
	///提取终结符VT的循环
	for (int i = 0; i < n_chan; i++)
	{      
		//n_chan是产生式的个数
		int chan_len = p[i].length(); //每个产生式的长度
		for (int j = 0; j < chan_len; j++)
			if (p[i][j] == '>')
			{
				tmp = "";
				for (int k = j + 1; k < chan_len; k++)
				{ //k记录"->"之后的下标
					tmp1 = p[i][k];
					if (set_VN.count(tmp1) == 0)
					{ //count==0表示该字符不在集合中，即∈VT
						tmp += p[i][k];           //拼接字符串
						tmp1 = p[i][k + 1];
						if ((k == chan_len - 1) || ((k < chan_len - 1) && set_VN.count(tmp1) == 1))
						{
							//判断(已是最后一个字符||后一个字符是VN则之前的字符串是一个VT)
							set_VT.insert(tmp);
							tmp = "";
						}
					}
				}
				break;  //本行产生式已扫描结束
			}
	}
	set_VT.erase("'");  //VT集合中删除"'"和"e"符号
	//通过集合迭代器，将其中元素转移到向量VN中(因为集合不易随机存取，但可以筛除重复元素)
	for (set<string>::iterator it = set_VN.begin(); it != set_VN.end(); it++)
		VN.push_back(*it);
	for (int i = 0; i < (int)VN.size(); i++)
		Vn2Num[VN[i]] = i;  ///每个VN映射到FIRST集合下标
	for (set<string>::iterator it = set_VT.begin(); it != set_VT.end(); it++)
		VT.push_back(*it);
	for (int i = 0; i < (int)VT.size(); i++)
	{
		if (VT[i] == "e")
			Vt2Num["#"] = i;
		else
			Vt2Num[VT[i]] = i;
	}
}
///SELECT集
void Select()
{
	SELECT[0].insert("(");
	SELECT[0].insert("i");
	SELECT[1].insert("+");
	SELECT[2].insert(")");
	SELECT[2].insert("#");
	SELECT[3].insert("(");
	SELECT[3].insert("i");
	SELECT[4].insert("*");
	SELECT[5].insert("+");
	SELECT[5].insert(")");
	SELECT[5].insert("#");
	SELECT[6].insert("(");
	SELECT[7].insert("i");
}
///预测分析表
void Predict()
{
	bool flag;
	for (int i = 0; i < (int)VT.size(); i++)
		if (VT[i] == "e")
		{
			VT[i] = "#";
			break;
		}
	for (int i = 0; i < n_VN; i++)
	{
		for (int j = 0; j < (int)VT.size(); j++)
		{
			flag = true;
			for (int k = 0; k < n_chan; k++)
				if (all_left[k] == VN[i] && SELECT[k].count(VT[j]) == 1)
				{
					predict[i].push_back(all_right[k]);
					flag = false;
					break;
				}
			if (flag) predict[i].push_back("error");
		}
	}
}
///分析输入字符串
void Analyze(string str)
{
	S = VN[0];  //将VN集中首元素定为开始符号元素
	vector<string> analy;  //分析向量(用向量代替可以随机访问，便于显示)
	vector<string> input;    //输入串向量(用向量代替可以随机访问，便于显示)
	string fun;      //存储在预测分析表中找到的表达式
	analy.push_back("#");
	analy.push_back(S);
	input.push_back("#");
	for (int i = (int)str.length() - 1; i >= 0; i--)
		input.push_back(CharStr(str[i]));
	///**************初始化分割线**************
	cout << endl << "步骤\t分析栈\t\t剩余输入串\t推导所用产生式" << endl;
	while (true)
	{
		cout << ++step << "\t";
		for (int i = 0; i < (int)analy.size(); i++)
			cout << analy[i];
		cout << "\t\t";
		for (int i = (int)input.size() - 1; i >= 0; i--)
			cout << input[i];
		if (input.back() == "#" && analy.back() == "#")
		{  //向量尾元素,模拟栈顶元素
			cout << "\t\t接受";
			break;
		}
		else if (input.back() == analy.back()) {   //两个栈顶元素同是一个VT则均出栈一个元素
			cout << "\t\t\"" << input.back() << "\"匹配";
			analy.pop_back();
			input.pop_back();
		}//两个都是VT但不相等，则出错
		else if (set_VT.count(input.back()) == 1 && set_VT.count(analy.back()) == 1 && input.back() != analy.back())
		{
			cout << "出错:两栈顶算符不匹配！" << endl;
			break;
		}
		//分析栈顶元素是VN
		else if (set_VN.count(analy.back()) == 1)
		{
			fun = predict[Vn2Num[analy.back()]][Vt2Num[input.back()]];
			if (fun != "error") { //能在预测分析表中找到表达式
				cout << "\t\t" << analy.back() << "->" << fun;
				analy.pop_back(); //模拟出栈
				for (int i = (int)fun.length() - 1; i >= 0; i--)
				{   //倒序入栈
					if (fun[i] == '\'')
					{
						analy.push_back(CharStr(fun[i - 1]) + CharStr(fun[i]));
						i--;
					}
					else if (fun[i] == 'e') break;
					else
						analy.push_back(CharStr(fun[i]));
				}
			}
			else
			{
				cout << endl << "出错:未在预测表中找到产生式！" << endl;
				break;
			}
		}//两个都是VT但不在终结符集中
		else if (set_VT.count(input.back()) == 0)
		{
			cout << endl << "出错:该字符不属于该文法！" << endl;
			break;
		}
		cout << endl;
	}
}
