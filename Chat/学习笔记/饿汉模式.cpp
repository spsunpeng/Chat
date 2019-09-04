#if 0

#include<iostream>
using namespace std;

class Singleton
{
public:
	static Singleton* getInstance()
	{
		return &instance;
	}
private:
	Singleton() {};
	static Singleton instance;
};

Singleton Singleton::instance;

int main()
{
	Singleton* p1 = Singleton::getInstance();
	Singleton* p2 = Singleton::getInstance();

	cout << p1 << " " << p2 << endl;

	return 0;
}
#endif