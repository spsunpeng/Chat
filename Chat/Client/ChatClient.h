#pragma once

#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <muduo/base/ThreadPool.h>
#include "json.hpp"
#include <semaphore.h>
/*
�ͻ���ʵ�֣�����C++ muduo�����
*/
class ChatClient
{
public:
	ChatClient(muduo::net::EventLoop *loop,
		const muduo::net::InetAddress &addr)
		:_client(loop, addr, "ChatClient")
	{
		// ���ÿͻ���TCP���ӻص��ӿ�
		_client.setConnectionCallback(bind(&ChatClient::onConnection,
			this, _1));

		// ���ÿͻ��˽������ݻص��ӿ�
		_client.setMessageCallback(bind(&ChatClient::onMessage,
			this, _1, _2, _3));

		// �ź�����ʼ����Դ����Ϊ0
		sem_init(&_loginSem, false, 0);
		sem_init(&_RegSem, false, 0);
		sem_init(&_addFriendSem, false, 0);
		sem_init(&_addGroupSem, false, 0);
		sem_init(&_onUserSem, false, 0);
		sem_init(&_addGroupUserSem, false, 0);		
	}
	// ���ӷ�����
	void connect()
	{
		_client.connect();
	}
private:
	// TcpClient�󶨻ص������������ӻ��߶Ͽ�������ʱ����
	void onConnection(const muduo::net::TcpConnectionPtr &con);

	// TcpClient�󶨻ص��������������ݽ���ʱ����
	void onMessage(const muduo::net::TcpConnectionPtr &con,
		muduo::net::Buffer *buf,
		muduo::Timestamp time);

	// �ͻ���������棬�ڵ������߳��н����û�������з��Ͳ���
	void userClient(const muduo::net::TcpConnectionPtr &con);

	void Login(const muduo::net::TcpConnectionPtr& con);
	void Register(const muduo::net::TcpConnectionPtr& con);
	void oneChat(const muduo::net::TcpConnectionPtr& con);
	void groupChat(const muduo::net::TcpConnectionPtr& con);
	void addFriend(const muduo::net::TcpConnectionPtr& con);
	void addGroup(const muduo::net::TcpConnectionPtr& con);
	void addGroupUser(const muduo::net::TcpConnectionPtr& con, bool isCreate = false);


	muduo::net::TcpClient _client;
	muduo::ThreadPool _pool;//�̳߳�

	sem_t _loginSem;
	sem_t _RegSem;
	sem_t _addFriendSem;
	sem_t _addGroupSem;
	sem_t _onUserSem;
	sem_t _addGroupUserSem;
};