#include<iostream>

#include<assert.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<signal.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>

#include<sys/epoll.h>


#if 0
//IO复用
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //选择协议族
	assert(sockfd != -1);

	sockaddr_in ser;
	ser.sin_family = PF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd, (sockaddr*)& ser, sizeof(ser));//服务绑定，让客户端可以访问
	assert(res != -1);

	listen(sockfd, 5);  //启动监听，listen与accept之间完成

	int epoll = epoll_create(5);

	epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = sockfd;
	epoll_ctl(epoll, EPOLL_CTL_ADD, sockfd, &event);

	while (1)
	{
		epoll_event revents[128];
		int n=epoll_wait(epoll, revents, 128, -1);

		for (int i = 0; i < n; ++i)
		{
			int fd = revents[i].data.fd;
			if (fd == sockfd)
			{
				sockaddr_in cli;
				socklen_t len = sizeof(cli);
				int c = accept(sockfd, (sockaddr*)& cli, &len);
				
				if (c > 0)
				{
					event.events = EPOLLIN | EPOLLRDHUP;
					event.data.fd = c;
					epoll_ctl(epoll, EPOLL_CTL_ADD, c, &event);
				}
			}
			else
			{
				if (revents[i].events & EPOLLRDHUP)
				{
					epoll_ctl(epoll, EPOLL_CTL_DEL, fd, NULL);
					std::cout << "one client break" << std::endl;
				}
				else
				{
					char recvbuf[128] = { 0 };
					int n = recv(fd, recvbuf, 127, 0);
					if (n <= 0)
					{
						std::cout << "one client break(EPOLLIN of type)" << std::endl;
						close(fd);
						break;
					}
					std::cout << recvbuf << std::endl;
					send(fd, "OK", 2, 0);
				}
			}
		}
	}

	close(sockfd);
}
#endif


#if 0
//线程池
std::queue<int> queC;
sem_t pthreadSem;

void* pthreadFun(void* data)
{
	while (1)
	{
		sem_wait(&pthreadSem);
		int c = queC.front();
		queC.pop();

		while (1)
		{
			char recvbuf[128] = { 0 };
			int n = recv(c, recvbuf, 127, 0);
			if (n <= 0)
			{
				std::cout << "one client break" << std::endl;
				close(c);
				break;
			}
			std::cout << recvbuf << std::endl;
			send(c, "OK", 2, 0);
		}
	}	
}

int main()
{
	sem_init(&pthreadSem, false, 0);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //选择协议族
	assert(sockfd != -1);

	sockaddr_in ser;
	ser.sin_family = PF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd, (sockaddr*)& ser, sizeof(ser));//服务绑定，让客户端可以访问
	assert(res != -1);

	listen(sockfd, 5);  //启动监听，listen与accept之间完成

	//启动线程池
	for (int i = 0; i < 3; ++i)
	{
		pthread_t id;
		int res = pthread_create(&id, NULL, pthreadFun, NULL);
		assert(res == 0);
	}

	while (1)
	{
		sockaddr_in cli;
		socklen_t len = sizeof(cli);
		int c = accept(sockfd, (sockaddr*)& cli, &len);
		if (c < 0)
			continue;	

		queC.push(c);
		sem_post(&pthreadSem);
	}

	close(sockfd);
}
#endif


#if 0
//多线程
void* CommClient(void* data)
{
	int c = *(int *)data;
	while (1)
	{
		char recvbuf[128] = { 0 };
		int n = recv(c, recvbuf, 127, 0);
		if (n <= 0)
		{
			std::cout << "one client break" << std::endl;
			close(c);
			break;
		}
		std::cout << recvbuf << std::endl;
		send(c, "OK", 2, 0);
	}
}

int main()
{
	signal(SIGCHLD, SIG_IGN);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //选择协议族
	assert(sockfd != -1);

	sockaddr_in ser;
	ser.sin_family = PF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd, (sockaddr*)& ser, sizeof(ser));//服务绑定，让客户端可以访问
	assert(res != -1);

	listen(sockfd, 5);  //启动监听，listen与accept之间完成

	while (1)
	{
		sockaddr_in cli;
		socklen_t len = sizeof(cli);
		int c = accept(sockfd, (sockaddr*)& cli, &len);
		if (c < 0)
			continue;

		pthread_t id;
		int res = pthread_create(&id, NULL, CommClient, (void*)&c);
		assert(res == 0);
	}

	close(sockfd);
}
#endif


#if 0
//多进程
void CommClient(int c)
{
	while (1)
	{
		char recvbuf[128] = { 0 };
		int n = recv(c, recvbuf, 127, 0);
		if (n <= 0)
		{
			std::cout << "one client break" << std::endl;
			close(c);
			break;
		}
		std::cout << recvbuf << std::endl;
		send(c, "OK", 2, 0);
	}
}

int main()
{
	signal(SIGCHLD, SIG_IGN);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //选择协议族
	assert(sockfd != -1);

	sockaddr_in ser;
	ser.sin_family = PF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd, (sockaddr*)& ser, sizeof(ser));//服务绑定，让客户端可以访问
	assert(res != -1);

	listen(sockfd, 5);  //启动监听，listen与accept之间完成

	while (1)
	{
		sockaddr_in cli;
		socklen_t len = sizeof(cli);
		int c = accept(sockfd, (sockaddr*)& cli, &len);
		if (c < 0)
			continue;

		if (fork() == 0)
		{
			//子进程
			CommClient(c);
			exit(0);
		}
		else
		{
			close(c);
		}		
	}

	close(sockfd);
}
#endif

#if 0
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //选择协议族
	assert(sockfd != -1);

	sockaddr_in ser;
	ser.sin_family = PF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd, (sockaddr*)& ser, sizeof(ser));//服务绑定，让客户端可以访问
	assert(res != -1);

	listen(sockfd, 5);  //启动监听，listen与accept之间完成

	while (1)
	{
		sockaddr_in cli;
		socklen_t len = sizeof(cli);
		int c = accept(sockfd, (sockaddr*)& cli, &len);
		if (c < 0)
			continue;

		while (1)
		{
			char recvbuf[128] = { 0 };
			int n = recv(c, recvbuf, 127, 0);
			if (n <= 0)
			{
				std::cout << "one client break" << std::endl;
				close(c);
				break;
			}
			std::cout << recvbuf << std::endl;
			send(c, "OK", 2, 0);
		}
	}	

	close(sockfd);
}
#endif