#if 1
#include <iostream>
//#include <boost/bind.hpp>
#include <string>
#include<vector>
using namespace std;

#include<unordered_map>
#include<list>
#include<algorithm>


class Observer
{
public:
	virtual void handle(int msgid) = 0;
};

class Observer1 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 1:
			cout << "Observer1 recv 1 msgid" << endl;
			break;
		case 2:
			cout << "Observer1 recv 2 msgid" << endl;
			break;
		}
	}
};

class Observer2 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 2:
			cout << "Observer2 recv 2 msgid" << endl;
			break;
		}
	}
};

class Observer3 : public Observer
{
public:
	void handle(int msgid)//handle处理
	{
		switch (msgid)
		{
		case 1:
			cout << "Observer3 recv 1 msgid" << endl;
			break;
		case 3:
			cout << "Observer3 recv 3 msgid" << endl;
			break;
		}
	}
};

class Subject
{
public:
	//订阅
	void addObserver(Observer* obser, int msgid)
	{
		_subMap[msgid].push_back(obser);
	}

	//发布
	void dispatch(int msgid)
	{
		auto it = _subMap.find(msgid);
		if (it != _subMap.end())
		{
			for (Observer* pObser : it->second)
			{
				pObser->handle(msgid);
			}
		}
	}

private:
	unordered_map<int, list<Observer*>> _subMap;
};

int main()
{
	Subject subject;
	Observer* p1 = new Observer1();
	Observer* p2 = new Observer2();
	Observer* p3 = new Observer3();

	subject.addObserver(p1, 1);
	subject.addObserver(p1, 2);
	subject.addObserver(p2, 2);
	subject.addObserver(p3, 1);
	subject.addObserver(p3, 3);

	int msgid = 0;
	while (1)
	{
		cout << "please enter id: ";
		cin >> msgid;
		if (msgid == -1)
			break;
		subject.dispatch(msgid);
	}

	return 0;
}
#endif
