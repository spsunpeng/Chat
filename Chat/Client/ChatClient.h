#pragma once

#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <muduo/base/ThreadPool.h>
#include "json.hpp"
#include <semaphore.h>
/*
客户端实现，基于C++ muduo网络库
*/
class ChatClient
{
public:
	ChatClient(muduo::net::EventLoop *loop,
		const muduo::net::InetAddress &addr)
		:_client(loop, addr, "ChatClient")
	{
		// 设置客户端TCP连接回调接口
		_client.setConnectionCallback(bind(&ChatClient::onConnection,
			this, _1));

		// 设置客户端接收数据回调接口
		_client.setMessageCallback(bind(&ChatClient::onMessage,
			this, _1, _2, _3));

		// 信号量初始化资源计数为0
		sem_init(&_loginSem, false, 0);
		sem_init(&_RegSem, false, 0);
		sem_init(&_addFriendSem, false, 0);
		sem_init(&_addGroupSem, false, 0);
		sem_init(&_onUserSem, false, 0);
		sem_init(&_addGroupUserSem, false, 0);		
	}
	// 连接服务器
	void connect()
	{
		_client.connect();
	}
private:
	// TcpClient绑定回调函数，当连接或者断开服务器时调用
	void onConnection(const muduo::net::TcpConnectionPtr &con);

	// TcpClient绑定回调函数，当有数据接收时调用
	void onMessage(const muduo::net::TcpConnectionPtr &con,
		muduo::net::Buffer *buf,
		muduo::Timestamp time);

	// 客户端输入界面，在单独的线程中接收用户输入进行发送操作
	void userClient(const muduo::net::TcpConnectionPtr &con);

	void Login(const muduo::net::TcpConnectionPtr& con);
	void Register(const muduo::net::TcpConnectionPtr& con);
	void oneChat(const muduo::net::TcpConnectionPtr& con);
	void groupChat(const muduo::net::TcpConnectionPtr& con);
	void addFriend(const muduo::net::TcpConnectionPtr& con);
	void addGroup(const muduo::net::TcpConnectionPtr& con);
	void addGroupUser(const muduo::net::TcpConnectionPtr& con, bool isCreate = false);


	muduo::net::TcpClient _client;
	muduo::ThreadPool _pool;//线程池

	sem_t _loginSem;
	sem_t _RegSem;
	sem_t _addFriendSem;
	sem_t _addGroupSem;
	sem_t _onUserSem;
	sem_t _addGroupUserSem;
};