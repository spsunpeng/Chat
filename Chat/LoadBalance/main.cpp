#include <iostream>
#include "lbserver.h"
#include "telnetserver.h"

/*
���ؾ��������������ܵ㣺
1.�ṩһ�ֺ��ʵĸ����㷨
2.�ܹ���̬���ChatServer
3.�ܹ��޳����еķ������ϵķ�����(����)

lbserver:����client�����ӣ�����һ��server��ip:port��client�������ر�����
telnetserver:����Զ��tenlet���֧�ֶ�̬���ChatServer��ip:port
heartbeatclient:ÿ��1����ChatServer����������Ϣ�����������Ƿ�����
*/
int main(int argc, char **argv)
{
	if (argc < 3)
	{
		LOG_ERROR << "run command invalid! argc must be 3!" << argc;
		return -1;
	}

	// ����lb�Ŀͻ���ģ�飬�����������������Ϣ

	// ����lb������ģ�飬����ChatClient������
	LOG_INFO << "lb started!!! " << argv[1] << ":" << atoi(argv[2]);
	muduo::net::EventLoop loop;
	
	muduo::net::InetAddress listenAddr1(argv[1], atoi(argv[2]));
	LBServer server(&loop, listenAddr1);
	server.start();

	// ��port+1�˿��ϵȴ�tenlet���ӣ����÷�������Ϣ
	muduo::net::InetAddress listenAddr2(argv[1], atoi(argv[2])+1);
	TelnetServer tserver(&loop, listenAddr2);
	tserver.start();

	loop.loop();
    return 0;
}