#include <cstdio>
#include "ChatClient.h"


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		LOG_ERROR << "run command invalid! argc must be 3!" << argc;
		return -1;
	}

	LOG_INFO << "ChatClient run";

	muduo::net::EventLoop loop;
	muduo::net::InetAddress serverAddr("127.0.0.1", atoi(argv[1]));
	ChatClient client(&loop, serverAddr);
	client.connect();
	loop.loop();

	return 0;
}