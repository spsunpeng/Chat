#include <iostream>
using namespace std;

#include "UserDO.h"
#include "UserModelBase.h"
#include "ChatServer.h"

int main(int argc, char **argv)
{
	LOG_INFO << "main pid:" << getpid();
	LOG_INFO << "main tid:" << pthread_self();

	if (argc < 2)
	{
		LOG_ERROR << "run command invalid! argc must be 3!" << argc;
		return -1;
	}
	muduo::net::EventLoop loop;
	muduo::net::InetAddress listenAddr("127.0.0.1", atoi(argv[1]));
	ChatServer server(&loop, listenAddr);
	server.start();
	loop.loop();

	return 0;
}