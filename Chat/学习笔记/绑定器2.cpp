#include<iostream>
#include<functional>  //函数对象
#include<algorithm>   //泛型算法
#include<vector>
#include<time.h>
#include<map>
using namespace std;
using namespace placeholders;

#if 1
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

	vector<int> vec({ 10, 20, 75, 42, 64, 13 });
	sort(vec.begin(), vec.end(), greater<int>());
	//Show(vec);

	//库函数
	//sort(vec.begin(), vec.end(), greater<int>());
	//auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 50));
	auto it = find_if(vec.begin(), vec.end(), bind(greater<int>(), 50, _1));


	//函数指针
	//sort(vec.begin(), vec.end(), greaterFun<int>);
	//auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 50));
	//实现不了，证明了函数对象比函数指针的用处更广，因为函数对象还有构造函数参与了

	//函数对象
	//sort(vec.begin(), vec.end(), mygreater<int>());
	//auto it = find_if(vec.begin(), vec.end(), mybind1st<mygreater<int>,int>(mygreater<int>(), 50));
	//auto it = find_if(vec.begin(), vec.end(), mybind1st(mygreater<int>(), 50));//优化，省略模板的参数
	

	vec.insert(it, 50);
	Show(vec);
}
#endif