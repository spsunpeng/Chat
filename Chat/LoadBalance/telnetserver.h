#pragma once

#include <muduo/net/TcpServer.h> // ��������
#include <muduo/net/EventLoop.h> // �¼�ѭ��
#include <boost/bind.hpp> // ���� 
#include <muduo/base/Logging.h> // ��־��
#include <vector>

// ���ؾ�������ά��server���б�
std::vector<std::string> _serverList;

class TelnetServer
{
public:
	TelnetServer(muduo::net::EventLoop *loop,
		const muduo::net::InetAddress &address)
		:_server(loop, address, "LBServer")
	{
		// �������ӻص� onConnection
		_server.setConnectionCallback(bind(&TelnetServer::onConnection,
			this, _1));
		// ������Ϣ�ص� onMessage
		_server.setMessageCallback(bind(&TelnetServer::onMessage,
			this, _1, _2, _3));
	}

	// ����LBServer����
	void start()
	{
		_server.start();
	}
private:
	// �󶨵Ļص����������������ӻ������ж�ʱ����
	void onConnection(const muduo::net::TcpConnectionPtr &con)
	{
		if (!con->connected())
		{
			con->shutdown();
		}
	}
	// �󶨵Ļص�����������������ʱ����
	void onMessage(const muduo::net::TcpConnectionPtr &con,
		muduo::net::Buffer *buf,
		muduo::Timestamp time)
	{
		// �������Զ��telnet���͵ķ��������ã������ռ�
		std::string host = buf->retrieveAllAsString();
		_serverList.push_back(host);
	}

	muduo::net::TcpServer _server;
};
