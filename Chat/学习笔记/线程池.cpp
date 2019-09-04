#include<iostream>
#include<functional>  //��������
#include<algorithm>   //�����㷨
#include<vector>
#include<time.h>
#include<map>
#include<thread>
using namespace std;
using namespace placeholders;

class Thread
{
public:
	Thread(function<void()> func):_func(func){}
private:
	function<void()> _func;
};


class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();
	void startPool(int size)
	{
		for (int i = 0; i < size; ++i)
		{
			_pool.push_back(new Thread(bind(&ThreadPool::runInThread, this, i)));
		}
	}
private:
	vector<Thread*> _pool;
	void runInThread(int id)
	{
		cout << "call runInThread id: " << id << endl;
	}
};