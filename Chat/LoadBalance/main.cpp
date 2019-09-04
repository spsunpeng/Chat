#include <iostream>
#include "lbserver.h"
#include "telnetserver.h"

/*
负载均衡器的三个功能点：
1.提供一种合适的负载算法
2.能够动态添加ChatServer
3.能够剔除现有的发生故障的服务器(容灾)

lbserver:接收client的连接，返回一个server的ip:port给client，主动关闭连接
telnetserver:接收远程tenlet命令，支持动态添加ChatServer的ip:port
heartbeatclient:每隔1秒向ChatServer发送心跳消息，检测服务器是否正常
*/
int main(int argc, char **argv)
{
	if (argc < 3)
	{
		LOG_ERROR << "run command invalid! argc must be 3!" << argc;
		return -1;
	}

	// 启动lb的客户端模块，向服务器发送心跳消息

	// 启动lb的网络模块，接收ChatClient的请求
	LOG_INFO << "lb started!!! " << argv[1] << ":" << atoi(argv[2]);
	muduo::net::EventLoop loop;
	
	muduo::net::InetAddress listenAddr1(argv[1], atoi(argv[2]));
	LBServer server(&loop, listenAddr1);
	server.start();

	// 在port+1端口上等待tenlet连接，配置服务器信息
	muduo::net::InetAddress listenAddr2(argv[1], atoi(argv[2])+1);
	TelnetServer tserver(&loop, listenAddr2);
	tserver.start();

	loop.loop();
    return 0;
}