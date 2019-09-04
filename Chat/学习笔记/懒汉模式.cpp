#include<iostream>
using namespace std;

class Singleton
{
public:
	static Singleton* getInstance()
	{
		static Singleton instance;
		return &instance;
	}
private:
	Singleton() {};
};


class Singleton2
{
public:
	static Singleton2& getInstance()
	{
		static Singleton2 instance;
		return instance;
	}
private:
	Singleton2() {};
	Singleton2(const Singleton2&) {};
};


int main()
{
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();
	cout << p1 << " " << p2 << endl;

	Singleton2& p3 = Singleton2::getInstance();
	cout << &p3 << endl;
}

//Singleton 独，单
//instance 例

/*
int a = 10; int& b = a; int& c = b; //对
int a=10; int& b = a; int&&c =b;  //错，没有引用的引用
*/