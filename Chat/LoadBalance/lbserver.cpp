#include "lbserver.h"
#include "CMD5Util.h"
#include <vector>

// 负载均衡器上维护server的列表 127.0.0.1:9000
extern std::vector<std::string> _serverList;

// 绑定的回调函数，当有新连接或连接中断时调用
void LBServer::onConnection(const muduo::net::TcpConnectionPtr &con)
{
	if (con->connected()) // 有客户端连接
	{
		// 根据客户端的ip:port号计算一个离散的md5值
		long md5val = getHashVal(con->peerAddress().toIpPort().c_str());
		// 通过普通哈希算法分配一个server的ip:port，并返回
		int index = md5val % _serverList.size();
		con->send(_serverList[index].c_str());
		// lbserver主动断开连接
		con->shutdown();
	}
}