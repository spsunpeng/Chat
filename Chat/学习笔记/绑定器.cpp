#include<iostream>
#include<functional>  //函数对象
#include<algorithm>   //泛型算法
#include<vector>
#include<time.h>
#include<map>

using namespace std;
using namespace placeholders;



#if 0
void Hello(string str1, string str2, string str3)
{
	cout << str1 << " " << str2 << " " << str3 << endl;
}

int main()
{
	bind(Hello, _1, "str2", "str3")("str1");

	bind(Hello,"str1", _2, "str3");
}
#endif



#if 0
void fun1(int a)
{
	cout << a << endl;
}

void fun2(int a, int b)
{
	cout << a << " " << b << endl;
}

template<typename T>
class myfunction{};

template<typename R, typename... A>
class myfunction<R(A...)> 
{
public: 
	using TYPE = R(*)(A...);
	myfunction(TYPE pfunc):_pfunc(pfunc){}

	void operator()(A... arg)
	{
		_pfunc(arg...);
	}

private:
	TYPE _pfunc;
};

int main()
{
	myfunction<void(int)> func1(fun1);
	func1(10);
	
	myfunction<void(int, int)> func2(fun2);
	func2(10, 20);
}
#endif

#if 0
template<typename T>
void Show(T data)
{
	cout << "T" << endl;
}

template<>
void Show(const char* str)
{
	cout << "const char*" << endl;
}


template<typename T>
class Text
{
public:
	Text(){cout << "T Printf" << endl;};
};

template<>
class Text<string>
{
public:
	Text(){cout << "string Printf" << endl;}
};

template<typename K>
class Text<K*>
{
public:
	Text(){cout << "K*" << endl;}
};

template<typename R, typename A1, typename A2>
class Text<R(*)(A1, A2)>
{
public:
	Text() { cout << "R(*)(A1, A2)" << endl; }
};

int sum(int a, int b) { return a + b; }

int main()
{
	//Show("1223");
	Text<char*> text;
	Text<int(*)(int,int)> text2;

	int(*fun)(int, int) = sum;
	cout<<fun(10, 20)<<endl;           //一样
	cout << (*fun)(10, 20) << endl;    //一样
}
#endif


#if 0
//function2
void doLogin() { cout << "Login Success" << endl; }
void doRegister() { cout << "Register Success" << endl; }
void doLoginOut() { cout << "LoginOut Success" << endl; }

int main()
{
	map<int, void(*)()> m1;//函数指针
	m1.insert({ 1, doLogin });
	m1[2] = doRegister;
	m1[3] = doLoginOut;

	map<int, function<void()>> m2;//function
	m2.insert({ 1, doLogin });
	m2[2] = doRegister;
	m2[3] = doLoginOut;

	while (1)
	{
		cout << "=====================" << endl;
		cout << "1.Login" << endl;
		cout << "2.Register" << endl;
		cout << "3.LoginOut" << endl;
		cout << "=====================" << endl;

		cout << "please enter: ";
		int choice;
		cin >> choice;

		if (choice < 1 || choice>3)
			break;

		m2[choice]();
	}
}
#endif


#if 1
//function1
void Hello1() { cout << "hello world" << endl; }
void Hello2(string str) { cout << str << endl; }

class Compare
{
public:
	void operator()(){cout << "compare" << endl;}
};

class Text
{
public:
	void Show() { cout << "Text" << endl; }
};

int main()
{
	function<void()> fun1(Hello1);
	fun1();

	function<void(string)> fun2 = Hello2;
	fun2("Hello2");

	function<int(int, int)> fun3 = [](int a, int b)->int {return a + b; };
	cout<<fun3(10, 20)<<endl;
	
	function<void(Text*)> fun4 = &Text::Show; //不加&运行时报错：非标准语法，请使用&来创建指向成员的指针
	fun4(&Text());

	function<void(Compare*)> fun5 = &Compare::operator();
	fun5(&Compare());


	//function<Compare> fun3 = Compare();

	return 0;
}
#endif



#if 0
//1. bind1st  bind2nd
void Show(vector<int>& vec)
{
	for (auto val : vec)
	{
		cout << val << " ";
	}
	cout << endl;
}

template<typename T>
bool greaterFun(T a, T b)
{
	return a > b;
}

/*
template<typename Compare, typename T>
void bind1stFun(Compare com, T val)
{
	com();
}*/

template<typename T>
class mygreater
{
public:
	bool operator()(T a, T b)
	{
		return a > b;
	}
};

template<typename Compare, typename T>
class _mybind1st
{
public:
	_mybind1st(Compare compare, T t) :com(compare), val(t) {}
	bool operator()(T a)
	{
		return com(val, a);
	}
private:
	Compare com;
	T val;
};

template<typename Compare, typename T>
_mybind1st<Compare, T> mybind1st(Compare comp, T val)
{
	return _mybind1st<Compare, T>(comp, val);
}

int main()
{
	/*
	vector<int> vec;
	srand(time(nullptr));
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}

	sort(vec.begin(), vec.end(), greater<int>());

	auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 71));
	if (it != vec.end())
	{
		vec.insert(it, 71);
	}

	auto it2 = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 50));
	if (it2 != vec.end())
	{
		vec.insert(it2, 50);
	}

	vec.clear();

	cout << vec.empty() << endl;


	int arr[] = { 15 , 48 , 24 , 16 , 72 , 34 };
	sort(arr, arr+6);
	for (int val : arr)
	{
		cout << val << " ";
	}
	cout << endl;

	Show(vec);
*/

	vector<int> vec({ 10, 20, 75, 42, 64, 13 });
	sort(vec.begin(), vec.end(), greater<int>());
	//Show(vec);

	//库函数
	//sort(vec.begin(), vec.end(), greater<int>());
	//auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 50));
	

	//函数指针
	//sort(vec.begin(), vec.end(), greaterFun<int>);
	//auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 50));
	//实现不了，证明了函数对象比函数指针的用处更广，因为函数对象还有构造函数参与了

	//函数对象
	//sort(vec.begin(), vec.end(), mygreater<int>());
	//auto it = find_if(vec.begin(), vec.end(), mybind1st<mygreater<int>,int>(mygreater<int>(), 50));
	auto it = find_if(vec.begin(), vec.end(), mybind1st(mygreater<int>(), 50));//优化，省略模板的参数

	vec.insert(it, 50);
	Show(vec);
}
#endif
	