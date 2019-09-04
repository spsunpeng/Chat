#pragma once

#include <muduo/net/TcpServer.h> // 服务器类
#include <muduo/net/EventLoop.h> // 事件循环
#include <boost/bind.hpp> // 绑定器 
#include <muduo/base/Logging.h> // 日志类
#include <vector>

class LBServer
{
public:
	LBServer(muduo::net::EventLoop *loop,
		const muduo::net::InetAddress &address)
		:_server(loop, address, "LBServer")
	{
		// 设置连接回调 onConnection
		_server.setConnectionCallback(bind(&LBServer::onConnection,
			this, _1));
	}

	// 启动LBServer服务
	void start()
	{
		_server.start();
	}
private:
	// TcpServer绑定的回调函数，当有新连接或连接中断时调用
	void onConnection(const muduo::net::TcpConnectionPtr &con);
	muduo::net::TcpServer _server;
};