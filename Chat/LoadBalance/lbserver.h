#pragma once

#include <muduo/net/TcpServer.h> // ��������
#include <muduo/net/EventLoop.h> // �¼�ѭ��
#include <boost/bind.hpp> // ���� 
#include <muduo/base/Logging.h> // ��־��
#include <vector>

class LBServer
{
public:
	LBServer(muduo::net::EventLoop *loop,
		const muduo::net::InetAddress &address)
		:_server(loop, address, "LBServer")
	{
		// �������ӻص� onConnection
		_server.setConnectionCallback(bind(&LBServer::onConnection,
			this, _1));
	}

	// ����LBServer����
	void start()
	{
		_server.start();
	}
private:
	// TcpServer�󶨵Ļص����������������ӻ������ж�ʱ����
	void onConnection(const muduo::net::TcpConnectionPtr &con);
	muduo::net::TcpServer _server;
};