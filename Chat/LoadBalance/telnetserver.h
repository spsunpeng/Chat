#pragma once

#include <muduo/net/TcpServer.h> // 服务器类
#include <muduo/net/EventLoop.h> // 事件循环
#include <boost/bind.hpp> // 绑定器 
#include <muduo/base/Logging.h> // 日志类
#include <vector>

// 负载均衡器上维护server的列表
std::vector<std::string> _serverList;

class TelnetServer
{
public:
	TelnetServer(muduo::net::EventLoop *loop,
		const muduo::net::InetAddress &address)
		:_server(loop, address, "LBServer")
	{
		// 设置连接回调 onConnection
		_server.setConnectionCallback(bind(&TelnetServer::onConnection,
			this, _1));
		// 设置消息回调 onMessage
		_server.setMessageCallback(bind(&TelnetServer::onMessage,
			this, _1, _2, _3));
	}

	// 启动LBServer服务
	void start()
	{
		_server.start();
	}
private:
	// 绑定的回调函数，当有新连接或连接中断时调用
	void onConnection(const muduo::net::TcpConnectionPtr &con)
	{
		if (!con->connected())
		{
			con->shutdown();
		}
	}
	// 绑定的回调函数，当有新数据时调用
	void onMessage(const muduo::net::TcpConnectionPtr &con,
		muduo::net::Buffer *buf,
		muduo::Timestamp time)
	{
		// 负责接收远程telnet发送的服务器配置，进行收集
		std::string host = buf->retrieveAllAsString();
		_serverList.push_back(host);
	}

	muduo::net::TcpServer _server;
};
