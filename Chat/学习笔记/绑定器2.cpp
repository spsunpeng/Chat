#include<iostream>
#include<functional>  //��������
#include<algorithm>   //�����㷨
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

	//�⺯��
	//sort(vec.begin(), vec.end(), greater<int>());
	//auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 50));
	auto it = find_if(vec.begin(), vec.end(), bind(greater<int>(), 50, _1));


	//����ָ��
	//sort(vec.begin(), vec.end(), greaterFun<int>);
	//auto it = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 50));
	//ʵ�ֲ��ˣ�֤���˺�������Ⱥ���ָ����ô����㣬��Ϊ���������й��캯��������

	//��������
	//sort(vec.begin(), vec.end(), mygreater<int>());
	//auto it = find_if(vec.begin(), vec.end(), mybind1st<mygreater<int>,int>(mygreater<int>(), 50));
	//auto it = find_if(vec.begin(), vec.end(), mybind1st(mygreater<int>(), 50));//�Ż���ʡ��ģ��Ĳ���
	

	vec.insert(it, 50);
	Show(vec);
}
#endif